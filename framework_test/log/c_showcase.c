#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <stdint.h>
#include <time.h>

// C wrappers provided by logger.cpp (C linkage)
void c_log_info(const char *msg);
void c_print_message(const char *msg, const char *color_code);
void c_run_logger_demo(void);

static const char *RESET = "\x1b[0m";
static const char *BOLD = "\x1b[1m";
static const char *DIM = "\x1b[2m";

// some common color codes (bright variants)
static const char *BRIGHT_GREEN = "\x1b[92m";
static const char *BRIGHT_YELLOW = "\x1b[93m";
static const char *BRIGHT_RED = "\x1b[91m";
static const char *BRIGHT_CYAN = "\x1b[96m";
static const char *BG_BRIGHT_RED = "\x1b[101m";

// helper: sleep for given milliseconds
static void sleep_ms(unsigned int ms)
{
    struct timespec ts;
    ts.tv_sec = ms / 1000;
    ts.tv_nsec = (ms % 1000) * 1000000L;
    nanosleep(&ts, NULL);
}

void *thread_worker(void *arg)
{
    int id = (int)(intptr_t)arg;
    char buf[256];
    for (int i = 0; i < 6; ++i)
    {
        snprintf(buf, sizeof(buf), "C-thread-%d: iteration=%d", id, i);
        // alternate colors per iteration
        const char *color = (i % 3 == 0) ? BRIGHT_CYAN : (i % 3 == 1) ? BRIGHT_YELLOW
                                                                      : BRIGHT_GREEN;
        c_print_message(buf, color);
        // also send a log entry through the simplified C log wrapper
        snprintf(buf, sizeof(buf), "C-thread-%d logged: iteration=%d", id, i);
        c_log_info(buf);
        sleep_ms(80); // 80ms
    }
    return NULL;
}

int main(void)
{
    char buf[512];

    // Heading
    c_print_message("=== C Showcase: using tester::Message + C++ logger ===\n", BOLD);

    // palette samples (using whole-message wrapper)
    c_print_message(" Standard color sample: GREEN", BRIGHT_GREEN);
    c_print_message(" Warning color sample: YELLOW", BRIGHT_YELLOW);
    c_print_message(" Error color sample: RED", BRIGHT_RED);

    // 256-color sample (print blocks using inline escapes)
    puts("\n256-color sample (short):");
    for (int i = 16; i < 36; ++i)
    {
        /* build a small inline colored fragment; use %s to append RESET */
        snprintf(buf, sizeof(buf), "\x1b[38;5;%dm %3d %s", i, i, RESET);
        // print inline colored fragment (no wrapper color prefix)
        c_print_message(buf, NULL);
        if ((i - 16 + 1) % 6 == 0)
            putchar('\n');
    }
    putchar('\n');

    // formatted memory report
    snprintf(buf, sizeof(buf), "Memory report: total=%uKB used=%uKB free=%uKB", 8192u, 3072u, 5120u);
    c_print_message(buf, DIM);

    // diff-like output with inline colors (use wrapper to color whole lines)
    c_print_message("+ expected:  function foo() { return 1; }", "\x1b[92m"); // green
    c_print_message("- actual:    function foo() { return 0; }", "\x1b[91m"); // red

    // assertion demo (background highlight)
    c_print_message("ASSERTION FAILED: expected=42 got=41", BG_BRIGHT_RED);

    // spawn several C threads that call into the C wrappers concurrently
    const int nthreads = 3;
    pthread_t thr[nthreads];
    for (int i = 0; i < nthreads; ++i)
    {
        pthread_create(&thr[i], NULL, thread_worker, (void *)(intptr_t)(i + 1));
    }

    // main thread also logs some entries
    for (int i = 0; i < 4; ++i)
    {
        snprintf(buf, sizeof(buf), "Main C thread quick log #%d", i + 1);
        c_log_info(buf);
        sleep_ms(120);
    }

    // join threads
    for (int i = 0; i < nthreads; ++i)
        pthread_join(thr[i], NULL);

    // finish with the full C++ logger demo (spawns its own threads, writes app.log)
    puts("\nNow running the full C++ logger pipeline demo (this will spawn threads and write app.log)...");
    c_run_logger_demo();

    c_print_message("\nC showcase complete.\n", BOLD);
    return 0;
}
