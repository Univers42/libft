#include "ft_list.h"
#include <stdlib.h>

t_list* removeDuplicates(t_list* head) {
    if (head == NULL || head->next == NULL)
        return (head);
    
    t_list *current = head;
    while(current != NULL && current->next != NULL)
    {
        if (*(int*)(current->content) == *(int*)(current->next->content))
        {
            t_list *duplicate = current->next;
            current->next = current->next->next;
            free(duplicate);
        }
        else
            current = current->next;
    }
    return (head);
}
