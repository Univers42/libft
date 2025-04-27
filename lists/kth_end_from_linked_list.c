#include "ft_list.h"
#include <stddef.h>

int getNthFromLast(t_list *head, int k) {
    // If list is empty
    if (head == NULL)
        return -1;
        
    // Define two pointers - fast and slow
    t_list *fast = head;
    t_list *slow = head;
    
    // Move fast pointer k nodes ahead
    for (int i = 1; i < k; i++) {
        // If we reach the end before moving k steps, return -1
        if (fast->next == NULL)
            return -1;
        fast = fast->next;
    }
    
    // Move both pointers until fast reaches the end
    while (fast->next != NULL) {
        slow = slow->next;
        fast = fast->next;
    }
    
    // Return the data of the node where slow points
    return *(int*)(slow->content);
}
