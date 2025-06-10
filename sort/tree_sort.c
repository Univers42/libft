/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_sort.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 19:16:13 by dlesieur          #+#    #+#             */
/*   Updated: 2025/06/10 21:48:35 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_sort.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * Binary search tree
 */

typedef struct s_tree_node
{
    int data;
    struct s_tree_node *left;
    struct s_tree_node *right;
}t_tree_node;

static t_tree_node *create_tree_node(int data)
{
    t_tree_node *tree;

    tree = (t_tree_node *)malloc(sizeof(t_tree_node*));
    if (!tree)
        return (NULL);
    tree->data = data;
    tree->left = NULL;
    tree->right = NULL;
    return (tree);
}

/**
 * @note duplicate are ignored
 */
static t_tree_node *insert_node(t_tree_node *root, int data)
{
    if (root == NULL)
        return (create_tree_node(data));
    if (data < root->data)
        root->left = insert_node(root->left, data);
    if (data > root->data)
        root->right = insert_node(root->right, data);
    return (root);   
}

static void inorder_traversal(t_tree_node *root, int *res, int *index)
{
    if (root == NULL)
        return ;
    inorder_traversal(root->left, res, index);
    res[(*index)++] = root->data;
    inorder_traversal(root->right, res, index);
}

static void free_tree(t_tree_node *root)
{
    if (root == NULL)
        return ;
    free_tree(root->left);
    free_tree(root->right);
    free(root);    
}

void tree_sort(int *arr, int size)
{
    t_tree_node *root = NULL;
    int index = 0;
    int i;

    i = -1;
    while (++i < size)
        root = insert_node(root, arr[i]);
    inorder_traversal(root, arr, &index);
    free_tree(root);
}

void print_array(int *arr, int size, const char *label)
{
    int i;
    
    printf("%s: ", label);
    for (i = 0; i < size; i++)
    {
        printf("%d", arr[i]);
        if (i < size - 1)
            printf(", ");
    }
    printf("\n");
}

int main(void)
{
    // Test case 1: Random array
    int arr1[] = {64, 34, 25, 12, 22, 11, 90};
    int size1 = sizeof(arr1) / sizeof(arr1[0]);
    
    printf("=== Tree Sort Test ===\n");
    print_array(arr1, size1, "Before sorting");
    tree_sort(arr1, size1);
    print_array(arr1, size1, "After sorting");
    
    // Test case 2: Already sorted
    int arr2[] = {1, 2, 3, 4, 5};
    int size2 = sizeof(arr2) / sizeof(arr2[0]);
    
    printf("\n=== Already Sorted Test ===\n");
    print_array(arr2, size2, "Before sorting");
    tree_sort(arr2, size2);
    print_array(arr2, size2, "After sorting");
    
    // Test case 3: Reverse sorted
    int arr3[] = {5, 4, 3, 2, 1};
    int size3 = sizeof(arr3) / sizeof(arr3[0]);
    
    printf("\n=== Reverse Sorted Test ===\n");
    print_array(arr3, size3, "Before sorting");
    tree_sort(arr3, size3);
    print_array(arr3, size3, "After sorting");
    
    // Test case 4: With duplicates
    int arr4[] = {3, 1, 4, 1, 5, 9, 2, 6, 5};
    int size4 = sizeof(arr4) / sizeof(arr4[0]);
    
    printf("\n=== With Duplicates Test ===\n");
    print_array(arr4, size4, "Before sorting");
    tree_sort(arr4, size4);
    print_array(arr4, size4, "After sorting");
    
    return (0);
}