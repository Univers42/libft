int isSorted(struct Node *head)
{
    if (!head || !head->next)
        return 1;
    int nonDecreasing = 1;
    int nonIncreasing = 1;
    
    struct Node *current = head;
    while(current->next)
    {
        if (current->data > current->next->data)
            nonDecreasing = 0;
        if (current->data < current->next->data)
            nonIncreasing = 0;
        if (!nonDecreasing && !nonIncreasing)
            return 0;
        current = current->next;
    }
    return (1);
}
