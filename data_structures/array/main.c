#include "array.h"
#include <stdio.h>

int main(void)
{
    t_array *a;
    t_arr_elem *e1;
    t_arr_elem *e2;
    t_arr_elem *e3;
    t_arr_elem *cur;

    a = array_create();
    if (!a)
        return (1);

    /* create three elements with indices 0,1,2 and simple values */
    e1 = array_create_element(0, "zero");
    e2 = array_create_element(1, "one");
    e3 = array_create_element(2, "two");

    /* insert them into list (before head so they become in order) */
    add_before(a->head, e1);
    add_before(a->head, e2);
    add_before(a->head, e3);

    /* update meta */
    a->nelem = 3;
    a->max_idx = 2;

    /* traverse and print */
    for (cur = element_forw(a->head); cur != a->head; cur = element_forw(cur))
        printf("idx=%zu val=%s\n", element_index(cur),
               element_value(cur) ? element_value(cur) : "(null)");

    /* reference element index 1 */
    printf("reference(1) => %s\n", array_reference(a, 1));

    /* cleanup */
    array_dispose(a);
    return (0);
}
