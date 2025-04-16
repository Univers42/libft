struct Node* sortedMerge(struct Node* head1, struct Node* head2) {
    struct Node dummy;
    //we use a stack-allocated dummy node instead of dynamically allocating one
    //which eliminate the need for createNode(and avoid potential memory leaks)
    struct Node *curr = &dummy;
    while(head1 != NULL && head2 != NULL)
    {
        if (head1->data <= head2->data)
        {
            curr->next = head1;
            head1 = head1->next;
        }
        else
        {
            curr->next = head2;
            head2 = head2->next;
        }
        curr = curr->next;
    }
    if(head1 == NULL)
        curr->next = head2;
    else
        curr->next = head1;
    return (dummy.next);
}
