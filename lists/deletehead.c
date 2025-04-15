struct Node * deleteHead(struct Node *head)
{
    if (!head)
        return (NULL);
    struct Node *newHead = head->next;
    free(head);
    return (newHead);
}
