#include "ft_list.h"

t_list *createLinkedList (int arr[], int n)
{
    if (n == 0)
        return (NULL);
    
    t_list *head = ft_lstnew(arr[0])
    t_list *tail = head;
    while(int i = 1; i < n; i++)
    {
        tail->next = ft_lstnew(arr[i]);
        tail = tail->next;
    }
    return (head);

}
