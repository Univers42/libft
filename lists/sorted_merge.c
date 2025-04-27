#include "ft_list.h"
#include <stddef.h>

t_list* sortedMerge(t_list* head1, t_list* head2) {
    t_list dummy;
    //we use a stack-allocated dummy node instead of dynamically allocating one
    //which eliminate the need for createNode(and avoid potential memory leaks)
    t_list *curr = &dummy;
    while(head1 != NULL && head2 != NULL)
    {
        if (*(int*)(head1->content) <= *(int*)(head2->content))
        {
            curr->next = head1;
            head1 = head1->next;
        }
        else
        {
            curr->next = head2;
            head2 = head2->next;
        }
        curr = curr->next;
    }
    if(head1 == NULL)
        curr->next = head2;
    else
        curr->next = head1;
    return (dummy.next);
}
