/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   leaks.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 00:11:15 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/19 03:05:31 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "leaks.hpp"
#include "symbolizer.hpp"
#include <dlfcn.h>
#include <unistd.h>
#include <algorithm>
#include <string_view>
#include <chrono>
#include <signal.h>
#include <execinfo.h> // for backtrace
#include <sstream>
#include <fstream>
#include <cstring>		 // for ft_strlen/strcmp
#include <unordered_map> // added for LogRecorder

namespace memcheck
{

	// global allocation store and control
	static std::vector<Allocation> g_allocations;
	static std::mutex g_alloc_mutex;
	static std::atomic<bool> g_tracking_active{false};
	static std::atomic<bool> g_show_test{true}; // mirrors original showTest behavior

	// signal-safe mirror of tracked allocation count
	static volatile sig_atomic_t g_sig_tracked_alloc_count = 0;
	static std::atomic<int> g_tracked_alloc_count{0};

	// reentrancy guard to avoid calling into allocator while we're updating data structures
	thread_local bool g_in_alloc_hook = false;

	// new: did we produce a leak report?
	static std::atomic<bool> g_had_leaks{false};

	void onAlloc(void *p, std::size_t size)
	{
		if (!p)
			return;

		// reentrancy guard
		if (g_in_alloc_hook)
			return;
		g_in_alloc_hook = true;

		// capture tracking state
		bool was_tracked = g_tracking_active.load(std::memory_order_relaxed);

		// capture backtrace (small depth)
		const int MAX_BT = 16;
		void *btbuf[MAX_BT];
		int bt_size = backtrace(btbuf, MAX_BT);
		std::vector<void *> btvec;
		for (int i = 0; i < bt_size; ++i)
			btvec.push_back(btbuf[i]);

		// thread id
		std::ostringstream tid_ss;
		tid_ss << std::this_thread::get_id();

		{
			std::lock_guard<std::mutex> lk(g_alloc_mutex);
			Allocation a(p, size, was_tracked);
			a.backtrace = std::move(btvec);
			a.thread_id = tid_ss.str();
			a.ts = std::chrono::system_clock::now();
			g_allocations.push_back(std::move(a));
		}

		if (was_tracked)
		{
			g_tracked_alloc_count.fetch_add(1, std::memory_order_relaxed);
			g_sig_tracked_alloc_count = g_sig_tracked_alloc_count + 1;
		}

		if (const char *dbg = std::getenv("MEMCHECK_DEBUG"); dbg && dbg[0] == '1')
		{
			const char msg[] = "memcheck: recorded alloc\n";
			(void)write(2, msg, sizeof(msg) - 1);
		}
		g_in_alloc_hook = false;
	}

	void onAllocTag(void *p, std::size_t size, std::string const &tag)
	{
		if (!p)
			return;
		// simple wrapper: capture then set tag (reuse onAlloc)
		// avoid double-capture; replicate logic minimally
		if (g_in_alloc_hook)
			return;
		g_in_alloc_hook = true;
		bool was_tracked = g_tracking_active.load(std::memory_order_relaxed);

		const int MAX_BT = 16;
		void *btbuf[MAX_BT];
		int bt_size = backtrace(btbuf, MAX_BT);
		std::vector<void *> btvec;
		for (int i = 0; i < bt_size; ++i)
			btvec.push_back(btbuf[i]);

		std::ostringstream tid_ss;
		tid_ss << std::this_thread::get_id();

		{
			std::lock_guard<std::mutex> lk(g_alloc_mutex);
			Allocation a(p, size, was_tracked);
			a.backtrace = std::move(btvec);
			a.thread_id = tid_ss.str();
			a.tag = tag;
			a.ts = std::chrono::system_clock::now();
			g_allocations.push_back(std::move(a));
		}

		if (was_tracked)
		{
			g_tracked_alloc_count.fetch_add(1, std::memory_order_relaxed);
			g_sig_tracked_alloc_count = g_sig_tracked_alloc_count + 1;
		}
		g_in_alloc_hook = false;
	}

