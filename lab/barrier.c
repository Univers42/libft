/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   barrier.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 00:17:19 by dlesieur          #+#    #+#             */
/*   Updated: 2025/11/24 00:34:20 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct s_glob {
    int suppressint;
    volatile sig_atomic_t intpending;
} t_glob;

/* single shared barrier instance */
static t_glob *init_barrier(void)
{
    static t_glob tb = {0};
    return &tb;
}

/* full compiler memory barrier */
static void barrier(void)
{
    __asm__ __volatile__ ("" : : : "memory");
}

/* behavior like the INTOFF macro: increment suppress counter and return 0 */
static inline int intoff(void)
{
    t_glob *g = init_barrier();
    g->suppressint++;
    barrier();
    return 0;
}

/* behavior like the INTON macro: decrement and handle pending if any */
static inline int inton(void)
{
    t_glob *g = init_barrier();
    barrier();
    if (--g->suppressint == 0 && g->intpending)
    {
        /* demo handler called when it is safe */
        g->intpending = 0;
        write(STDOUT_FILENO, "INT handled at inton (deferred)\n", 33);
    }
    return 0;
}

/* demo "onint" used by signal handler when suppressint==0 */
static void onint_demo(void)
{
    write(STDOUT_FILENO, "INT handled immediately by onint\n", 33);
}

/* SIGINT handler: mark pending; if not suppressed, handle immediately */
static void sigint_handler(int sig)
{
    (void)sig;
    t_glob *g = init_barrier();
    g->intpending = 1;
    if (g->suppressint == 0)
        onint_demo(); /* immediate handling */
    /* otherwise return to main; handling deferred until inton() */
}

/* critical section without suppression */
static void demo_without_barrier(void)
{
    t_glob *g = init_barrier();
    g->suppressint = 0;
    g->intpending = 0;

    puts("\nDemo A — NO suppression:");
    puts(" Entering critical section (simulate work) and raising SIGINT...");
    sleep(1);
    raise(SIGINT); /* simulate user pressing Ctrl-C */
    /* if onint_demo ran above, it already printed a message */
    usleep(200000); /* small delay to allow handler output ordering */
    puts(" After critical section (in demo A)");
}

/* critical section with suppression using intoff/inton */
static void demo_with_barrier(void)
{
    t_glob *g = init_barrier();
    g->suppressint = 0;
    g->intpending = 0;

    puts("\nDemo B — WITH suppression (intoff/inton):");
    puts(" Entering critical section (suppressed) and raising SIGINT...");
    intoff();
    sleep(1);
    raise(SIGINT); /* SIGINT arrives while suppressed */
    /* handler sets intpending but does not call onint_demo */
    usleep(200000);
    printf(" Still inside critical section: intpending=%d suppressint=%d\n",
           (int)g->intpending, g->suppressint);
    /* leaving critical section: call inton to process pending INT */
    inton();
    puts(" After inton (in demo B)");
}

int main(void)
{
    struct sigaction sa = {0};
    sa.sa_handler = sigint_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("sigaction");
        return 1;
    }

    puts("Signal suppression demo. Program will simulate SIGINT (raise).");
    demo_without_barrier();
    demo_with_barrier();

    puts("\nDemo finished.");
    return 0;
}