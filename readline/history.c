#include "readline.h"
#include <stdlib.h>
#include <string.h>

static char **g_hist = NULL;
static int g_count = 0;
static int g_capacity = 0;
static int g_index = -1;

void rl_history_init(void)
{
    if (!g_hist)
    {
        g_capacity = 64;
        g_hist = (char **)malloc(sizeof(char *) * g_capacity);
        g_count = 0;
        g_index = -1;
    }
}

void rl_history_add(const char *line)
{
    char **tmp;
    char *dup;
    size_t len;
    if (!line || !*line)
        return;
    rl_history_init();
    if (g_count + 1 >= g_capacity)
    {
        g_capacity *= 2;
        tmp = (char **)realloc(g_hist, sizeof(char *) * g_capacity);
        if (!tmp)
            return;
        g_hist = tmp;
    }
    len = strlen(line);
    dup = (char *)malloc(len + 1);
    if (!dup)
        return;
    memcpy(dup, line, len + 1);
    g_hist[g_count++] = dup;
    g_index = g_count; /* reset index to one past end */
}

char *rl_history_prev(void)
{
    if (!g_hist || g_count == 0)
        return NULL;
    if (g_index > 0)
        g_index--;
    if (g_index < 0)
        g_index = 0;
    return g_hist[g_index];
}

char *rl_history_next(void)
{
    if (!g_hist || g_count == 0)
        return NULL;
    if (g_index < g_count - 1)
        g_index++;
    else
    {
        g_index = g_count;
        return "";
    }
    return g_hist[g_index];
}

void rl_history_reset_index(void)
{
    g_index = g_count;
}

/* new API: find previous history entry containing pat, searching backward from from_index.
   If from_index < 0, start from last entry (g_count - 1).
   Returns index or -1 if not found. */
int rl_history_find_prev(const char *pat, int from_index)
{
    int i;

    if (!g_hist || g_count == 0 || !pat)
        return -1;
    if (from_index < 0)
        i = g_count - 1;
    else
        i = from_index - 1;
    for (; i >= 0; --i)
    {
        if (strstr(g_hist[i], pat) != NULL)
            return i;
    }
    return -1;
}

/* return pointer to history entry or NULL */
const char *rl_history_get(int idx)
{
    if (!g_hist || idx < 0 || idx >= g_count)
        return NULL;
    return g_hist[idx];
}

void rl_history_free(void)
{
    int i;
    if (!g_hist)
        return;
    for (i = 0; i < g_count; ++i)
        free(g_hist[i]);
    free(g_hist);
    g_hist = NULL;
    g_count = 0;
    g_capacity = 0;
    g_index = -1;
}

/*
 * Adapter to provide the legacy `history` symbol used by var code.
 * Signature: history(void *hist, int *he, void *unused, int histsize)
 *
 * Behaviour:
 *  - If histsize <= 0: do nothing.
 *  - If histsize < current count: drop oldest entries to fit the new size.
 *  - If histsize != current capacity: attempt to realloc the backing array.
 *  - Keeps g_index within bounds.
 *
 * This is intentionally conservative: it trims oldest entries and preserves
 * newer entries, and never crashes on allocation failure.
 */
void history(void *hist_arg, int *he, void *unused, int histsize)
{
    int i, remove, keep;
    char **tmp;

    (void)hist_arg;
    (void)he;
    (void)unused;

    if (histsize <= 0)
        return;

    /* ensure history storage exists */
    rl_history_init();

    /* If new size is smaller than current count, drop oldest entries */
    if (histsize < g_count)
    {
        remove = g_count - histsize;
        /* free oldest 'remove' entries */
        for (i = 0; i < remove; ++i)
            free(g_hist[i]);
        /* shift remaining entries toward start */
        keep = g_count - remove;
        for (i = 0; i < keep; ++i)
            g_hist[i] = g_hist[i + remove];
        g_count = keep;
        /* adjust index if necessary */
        if (g_index > g_count)
            g_index = g_count;
    }

    /* If capacity needs change, attempt to realloc backing array */
    if (histsize != g_capacity)
    {
        tmp = (char **)realloc(g_hist, sizeof(char *) * (size_t)histsize);
        if (tmp)
        {
            g_hist = tmp;
            g_capacity = histsize;
        }
        /* if realloc failed, keep old capacity to remain safe */
    }

    /* Ensure index is valid (one past last entry allowed) */
    if (g_index > g_count)
        g_index = g_count;
}
