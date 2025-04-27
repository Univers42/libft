#include "ft_list.h"
#include <stdlib.h>

t_list *deleteHead(t_list *head)
{
    if (!head)
        return (NULL);
    t_list *newHead = head->next;
    free(head);
    return (newHead);
}
