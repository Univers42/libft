#include "ft_list.h"

int isSorted(t_list *head)
{
    if (!head || !head->next)
        return 1;
    int nonDecreasing = 1;
    int nonIncreasing = 1;
    
    t_list *current = head;
    while(current->next)
    {
        if (*(int*)(current->content) > *(int*)(current->next->content))
            nonDecreasing = 0;
        if (*(int*)(current->content) < *(int*)(current->next->content))
            nonIncreasing = 0;
        if (!nonDecreasing && !nonIncreasing)
            return 0;
        current = current->next;
    }
    return (1);
}
