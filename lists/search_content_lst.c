#include "ft_list.h"
#include <stddef.h>

int searchLinkedList(t_list *head, int x) 
{
    for (t_list *curr = head; curr != NULL; curr = curr->next)
    {
        if(*(int*)(curr->content) == x)
            return (1);
    }
    return (0);
}
