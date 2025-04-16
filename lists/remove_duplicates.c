struct Node* removeDuplicates(struct Node* head) {
    if (head == NULL || head->next == NULL)
        return (head);
    struct Node *current = head;
    while(current != NULL && current->next != NULL)
    {
        if (current->data == current->next->data)
        {
            struct Node *duplicate = current->next;
            current->next = current->next->next;
        }
        else
            current = current->next;
    }
    return (head);
}