	void onFree(void *p)
	{
		if (!p)
			return;

		// avoid reentrancy
		if (g_in_alloc_hook)
			return;

		g_in_alloc_hook = true;
		bool removed = false;
		bool was_tracked = false;
		{
			std::lock_guard<std::mutex> lk(g_alloc_mutex);
			auto it = std::find_if(g_allocations.begin(), g_allocations.end(),
								   [p](Allocation const &a)
								   { return a.ptr == p; });
			if (it != g_allocations.end())
			{
				was_tracked = it->tracked;
				g_allocations.erase(it);
				removed = true;
			}
		}
		if (removed && was_tracked)
		{
			g_tracked_alloc_count.fetch_sub(1, std::memory_order_relaxed);
			// avoid underflow; sig_atomic_t is signed
			if (g_sig_tracked_alloc_count > 0)
				g_sig_tracked_alloc_count = g_sig_tracked_alloc_count - 1;

			// lightweight debug output if user enables MEMCHECK_DEBUG=1
			if (const char *dbg = std::getenv("MEMCHECK_DEBUG"); dbg && dbg[0] == '1')
			{
				const char msg[] = "memcheck: recorded free\n";
				(void)write(2, msg, sizeof(msg) - 1);
			}
		}
		g_in_alloc_hook = false;
	}

	void setTrackingActive(bool active) noexcept { g_tracking_active.store(active, std::memory_order_relaxed); }
	bool isTrackingActive() noexcept { return g_tracking_active.load(std::memory_order_relaxed); }

	std::vector<Allocation> snapshotAllocations()
	{
		std::lock_guard<std::mutex> lk(g_alloc_mutex);
		return g_allocations;
	}

	// LoggerObserver
	void LoggerObserver::notify(std::string_view msg)
	{
		// simple stderr logger, keep minimal and non-throwing
		write(2, msg.data(), msg.size());
		write(2, "\n", 1);
	}

	// LogRecorder implementation (simple thread-safe collector + counters)
	class LogRecorderImpl
	{
	public:
		std::mutex m;
		std::vector<std::string> logs;
		std::unordered_map<std::string, int> counts;

		void push(std::string_view msg)
		{
			std::lock_guard<std::mutex> lk(m);
			logs.emplace_back(msg);
			// simple classification: increment counts for some known tokens and all messages by exact text
			if (std::string_view(msg).find("LEAKS.KO") != std::string_view::npos)
				++counts["LEAKS"];
			if (std::string_view(msg).find("memcheck: started") != std::string_view::npos)
				++counts["started"];
			if (std::string_view(msg).find("memcheck: stopped") != std::string_view::npos)
				++counts["stopped"];
			if (std::string_view(msg).find("memcheck: report done") != std::string_view::npos)
				++counts["report_done"];
			// per-message exact count
			++counts[std::string(msg)];
		}

		std::vector<std::string> snapshot()
		{
			std::lock_guard<std::mutex> lk(m);
			return logs;
		}

		int count(const std::string &key)
		{
			std::lock_guard<std::mutex> lk(m);
			auto it = counts.find(key);
			return it == counts.end() ? 0 : it->second;
		}

		void clear()
		{
			std::lock_guard<std::mutex> lk(m);
			logs.clear();
			counts.clear();
		}
	};

	// single global recorder instance
	static LogRecorderImpl &recorder()
	{
		static LogRecorderImpl inst;
		return inst;
	}

	// LogRecorder (Observer) wrapper that forwards to the static recorder
	void LogRecorder::notify(std::string_view msg)
	{
		recorder().push(msg);
	}

	std::vector<std::string> LogRecorder::getLogs()
	{
		return recorder().snapshot();
	}

	void LogRecorder::clearLogs()
	{
		recorder().clear();
	}

	int LogRecorder::countReason(std::string const &keyword)
	{
		return recorder().count(keyword);
	}

	// LeakCheck implementation
	void LeakCheck::start()
	{
		// clear previous allocations and enable tracking
		{
			std::lock_guard<std::mutex> lk(g_alloc_mutex);
			g_allocations.clear();
		}
		// reset leak flag when starting a new run
		g_had_leaks.store(false, std::memory_order_relaxed);
		setTrackingActive(true);
	}

	void LeakCheck::stop()
	{
		setTrackingActive(false);
	}

