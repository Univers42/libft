#include "ft_list.h"
#include <stdlib.h>

t_list *deleteAtPosition(t_list *head, int pos)
{
    int position;
    if(!head)
        return NULL;
        
    if(pos == 1)
    {
        t_list *temp = head;
        head = head->next;
        free(temp);
        return head;
    }
    
    t_list *curr = head;
    position = 1;
    
    while(curr != NULL && position < pos - 1)
    {
        curr = curr->next;
        position++;
    }
    if(curr == NULL || curr->next == NULL)
        return head;

    t_list *temp = curr->next;
    curr->next = temp->next;
    free(temp);
    return head;
}
