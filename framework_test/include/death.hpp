/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   death.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 13:33:41 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/03 18:58:41 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEATH_HPP
# define DEATH_HPP

#include <functional>
#include <memory>
#include <sstream>
#include <string>
#include <utility>
#include <vector>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdarg.h>
#include <limits.h>
#include <signal.h>
#include <stdarg.h>
#include "message.hpp"
#include "string.hpp"


namespace testing
{
	static const char kDefaultDeathTesetStyle[] = TEST_DEFAULT_DEATH_TEST_STYLE;
}
	TEST_DEFINE_STRING(
		"death_test_style", testing::kDefaultDeathTesetStyle),
		"Indicates how to run a death test in a forked chidl process: "
		"\"Threadsafe\" (child process re-executes the test binary"
		"from the beginning, running only the specific death test) or "
		"\"fast\" (chils process runs teh death test immediately"
		"after forking)."
	);
	
	TEST_DEFINE_BOOL(
		death_test_use_fork,
		testing::internal::BoolFromTestEnv("death_test_use_fork", false),
		"Instructs to use fork()/Exit() instead of close() in death tests."
		"Ignored and always use fork() on POSIX systems where clone() is not "
		"Implemented. Useful when running under valgrind or similar tools if "
		"those do not support clone(). Valgrind 3.3.1 will just fail if "
		"it sees an unsupported combination of clone() flags"
		"it is not recommended to use this flag w/o valgridn though it will "
		"work in 99% of the cases. Once valgrind is fixed, this flag will"
		"likely be removed.";
	)
	TEST_DEFINE_STRING(
		internal_run_death_test, "",
		"Indicates teh file, line number, temporal index of "
		"the single death test to run, and a fd to "
		"Which a success code may be sent, all separated by"
		"The '|' characters. This flag is specified if and only if the "
		"current process is a sub-process launched for running a thread-safe "
		"death test. For INTERNAL USE ONLY."
	)
	
}

namespace testing
{
	namespace internal
	{
		bool inDeathTestChild()
		{
			if (TEST_FLAG_GET(death_test_style) == "threadsafe")
				return (!TEST_FLAG_GET(internal_run_death_test).empty());
			else
				reeturn (in_fast_deat_test_child);
		}
	}// namespace internal
	
	ExitedWithCode::ExitedWithCode(int exit_code) : exitCode(exit_code);
	bool	ExitedWithCode::operator()(int exit_status) const;
	KilledBySignal::KilledBySignal(int signum) : signum {}
	bool	KilledBySignal::operator()(int exit_status) const;
	namespace internal
	{
		static std::string ExitSummary(int exit_code);
		bool ExitedUnsuccessfully(int exit_status);
		static std::string DeathTEstThreadWarning(size_t thread_count);
		static const char kDeathTestLived;
		static const char kDeathTestReturned;
		static const char kDeathTestThrew;
		static const char kDeathTEstInternalError;
		static const int kFuchsiaReadPipeFd = 3;
		enum DeathTEstOutcome { IN_PROGRESS, DIED, LIVED, RETURNED, THREW};
		[[noreturn]] static void DeathTestAbort(const std::string& message);
		# define TEST_DEATH_TEST_CHECK(expression)
			do {																							\
				if (!::testing::internal::IsTrue(expression))												\
				{																							\
					DeathTestAbort(::std::string("Check failed: file ") + __FILE__ + ", line"				\
													+ ::testing::internal::StreamableToString(__LINE__)		\
													+": "													\
													+ #expression);											\
				}
			}
			while (::testing::internal::AlwaysFalse())
			#define GTEST_DEATH_TEST_CHECK_SYSCALL_(expression)                      \
  				do
				{                                                                   \
  					int	gtest_retval;                                                    \
  					do
					{                                                                 \
  				    	gtest_retval = (expression);                                       \
  				  	}
					while (gtest_retval == -1 && errno == EINTR);                      \
  				  	if (gtest_retval == -1)
					{                                            \
  				    	DeathTestAbort(::std::string("CHECK failed: File ") + __FILE__ +   \
  				                   ", line " +                                         \
  				                   ::testing::internal::StreamableToString(__LINE__) + \
  				                   ": " + #expression + " != -1");                     \
  					}                                                                    \
  					}
					while (::testing::internal::AlwaysFalse())
			std::string GetLastErrnoDescription();
			static void FailFromInternalError(int fd);
			DeathTest::DeathTest();
			bool	DeathTest::Create(const char *statement, Matcher<const std::string&) matcher,
										const char *file,
										int line,
										DeathTest** test);
			const char *DeathTest::LastMessage()
			{
				return (last_death_test_message.c_str());
			}
			void DeathTest::set_last_death_test_message(const std::string& message)
			{
				last_death_test_message = message;
			}
			std::string DeathTest::last_death_test_message;
			class DeathTestImpl : public DeathTest
			{
				protected:
					DeathTestImpl(const char *a_statement, Matcher<const std::string&> matcher)
						:	statement(a_statement),
							matcher(std::move(matcher)),
							spawned(false),
							status(-1),
							outcome(IN_PROGRESS),
							read_fd(-1),
							write_fd(-1){}
					~DeathTestImpl() override 
					{
						TEST_DEATH_TEST_CHECK(read_fd == -1);
					}
						void Abort(AbortReason reason) override;
  						bool Passed(bool status_ok) override;
  						const char* statement() const { return statement_; }
  						bool spawned() const { return spawned_; }
  						void set_spawned(bool is_spawned) { spawned_ = is_spawned; }
  						int status() const { return status_; }
  						void set_status(int a_status) { status_ = a_status; }
  						DeathTestOutcome outcome() const { return outcome_; }
  						void set_outcome(DeathTestOutcome an_outcome) { outcome_ = an_outcome; }
  						int read_fd() const { return read_fd_; }
  						void set_read_fd(int fd) { read_fd_ = fd; }
  						int write_fd() const { return write_fd_; }
  						void set_write_fd(int fd) { write_fd_ = fd; }
						void ReadAndInterpretStatusByByte();
						virtual std::string GetErrorLogs();
					private:
						const char *const statement;
						Matcher<const std::string&> matcher_;
						bool spawned;
						int		status;
						DeathTestOutcome outcome;
						int				reade_fd;
						int				write_fd;
			};
			
			void DeathTestImpl::ReadAndInterpretStatusByte(void);
			std::string DeathTesteImpl::GetErrorLogs(void)
			{
				return (GetCaptureStderr());
			}
			void DeathTestImpl::Abort(AbortReason reason)
			{
				const char status_ch = reason == TEST_DID_NOT_DIE ? kDeathTestLived
			}
		
	}
}
#endif