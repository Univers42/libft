#include "ft_sort.h"

static t_sort_node *create_node(int value, int index)
{
    t_sort_node *node;

    node = malloc(sizeof(t_sort_node));
    if (!node)
        return (NULL);
    node->value = value;
    node->index = index;
    node->left = NULL;
    node->right = NULL;
    return (node);
}

static t_sort_node *insert_cartesian(t_sort_node *root, int value, int index)
{
    if (!root)
        return (create_node(value, index));
    if (value > root->value)
    {
        root->right = insert_cartesian(root->right, value, index);
    }
    else
    {
        root->left = insert_cartesian(root->left, value, index);
    }
    return (root);
}

static void inorder_traversal(t_sort_node *root, int *result, int *idx)
{
    if (!root)
        return;
    inorder_traversal(root->left, result, idx);
    result[(*idx)++] = root->value;
    inorder_traversal(root->right, result, idx);
}

static void free_tree(t_sort_node *root)
{
    if (!root)
        return;
    free_tree(root->left);
    free_tree(root->right);
    free(root);
}

void cartesian_sort(int *arr, int size)
{
    t_sort_node *root;
    int *result;
    int idx;
    int i;

    if (!arr || size <= 0)
        return;
    root = NULL;
    result = malloc(size * sizeof(int));
    if (!result)
        return;
    idx = 0;
    i = 0;
    while (i < size)
    {
        root = insert_cartesian(root, arr[i], i);
        i++;
    }
    inorder_traversal(root, result, &idx);
    ft_memcpy(arr, result, size * sizeof(int));
    free(result);
    free_tree(root);
}
