struct Node * deleteAtPosition(struct Node *head, int pos)
{
    int position;
    if(!head)
        return NULL;
        
    if(pos == 1)
    {
        struct Node *temp = head;
        head = head->next;
        free(temp);
        return head;
    }
    
    struct Node *curr = head;
    position = 1;
    
    while(curr != NULL && position < pos - 1)
    {
        curr = curr->next;
        position++;
    }
    if(curr == NULL || curr->next == NULL)
        return head;

    struct Node *temp = curr->next;
    curr->next = temp->next;
    free(temp);
    return head;
}
