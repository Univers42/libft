#include "ft_list.h"
#include <stdlib.h>

t_list *createLinkedList(int arr[], int n)
{
    if (n == 0)
        return (NULL);

    int *first_val = malloc(sizeof(int));
    if (!first_val)
        return NULL;
    *first_val = arr[0];
    t_list *head = ft_lstnew(first_val);
    if (!head)
    {
        free(first_val);
        return NULL;
    }
    t_list *tail = head;
    int i;
    for (i = 1; i < n; i++)
    {
        int *val = malloc(sizeof(int));
        if (!val)
            break;
        *val = arr[i];
        tail->next = ft_lstnew(val);
        if (!tail->next)
        {
            free(val);
            break;
        }
        tail = tail->next;
    }
    return (head);
}
