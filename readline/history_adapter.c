#include "readline.h"

/*
 * Minimal adapter to provide the classic `history` symbol used by var code.
 * Signature: history(void *hist, int *he, void *unused, int histsize)
 *
 * Keep behaviour intentionally minimal (no-op). This satisfies the linker
 * and is safe: if a real history implementation is linked later it will
 * override this symbol (or you can replace this adapter with a full one).
 */
void history(void *hist_arg, int *he, void *unused, int histsize)
{
    (void)hist_arg;
    (void)he;
    (void)unused;
    (void)histsize;
    /* no-op: adapter exists to satisfy linking; upgrade later if desired */
}