	void LeakCheck::report()
	{
		auto snap = snapshotAllocations();
		if (snap.empty())
			return;

		// load suppression patterns (if any)
		std::vector<std::string> suppress;
		if (const char *sp = std::getenv("MEMCHECK_SUPPRESS"))
		{
			std::ifstream f(sp);
			std::string line;
			while (std::getline(f, line))
			{
				if (!line.empty())
					suppress.push_back(line);
			}
		}

		// control how many frames to show and how many to skip
		int skip = 1;		// default skip top frame
		int maxFrames = 10; // default frames to show
		if (const char *sskip = std::getenv("MEMCHECK_SKIP"))
			skip = atoi(sskip);
		if (skip < 0)
			skip = 0;
		if (const char *sframes = std::getenv("MEMCHECK_FRAMES"))
			maxFrames = atoi(sframes);
		if (maxFrames < 0)
			maxFrames = 0;

		bool json_out = false;
		if (const char *j = std::getenv("MEMCHECK_JSON"))
			if (j && j[0] == '1')
				json_out = true;

		// fast mode: skip addr2line/symbolization and only print raw addresses
		bool fast_mode = false;
		// explicit override (env)
		if (const char *f = std::getenv("MEMCHECK_FAST"))
			if (f && f[0] == '1')
				fast_mode = true;
		// auto-enable fast mode when too many leaks (avoid long addr2line runs)
		if (!fast_mode)
		{
			int default_thresh = 20;
			int thresh = default_thresh;
			if (const char *t = std::getenv("MEMCHECK_AUTO_FAST"))
				thresh = atoi(t) > 0 ? atoi(t) : default_thresh;
			// count tracked allocations
			int tracked = 0;
			for (auto const &a : snap)
				if (a.tracked)
					++tracked;
			if (tracked > thresh)
				fast_mode = true;
		}

		if (json_out)
		{
			std::ostringstream out;
			out << "[\n";
			bool first = true;
			for (auto const &a : snap)
			{
				if (!a.tracked)
					continue;

				// build frames representation: either symbolized or raw hex strings
				std::vector<std::string> frames;
				if (!fast_mode)
				{
					auto sym = symbolizer::symbolize_many(a.backtrace, skip);
					if ((int)sym.size() > maxFrames)
						sym.resize(maxFrames);
					frames = std::move(sym);
				}
				else
				{
					// raw hex addresses (skip top frames)
					size_t s = (skip < 0) ? 0 : (size_t)skip;
					for (size_t i = s; i < a.backtrace.size() && (int)frames.size() < maxFrames; ++i)
					{
						std::ostringstream ss;
						ss << "0x" << std::hex << (uintptr_t)a.backtrace[i];
						frames.push_back(ss.str());
					}
				}

				// suppression check (on frames text)
				bool skip_this = false;
				for (auto const &fline : frames)
				{
					for (auto const &pat : suppress)
						if (!pat.empty() && fline.find(pat) != std::string::npos)
						{
							skip_this = true;
							break;
						}
					if (skip_this)
						break;
				}
				if (skip_this)
					continue;

				if (!first)
					out << ",\n";
				first = false;
				out << "  {\n";
				out << "    \"ptr\": \"" << a.ptr << "\",\n";
				out << "    \"size\": " << a.size << ",\n";
				out << "    \"thread\": \"" << a.thread_id << "\",\n";
				if (!a.tag.empty())
					out << "    \"tag\": \"" << a.tag << "\",\n";
				out << "    \"backtrace\": [\n";
				for (size_t i = 0; i < frames.size(); ++i)
				{
					out << "      \"";
					for (char c : frames[i])
						if (c == '"')
							out << '\\' << '"';
						else
							out << c;
					out << "\"";
					if (i + 1 < frames.size())
						out << ",\n";
					else
						out << "\n";
				}
				out << "    ]\n";
				out << "  }";
			}
			out << "\n]\n";
			auto s = out.str();
			write(1, s.c_str(), s.size());
			// mark that we produced a leak report
			g_had_leaks.store(true, std::memory_order_relaxed);
			return;
		}

		// plain text output (detailed symbolized backtrace or fast raw addresses)
		std::ostringstream ss;
		ss << "\033[31mLEAKS.KO\n";
		for (auto const &a : snap)
		{
			if (!a.tracked)
				continue;

			// gather frames: symbolized or raw hex
			std::vector<std::string> frames;
			if (!fast_mode)
			{
				auto sym = symbolizer::symbolize_many(a.backtrace, skip);
				if ((int)sym.size() > maxFrames)
					sym.resize(maxFrames);
				frames = std::move(sym);
			}
			else
			{
				size_t sidx = (skip < 0) ? 0 : (size_t)skip;
				for (size_t i = sidx; i < a.backtrace.size() && (int)frames.size() < maxFrames; ++i)
				{
					std::ostringstream as;
					as << "0x" << std::hex << (uintptr_t)a.backtrace[i];
					frames.push_back(as.str());
				}
			}

			// suppression check
			bool skip_this = false;
			for (auto const &fl : frames)
			{
				for (auto const &pat : suppress)
					if (!pat.empty() && fl.find(pat) != std::string::npos)
					{
						skip_this = true;
						break;
					}
				if (skip_this)
					break;
			}
			if (skip_this)
				continue;

			// header
			ss << "- [" << a.ptr << " : " << a.size << "]";
			ss << " (thread=" << a.thread_id;
			if (!a.tag.empty())
				ss << ", tag=" << a.tag;
			ss << ")\n";

			// raw addresses (when fast_mode we already used hex strings in frames)
			if (fast_mode && !frames.empty())
			{
				ss << "    raw addresses:\n";
				for (auto const &addr_str : frames)
					ss << "      " << addr_str << "\n";
			}
			else
			{
				// symbolized frames
				if (!frames.empty())
				{
					ss << "    symbolized frames:\n";
					for (auto const &fline : frames)
						ss << "      " << fline << "\n";
				}
				else
				{
					ss << "    (no symbolized frames)\n";
				}
			}
			ss << "\n";
		}
		ss << "\033[0m";
		auto s = ss.str();
		write(1, s.c_str(), s.size());
		if (g_show_test)
			write(1, "\n", 1);

		// mark that we produced a leak report
		g_had_leaks.store(true, std::memory_order_relaxed);
	}

