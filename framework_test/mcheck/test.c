/*
 * Comprehensive test for memcheck:
 * - memcheck_malloc / memcheck_free / memcheck_strdup wrappers
 * - explicit registration with tags
 * - arrays of pointers (char **), nested int** matrix
 * - partial frees to create tricky leaks
 * - threaded allocations to show thread ids in reports
 *
 * Build: make
 * Run:   ./a.out
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdint.h>
#include <unistd.h>

/* memcheck C API provided by leaks.cpp */
extern void memcheck_start(void);
extern void memcheck_stop(void);
extern void memcheck_report(void);

extern void *memcheck_malloc(size_t size);
extern void memcheck_free(void *p);
extern char *memcheck_strdup(const char *s);

/* explicit registration (when using plain malloc/strdup) */
extern void memcheck_register_alloc_tag(void *p, size_t size, const char *tag);

/* small helpers */
static char *mklabel(const char *pref, int i)
{
    char buf[128];
    snprintf(buf, sizeof(buf), "%s-%d", pref, i);
    return memcheck_strdup(buf);
}

/* Demo: array of strings (char **) with partial frees */
static void demo_string_array(void)
{
    printf("\n--- demo_string_array ---\n");
    const int N = 6;
    char **arr = memcheck_malloc(N * sizeof(char *));
    memcheck_register_alloc_tag(arr, N * sizeof(char *), "string-array/table");
    printf("allocated arr=%p (table)\n", (void *)arr);

    for (int i = 0; i < N; ++i)
    {
        arr[i] = mklabel("str", i); /* registered by memcheck_strdup */
        printf("  arr[%d] = %p\n", i, (void *)arr[i]);
    }

    /* free first half, leave second half leaked */
    for (int i = 0; i < N / 2; ++i)
    {
        memcheck_free(arr[i]); /* unregister+free */
        printf("  freed arr[%d]\n", i);
        arr[i] = NULL;
    }

    /* free outer table; inner leaked strings remain tracked */
    memcheck_free(arr);
    printf("freed arr (outer table)\n");
}

/* Demo: 2D int matrix with partial row frees */
static void demo_int_matrix(void)
{
    printf("\n--- demo_int_matrix ---\n");
    const int R = 4, C = 8;
    int **mat = memcheck_malloc(R * sizeof(int *));
    memcheck_register_alloc_tag(mat, R * sizeof(int *), "int-matrix/table");
    printf("allocated mat=%p\n", (void *)mat);

    for (int r = 0; r < R; ++r)
    {
        mat[r] = memcheck_malloc(C * sizeof(int));
        memcheck_register_alloc_tag(mat[r], C * sizeof(int), "int-matrix/row");
        for (int c = 0; c < C; ++c)
            mat[r][c] = r * 100 + c;
        printf("  mat[%d] = %p\n", r, (void *)mat[r]);
    }

    /* free even rows, leave odd rows leaked */
    for (int r = 0; r < R; ++r)
    {
        if ((r & 1) == 0)
        {
            memcheck_free(mat[r]);
            printf("  freed mat[%d]\n", r);
            mat[r] = NULL;
        }
    }

    memcheck_free(mat);
    printf("freed mat (outer table)\n");
}

/* Demo: deep pointer structure (char ***) with partial frees */
static void demo_deep_pointers(void)
{
    printf("\n--- demo_deep_pointers ---\n");
    const int B = 3;
    char ***blocks = memcheck_malloc(B * sizeof(char **));
    memcheck_register_alloc_tag(blocks, B * sizeof(char **), "blocks/top");
    printf("allocated blocks=%p\n", (void *)blocks);

    for (int b = 0; b < B; ++b)
    {
        int M = 3 + b;
        blocks[b] = memcheck_malloc(M * sizeof(char *));
        memcheck_register_alloc_tag(blocks[b], M * sizeof(char *), "blocks/array");
        for (int m = 0; m < M; ++m)
        {
            char tmp[96];
            snprintf(tmp, sizeof(tmp), "blk%d_item%d", b, m);
            blocks[b][m] = memcheck_strdup(tmp); /* registered */
        }
        printf("  blocks[%d] = %p (M=%d)\n", b, (void *)blocks[b], M);
    }

    /* free only first block fully */
    for (int m = 0; m < 3; ++m)
        memcheck_free(blocks[0][m]);
    memcheck_free(blocks[0]);
    printf("freed blocks[0] fully; others partially leaked\n");

    /* free top pointer */
    memcheck_free(blocks);
    printf("freed blocks (top)\n");
}

/* Thread worker to show thread-id entries in report */
static void *worker(void *arg)
{
    int id = (int)(intptr_t)arg;
    for (int i = 0; i < 100; ++i)
    {
        char *t = memcheck_strdup("worker-temp");
        char tag[64];
        snprintf(tag, sizeof(tag), "worker/%d/temp", id);
        memcheck_register_alloc_tag(t, strlen(t) + 1, tag);
        /* free immediately */
        memcheck_free(t);
    }
    return NULL;
}

int main(void)
{
    /* default: plain text, more frames */
    setenv("MEMCHECK_JSON", "0", 1);
    setenv("MEMCHECK_FRAMES", "12", 1);
    setenv("MEMCHECK_SKIP", "1", 1);

    printf(">>> memcheck demo starting\n");
    memcheck_start();

    demo_string_array();
    demo_int_matrix();
    demo_deep_pointers();

    /* threaded work */
    pthread_t t1, t2;
    pthread_create(&t1, NULL, worker, (void *)(intptr_t)1);
    pthread_create(&t2, NULL, worker, (void *)(intptr_t)2);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    /* create a deliberate leak registered with a tag to show in report */
    char *leak = memcheck_malloc(256);
    memcheck_register_alloc_tag(leak, 256, "intentional/leak");
    strcpy(leak, "I am intentionally leaked for demo");
    printf("\n>>> stopping memcheck and printing report\n");
    memcheck_stop();
    memcheck_report();

    return 0;
}
