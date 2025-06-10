/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 17:36:07 by dlesieur          #+#    #+#             */
/*   Updated: 2025/06/10 19:15:31 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "ft_sort.h"


int get_left_child(int parent_index)
{
    return (parent_index * 2 + 1);
}

int get_right_child(int parent_index)
{
    return (parent_index * 2 + 2);
}

int is_valid_index(int index, int size)
{
    if (index >= 0 && index < size)
        return (1);
    return (0);
       
}

void print_tree_relationships(int *arr, int size)
{
    int i;
    int left;
    int right;

    i = 0;
    while (i < size)
    {
        printf("Index %d (%d): ", i, arr[i]);
        
        left = get_left_child(i);
        right = get_right_child(i);
        
        if (!is_valid_index(left, size))
        {
            printf("Left=none Right=none");  // No children
        }
        else
        {
            printf("Left=%d (%d) ", left, arr[left]);
            
            if (is_valid_index(right, size))
                printf("Right=%d (%d)", right, arr[right]);
            else
                printf("Right=none");
        }
        
        printf("\n");
        i++;  // Continue to next node instead of returning
    }
}

// Calculate tree height (number of levels)
int get_tree_height(int size)
{
	int height;
	int nodes_count;

	height = 0;
	nodes_count = 1;
	while (nodes_count <= size)
	{
		height++;
		nodes_count *= 2;
	}
	return (height);
}

// Print spaces for alignment
void print_spaces(int count)
{
	int i;

	i = 0;
	while (i < count)
	{
		printf(" ");
		i++;
	}
}

// Print nodes at a specific level
void print_level_nodes(int *arr, int size, int level, int max_height)
{
	int start_index;
	int end_index;
	int nodes_in_level;
	int base_spacing;
	int initial_spacing;
	int i;
	int current_index;

	start_index = (1 << level) - 1;  // 2^level - 1
	nodes_in_level = 1 << level;     // 2^level
	end_index = start_index + nodes_in_level - 1;
	
	// Calculate base spacing for bottom level
	base_spacing = 6;  // Minimum space between nodes
	
	// Calculate spacing for current level
	int level_spacing = base_spacing * (1 << (max_height - level - 1));
	initial_spacing = level_spacing / 2;
	
	// Initial spacing before first node
	print_spaces(initial_spacing);
	
	i = 0;
	current_index = start_index;
	while (i < nodes_in_level && current_index <= end_index)
	{
		if (current_index < size)
		{
			printf("%2d", arr[current_index]);
		}
		else
		{
			printf("  ");  // Empty space for missing nodes
		}
		
		if (i < nodes_in_level - 1)
			print_spaces(level_spacing);
		
		current_index++;
		i++;
	}
	printf("\n");
}

// Print the branches (/ and \) between levels
void print_branches(int level, int max_height)
{
	int nodes_in_level;
	int base_spacing;
	int level_spacing;
	int initial_spacing;
	int branch_gap;
	int i;

	if (level >= max_height - 1)
		return;

	nodes_in_level = 1 << level;
	base_spacing = 6;
	level_spacing = base_spacing * (1 << (max_height - level - 1));
	initial_spacing = level_spacing / 2;
	branch_gap = level_spacing / 4;
	
	// Position branches directly under parent nodes
	print_spaces(initial_spacing - branch_gap);
	
	i = 0;
	while (i < nodes_in_level)
	{
		printf("/");
		print_spaces(branch_gap * 1);
		printf("\\");
		
		if (i < nodes_in_level - 1)
			print_spaces(level_spacing - branch_gap / 2);
		i++;
	}
	printf("\n");
}

// Main function to print visual tree
void print_visual_tree(int *arr, int size)
{
	int height;
	int level;

	if (!arr || size <= 0)
		return;

	height = get_tree_height(size);
	printf("\nVisual Tree Representation:\n");
	
	level = 0;
	while (level < height)
	{
		print_level_nodes(arr, size, level, height);
		if (level < height - 1)
			print_branches(level, height);
		level++;
	}
	printf("\n");
}

int main(void)
{
    int arr[] = {50, 30, 70, 20, 40, 60, 80,1,65,1,6,5,54};
    int size = sizeof(arr)/ sizeof(arr[0]);
    int i;
    
    printf("Array: ");
    i = 0;
    while (i < size)
    {
        printf("%d ", arr[i]);
        i++;
    }
    printf("\nTree relationships:\n");
    print_tree_relationships(arr, size);
    print_visual_tree(arr, size);
    
    return (0);
}