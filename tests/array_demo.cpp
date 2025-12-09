#ifdef __cplusplus
/* avoid C++ keyword 'new' in header by mapping it to new_ just while including */
#define new new_
extern "C"
{
#endif
#include "ds.h"
#ifdef __cplusplus
}
#undef new
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static void print_meta(const char *tag, t_arr *a)
{
    if (!a)
    {
        printf("%s: <null array>\n", tag);
        return;
    }
    {
        size_t ne = array_num_elements(a);
        size_t mx = array_max_index(a);
        size_t fi = array_first_index(a);
        size_t al = array_alloc_size(a);

        printf("%s: nelem=%zu max_idx=", tag, ne);
        if (mx == (size_t)-1)
            printf("none");
        else
            printf("%zu", mx);
        printf(" first_idx=");
        if (ne == 0)
            printf("none");
        else
            printf("%zu", fi);
        printf(" alloc=%zu\n", al);
    }
}

static void debug_meta_separate(const char *tag, t_arr *a)
{
    printf("%s: ", tag ? tag : "(null)");
    printf("nelem=%zu ", array_num_elements(a));
    if (array_max_index(a) == (size_t)-1)
        printf("max_idx=none ");
    else
        printf("max_idx=%zu ", array_max_index(a));
    if (array_num_elements(a) == 0)
        printf("first_idx=none ");
    else
        printf("first_idx=%zu ", array_first_index(a));
    printf("alloc=%zu\n", array_alloc_size(a));
}

static void dump_array(const char *title, t_arr *a)
{
    t_arr_elem *e;
    printf("%s\n", title ? title : "array");
    if (!a || array_empty(a))
    {
        printf("  <empty>\n");
        return;
    }
    for (e = element_forw(a->head); e != a->head; e = element_forw(e))
        printf("  [%zu] %s\n", element_index(e), element_value(e) ? element_value(e) : "(null)");
}

int main(void)
{
    t_arr *a;
    char *v;
    t_arr_elem *el;
    t_arr *cpy;
    t_arr *slice;
    t_arr_elem **elems;
    size_t i;

    puts("=== demo_all: start ===");

    /* create and push values */
    a = array_create();
    array_push(a, strdup("one"));
    array_push(a, strdup("two"));
    array_push(a, strdup("three"));
    print_meta("after pushes", a);
    dump_array("contents", a);

    /* pop / shift */
    array_pop(a); /* removes last pushed element */
    print_meta("after pop", a);
    dump_array("contents", a);

    /* shift (pop front) */
    if (array_shift(a, &v) == 0)
    {
        printf("shifted value: %s\n", v);
        free(v);
    }
    print_meta("after shift", a);
    dump_array("contents", a);

    /* push and rshift (push front) */
    array_rshift(a, 1, strdup("front"));
    print_meta("after rshift(front)", a);
    dump_array("contents", a);

    /* use array_shift_element / array_unshift_element */
    array_shift_element(a, strdup("elem_pushed_front"));
    print_meta("after array_shift_element", a);
    dump_array("contents", a);

    el = array_unshift_element(a); /* remove and return first element */
    if (el)
    {
        printf("unshift_element returned: %s\n", element_value(el));
        array_dispose_element(el);
    }
    print_meta("after unshift_element", a);
    dump_array("contents", a);

    /* insert at index and replace */
    array_insert(a, 5, strdup("index5"));
    array_insert(a, 1, strdup("at1"));
    print_meta("after inserts", a);
    dump_array("contents", a);

    /* replace value using array_value_replace (takes ownership of new value) */
    if (array_value_replace(a, 1, strdup("replaced_at1"), NULL) == 0)
        printf("replaced index 1\n");
    dump_array("contents after replace", a);

    /* reference lookup */
    v = array_reference(a, 5);
    printf("reference index 5 -> %s\n", v ? v : "(null)");

    /* copy and slice */
    cpy = array_copy(a);
    print_meta("copy meta", cpy);
    dump_array("copy contents", cpy);

    /* safe slice: first 2 elements */
    slice = array_slice_safe(a, 0, 2);
    print_meta("slice meta", slice);
    debug_meta_separate("slice meta (separate)", slice); /* isolated print */
    dump_array("slice contents", slice);

    /* array_shift_safe: take up to 3 front elements */
    elems = array_shift_safe(a, 3, 0);
    if (elems)
    {
        printf("array_shift_safe returned elements:\n");
        for (i = 0; elems[i] != NULL; ++i)
        {
            printf("  [%zu] %s\n", i, element_value(elems[i]) ? element_value(elems[i]) : "(null)");
        }
        array_dispose_elements(elems);
    }
    print_meta("after shift_safe", a);
    debug_meta_separate("after shift_safe (separate)", a); /* isolated print */
    dump_array("contents", a);

    /* test element-level helpers */
    el = array_create_element(42, strdup("forty two"));
    printf("created element: idx=%zu val=%s\n", element_index(el), element_value(el));
    array_element_replace(el, strdup("42"));
    printf("after element_replace: %s\n", element_value(el));
    array_dispose_element(el);

    /* lastref helpers exercise */
    set_lastref(a, element_forw(a->head));
    if (is_lastref(a))
        printf("lastref set to index %zu\n", element_index(lastref(a)));
    unset_lastref(a);
    invalidate_lastref(a);

    /* other setters */
    set_first_index(a, 100);
    set_num_elements(a, array_num_elements(a));
    set_max_idx(a, array_max_index(a));

    print_meta("final", a);
    debug_meta_separate("final (separate)", a); /* isolated print */
    dump_array("final contents", a);

    /* cleanup */
    array_dispose(cpy);
    array_dispose(slice);
    array_dispose(a);

    puts("=== demo_all: end ===");
    return 0;
}
