int searchLinkedList(struct Node *head, int x) 
{
    for (struct Node *curr = head; curr != NULL; curr= curr->next )
    {
        if(curr->data == x)
            return (1);
    }
    return (0);
}