	// MemCheckFacade
	MemCheckFacade::MemCheckFacade() noexcept
	{
		// always add the core strategy
		addStrategy(std::make_unique<LeakCheck>());

		// always register the log recorder (non-verbose collector)
		addObserver(std::make_shared<LogRecorder>());

		// Add a logger observer only when compiled with MEMCHECK_OBSERVER.
#ifdef MEMCHECK_OBSERVER
		addObserver(std::make_shared<LoggerObserver>());
#endif
	}

	void MemCheckFacade::addStrategy(std::unique_ptr<MemCheckStrategy> s)
	{
		if (s)
			strategies_.push_back(std::move(s));
	}

	void MemCheckFacade::addObserver(std::shared_ptr<Observer> o)
	{
		if (o)
			observers_.push_back(std::move(o));
	}

	void MemCheckFacade::startAll()
	{
		// ensure we only start once
		bool expected = false;
		if (!started_.compare_exchange_strong(expected, true))
			return;

		for (auto &s : strategies_)
			if (s)
				s->start();
		notifyObservers("memcheck: started");
	}

	void MemCheckFacade::stopAll()
	{
		// ensure we only stop once (only when previously started)
		bool expected = true;
		if (!started_.compare_exchange_strong(expected, false))
			return;

		for (auto &s : strategies_)
			if (s)
				s->stop();
		notifyObservers("memcheck: stopped");
	}

	void MemCheckFacade::reportAll()
	{
		for (auto &s : strategies_)
			if (s)
				s->report();

		// notify observers only when debugging is enabled or leaks were reported
		bool dbg = false;
		if (const char *d = std::getenv("MEMCHECK_DEBUG"))
			dbg = (d[0] == '1');
		if (dbg || g_had_leaks.load(std::memory_order_relaxed))
			notifyObservers("memcheck: report done");
	}

	void MemCheckFacade::notifyObservers(std::string_view msg)
	{
		// compile-time control: the observer is only meaningful if MEMCHECK_OBSERVER was defined.
		// However, if we detected leaks (g_had_leaks), we still want to notify any observers that exist.
#ifdef MEMCHECK_OBSERVER
		const bool compile_enable = true;
#else
		const bool compile_enable = false;
#endif
		if (!compile_enable && !g_had_leaks.load(std::memory_order_relaxed))
			return;
		for (auto &o : observers_)
			if (o)
				o->notify(msg);
	}

	// RAIICollector
	RAIICollector::RAIICollector()
	{
		// start all checks on construction
		facade_.startAll();
	}

	RAIICollector::~RAIICollector()
	{
		// stop and report on destruction
		facade_.stopAll();
		facade_.reportAll();
	}

} // namespace memcheck

// --- async-signal-safe helpers and signal handler ---

// make the simple signal flag visible to C code (checked by main.c)
// single definition, must be visible before the handler below
extern "C" volatile sig_atomic_t memcheck_got_signal = 0;

// keep previous handlers to restore later (define here so they're in scope)
static struct sigaction g_old_sigint_action{};
static struct sigaction g_old_sigterm_action{};
static bool g_signal_handlers_installed = false;

static void write_uint_to_fd(unsigned int v, int fd)
{
	char buf[20];
	int i = 0;
	if (v == 0)
	{
		buf[i++] = '0';
	}
	else
	{
		unsigned int x = v;
		char tmp[20];
		int t = 0;
		while (x > 0 && t < (int)sizeof(tmp))
		{
			tmp[t++] = '0' + (x % 10);
			x /= 10;
		}
		while (t > 0)
			buf[i++] = tmp[--t];
	}
	// write is async-signal-safe
	write(fd, buf, (size_t)i);
}

static void memcheck_sig_handler(int sig)
{
	// mark that a signal was received; do minimal work in handler.
	(void)sig;
	memcheck_got_signal = 1;
	// return to allow program to handle shutdown gracefully
}

