/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   destroy.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 17:07:05 by marvin            #+#    #+#             */
/*   Updated: 2025/12/20 17:07:05 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


extern "C" 
{
	#include "libft.h"
	#include <stdio.h>
	#include <string.h>
}

typedef struct Pair
{
    char *k;
    char *v;
} Pair;

typedef struct Node
{
    int  val;
    char *name;
} Node;

static void free_pair(void *p)
{
    Pair *pp = p;
    if (!pp)
        return;
    free(pp->k);
    free(pp->v);
    free(pp);
}

static void free_node(void *p)
{
    Node *n = p;
    if (!n)
        return;
    free(n->name);
    free(n);
}

typedef struct Address
{
	char	*street;
	char	*city;
	char	*country;
}	Address;

typedef struct Employee
{
	char	*name;
	Address	*addr;
	char	**tags;
	size_t tag_count;
}	Employee;

static void free_employee(void *p)
{
	Employee *e = p;
	if (!e)
		return ;
	free(e->name);
	if (e->addr)
	{
		free(e->addr->street);
		free(e->addr->city);
		free(e->addr->country);
		free(e->addr);
	}
	if (e->tags)
	{
		for (size_t i = 0; i < e->tag_count; ++i)
			free(e->tags[i]);
		free(e->tags);
	}
	free(e);
}

/**
 * free any expacts either
 * - raw buffer (elem_size = 0, free_elem = NULL)
 * - array of pointers (elem_size = sizeof(void *), free_elem = function to free each element)
 * so our per-element free fucntiosn (free_pair, free_node, free) must free exactly one elem
 * destroy simply iterates through all the elemetnts and calls that single-element free
 * for each thren frees the container.
 * !Disclaimer!: destroy does not set the original pointer to NULL after freeing.
 * 
 * When not to use:
 * - nested structures where elements contain other dynamically allocated memory
 *  - in such cases, ensure that the provided free_elem function handles all necessary cleanup
 * 
 * Example usage in main():
 */
int main(void)
{
    /* Raw buffer (no per-element cleanup) */
	char *buffer = malloc(100);
    if (!buffer)
    {
        perror("malloc");
        return 1;
    }
    strcpy(buffer, "raw buffer example");
    printf("buffer: %s\n", buffer);
    destroy(buffer, 1, 0, NULL);
	
	/* Array of C-strings (char **) */
	{
    	size_t str_count = 3;
    	char **arr = malloc(str_count * sizeof(char *));
    	if (!arr)
    	{
    	    perror("malloc");
    	    return 1;
    	}
    	arr[0] = strdup("one");
    	arr[1] = strdup("two");
    	arr[2] = strdup("three");
    	printf("arr[1]: %s\n", arr[1] ? arr[1] : "(null)");
    	destroy(arr, str_count, sizeof(void *), free);
	}
    
	/* Linked-list-like nodes but stored as array of pointers */
	{
    	size_t node_count = 2;
    	Node **nodes = malloc(node_count * sizeof(Node *));
    	if (!nodes)
    	{
    	    perror("malloc");
    	    return 1;
    	}
    	for (size_t i = 0; i < node_count; ++i)
    	{
    	    nodes[i] = malloc(sizeof(Node));
    	    if (nodes[i])
    	    {
    	        nodes[i]->val = (int)i;
    	        nodes[i]->name = strdup("node-name");
    	    }
    	}
    	printf("nodes[0].val: %d\n", nodes[0] ? nodes[0]->val : -1);
    	destroy(nodes, node_count, sizeof(void *), free_node);
	}

	/* Map: array of pointers to Pair structs (key/value are separately malloc'd) */
    {
    	size_t map_n = 2;
    	Pair **map = malloc(map_n * sizeof(Pair *));
    	if (!map)
    	{
    	    perror("malloc");
    	    return 1;
    	}
    	for (size_t i = 0; i < map_n; ++i)
    	{
    	    map[i] = malloc(sizeof(Pair));
    	    if (map[i])
    	    {
    	        map[i]->k = strdup("key");
    	        map[i]->v = strdup("value");
    	    }
    	}
    	printf("map[0].k: %s\n", (map[0] && map[0]->k) ? map[0]->k : "(null)");
    	destroy(map, map_n, sizeof(void *), free_pair);
	}
	/**
	 * Let's take example of an 
	 */
	{
		size_t n = 3;
		Employee **emps = malloc(n * sizeof(Employee *));

		if (!emps)
			return (1);
		for (size_t i = 0; i < n; i++)
		{
			emps[i] = malloc(sizeof(Employee));
			if (!emps[i])
			{
				destroy(emps, i, sizeof(void *), free_employee);
				return (1);
			}
			emps[i]->name = strdup("Dylan");
			emps[i]->addr = calloc(1, sizeof(Address)); // zero-initialize Address (all fields NULL)
			emps[i]->addr->street = strdup("123 Example St");
			emps[i]->addr->city   = strdup("Example City");
			emps[i]->tag_count = 2;
			emps[i]->tags = malloc(emps[i]->tag_count * sizeof(char *));
			emps[i]->tags[0] = strdup("engineer");
			emps[i]->tags[1] = strdup("remote");
		}
		printf("first employee: %s @ %s \n", emps[0]->name, emps[0]->addr->street);
		destroy(emps, n, sizeof(void *), free_employee);
		return (0);
	}
    return 0;
}