// make the single facade visible before the C API functions that use it
static memcheck::MemCheckFacade g_memcheck_facade;

// add idempotency flags to avoid duplicate reports (e.g. explicit + atexit)
static std::atomic<bool> g_reported{false};
static std::atomic<bool> g_atexit_registered{false};

extern "C"
{

	void memcheck_start(void)
	{
		// start tracking (starts strategies)
		g_memcheck_facade.startAll();

		// register atexit handler once so stop+report run on normal exit
		if (!g_atexit_registered.load(std::memory_order_acquire))
		{
			// try to register; mark registered regardless of atexit result to avoid repeated attempts
			std::atexit([]()
						{
							// always stop, but report only if it hasn't been reported already
							g_memcheck_facade.stopAll();
							// ensure only one thread/process emits the detailed report
							bool expected = false;
							if (g_reported.compare_exchange_strong(expected, true))
								g_memcheck_facade.reportAll(); });
			g_atexit_registered.store(true, std::memory_order_release);
		}

		// install signal handlers (only once)
		if (!g_signal_handlers_installed)
		{
			struct sigaction act;
			act.sa_handler = memcheck_sig_handler;
			sigemptyset(&act.sa_mask);
			act.sa_flags = SA_RESTART;

			// SIGINT
			(void)sigaction(SIGINT, &act, &g_old_sigint_action);
			// SIGTERM
			(void)sigaction(SIGTERM, &act, &g_old_sigterm_action);

			g_signal_handlers_installed = true;
		}
	}

	void memcheck_stop(void)
	{
		// stop tracking
		g_memcheck_facade.stopAll();

		// restore previous signal handlers if we installed them
		if (g_signal_handlers_installed)
		{
			(void)sigaction(SIGINT, &g_old_sigint_action, nullptr);
			(void)sigaction(SIGTERM, &g_old_sigterm_action, nullptr);
			g_signal_handlers_installed = false;
		}
	}

	void memcheck_report(void)
	{
		// make report idempotent: only the first caller emits the full report
		bool expected = false;
		if (g_reported.compare_exchange_strong(expected, true))
		{
			g_memcheck_facade.reportAll();
		}
	}

	// New explicit registration APIs for C tests:
	void memcheck_register_alloc(void *p, size_t size)
	{
		if (p)
			memcheck::onAlloc(p, size);
	}
	void memcheck_register_alloc_tag(void *p, size_t size, const char *tag)
	{
		if (p)
			memcheck::onAllocTag(p, size, tag ? std::string(tag) : std::string());
	}
	void memcheck_register_free(void *p)
	{
		if (p)
			memcheck::onFree(p);
	}

	// New convenience wrappers: allocate/free and register/unregister in one call.
	// These wrap the real libc functions (via dlsym) to avoid duplicating calls.
	void *memcheck_malloc(size_t size)
	{
		using libc_malloc_t = void *(*)(size_t);
		static libc_malloc_t libc_malloc = nullptr;
		if (!libc_malloc)
			libc_malloc = (libc_malloc_t)dlsym(RTLD_NEXT, "malloc");
		void *p = libc_malloc ? libc_malloc(size) : nullptr;
		if (p)
			memcheck::onAlloc(p, size);
		return p;
	}

	void memcheck_free(void *p)
	{
		// unregister first so bookkeeping reflects imminent free
		if (p)
			memcheck::onFree(p);

		using libc_free_t = void (*)(void *);
		static libc_free_t libc_free = nullptr;
		if (!libc_free)
			libc_free = (libc_free_t)dlsym(RTLD_NEXT, "free");
		if (libc_free)
			libc_free(p);
	}

	char *memcheck_strdup(const char *s)
	{
		if (!s)
			return nullptr;
		using libc_strdup_t = char *(*)(const char *);
		static libc_strdup_t libc_strdup = nullptr;
		if (!libc_strdup)
			libc_strdup = (libc_strdup_t)dlsym(RTLD_NEXT, "strdup");
		char *p = libc_strdup ? libc_strdup(s) : nullptr;
		if (p)
			memcheck::onAlloc(p, ft_strlen(p) + 1);
		return p;
	}

	// C API for log access
	int memcheck_log_count(const char *kind)
	{
		if (!kind)
			return 0;
		return memcheck::LogRecorder::countReason(std::string(kind));
	}

	void memcheck_dump_logs(void)
	{
		auto logs = memcheck::LogRecorder::getLogs();
		for (auto &s : logs)
		{
			// write to stdout safely
			write(1, s.c_str(), s.size());
			write(1, "\n", 1);
		}
	}
} // extern "C"