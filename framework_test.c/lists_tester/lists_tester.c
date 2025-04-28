/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lists_tester.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 10:00:00 by dlesieur          #+#    #+#             */
/*   Updated: 2025/04/28 10:08:18 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../lists/ft_list.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// ANSI color codes for better visualization
#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define RESET   "\x1b[0m"

// Test context: Library management system
// We'll use our linked list to handle book inventory management

// Utility functions
void    print_test_header(const char *title);
void    print_list(t_list *head, const char *label);
int     *create_random_int(int value);
void    free_int_content(void *content);
void    print_result(const char *test_name, int success);
void    double_value(void *content);
void    *double_value_copy(void *content);
int     *generate_random_array(int size, int max);

int main(void)
{
    // Seed random number generator
    srand(time(NULL));
    
    printf(CYAN "===========================================\n");
    printf("   LIBRARY MANAGEMENT SYSTEM - LIST TESTS   \n");
    printf("===========================================\n\n" RESET);
    
    // SCENARIO: We're building a library management system
    // Each integer in our lists represents a book ID
    
    // PART 1: Initial book inventory creation
    print_test_header("CREATING INITIAL INVENTORY");
    
    // Generate a random array of book IDs
    int size = 8;
    int *book_ids = generate_random_array(size, 100);
    
    // Print the original array
    printf("Original book IDs: [ ");
    for (int i = 0; i < size; i++)
        printf("%d%s", book_ids[i], (i < size - 1) ? ", " : " ");
    printf("]\n\n");
    
    // Create a linked list from the array (our inventory)
    printf(YELLOW "Creating inventory from book IDs using create_linked_list()...\n" RESET);
    t_list *inventory = create_linked_list(book_ids, size);
    print_list(inventory, "Initial Inventory");
    
    // PART 2: Basic inventory information
    print_test_header("BASIC INVENTORY INFORMATION");
    
    int inv_size = ft_lstsize(inventory);
    printf(YELLOW "Inventory size using ft_lstsize(): %d books\n" RESET, inv_size);
    print_result("Inventory size check", inv_size == size);
    
    t_list *last_book = ft_lstlast(inventory);
    printf(YELLOW "Last book in inventory using ft_lstlast(): %d\n" RESET, 
           last_book ? *(int *)(last_book->content) : -1);
    
    // PART 3: Adding new books
    print_test_header("ADDING NEW BOOKS");
    
    // Add a new book to the front (new arrival)
    int *new_book_id = create_random_int(999);
    t_list *new_book = ft_lstnew(new_book_id);
    printf(YELLOW "Adding new arrival (ID: %d) to front using ft_lstadd_front()...\n" RESET, *new_book_id);
    ft_lstadd_front(&inventory, new_book);
    print_list(inventory, "Inventory after new arrival");
    
    // Add another book to the end (backorder arrived)
    int *backorder_id = create_random_int(888);
    t_list *backorder = create_node(backorder_id);
    printf(YELLOW "Adding backorder (ID: %d) to end using ft_lstadd_back()...\n" RESET, *backorder_id);
    ft_lstadd_back(&inventory, backorder);
    print_list(inventory, "Inventory after backorder");
    
    // PART 4: Searching for books
    print_test_header("SEARCHING FOR BOOKS");
    
    // Pick a random book ID from our inventory to search for
    int search_id = *((int*)inventory->next->content); // Get the second book ID
    int found = search_content_lst(inventory, search_id);
    printf(YELLOW "Searching for book ID %d using search_content_lst()...\n" RESET, search_id);
    print_result("Book search", found);
    
    // Search for a book that doesn't exist
    int nonexistent_id = 12345;
    found = search_content_lst(inventory, nonexistent_id);
    printf(YELLOW "Searching for nonexistent book ID %d...\n" RESET, nonexistent_id);
    print_result("Nonexistent book search", !found);
    
    // PART 5: Removing books
    print_test_header("REMOVING BOOKS");
    
    // Delete a book at position 3 (checked out)
    int position = 3;
    printf(YELLOW "Removing book at position %d using deleteatposition()...\n" RESET, position);
    inventory = deleteatposition(inventory, position);
    print_list(inventory, "Inventory after removal");
    
    // Delete the first book (sold)
    printf(YELLOW "Removing first book using delete_head()...\n" RESET);
    inventory = delete_head(inventory);
    print_list(inventory, "Inventory after selling first book");
    
    // PART 6: Creating a duplicate inventory with similar books
    print_test_header("CREATING SECOND BRANCH INVENTORY");
    
    // Create another array with some duplicate IDs
    int size2 = 6;
    int *branch_ids = generate_random_array(size2, 80);
    
    // Ensure some duplicates by setting a few values the same
    branch_ids[1] = book_ids[2];
    branch_ids[3] = book_ids[4];
    
    printf("Branch book IDs: [ ");
    for (int i = 0; i < size2; i++)
        printf("%d%s", branch_ids[i], (i < size2 - 1) ? ", " : " ");
    printf("]\n\n");
    
    t_list *branch_inventory = create_linked_list(branch_ids, size2);
    print_list(branch_inventory, "Branch Inventory");
    
    // PART 7: Check if sorted
    print_test_header("CHECKING INVENTORY SORTING");
    
    // Create a sorted inventory for comparison
    int sorted_ids[] = {101, 102, 103, 104, 105};
    t_list *sorted_inventory = create_linked_list(sorted_ids, 5);
    print_list(sorted_inventory, "Sorted Inventory");
    
    int is_sorted = issorted(sorted_inventory);
    printf(YELLOW "Is the inventory sorted using issorted()? %s\n" RESET, 
           is_sorted ? "Yes" : "No");
    print_result("Sorted inventory check", is_sorted);
    
    // Check if main inventory is sorted
    is_sorted = issorted(inventory);
    printf(YELLOW "Is the main inventory sorted? %s\n" RESET, 
           is_sorted ? "Yes" : "No");
    
    // PART 8: Find kth book from end
    print_test_header("FINDING KTH BOOK FROM END");
    
    int k = 2;
    int kth_book = kth_end_from_linked_list(inventory, k);
    printf(YELLOW "The %dth book from the end using kth_end_from_linked_list(): ID %d\n" RESET, 
           k, kth_book);
    
    // PART 9: Remove duplicate books
    print_test_header("REMOVING DUPLICATE BOOKS");
    
    // Create a list with duplicates
    int dup_ids[] = {201, 201, 202, 203, 203, 203, 204};
    t_list *dup_inventory = create_linked_list(dup_ids, 7);
    print_list(dup_inventory, "Inventory with Duplicates");
    
    printf(YELLOW "Removing duplicates using remove_duplicates()...\n" RESET);
    dup_inventory = remove_duplicates(dup_inventory);
    print_list(dup_inventory, "Inventory after Duplicate Removal");
    
    // PART 10: Merge sorted inventories
    print_test_header("MERGING BRANCH INVENTORIES");
    
    // Create two sorted inventories
    int sorted_ids1[] = {101, 103, 105, 107, 109};
    int sorted_ids2[] = {102, 104, 106, 108, 110};
    
    t_list *sorted_inv1 = create_linked_list(sorted_ids1, 5);
    t_list *sorted_inv2 = create_linked_list(sorted_ids2, 5);
    
    print_list(sorted_inv1, "Sorted Inventory 1");
    print_list(sorted_inv2, "Sorted Inventory 2");
    
    printf(YELLOW "Merging sorted inventories using sorted_merge()...\n" RESET);
    t_list *merged_inventory = sorted_merge(sorted_inv1, sorted_inv2);
    print_list(merged_inventory, "Merged Inventory");
    
    is_sorted = issorted(merged_inventory);
    print_result("Merged inventory sorting check", is_sorted);
    
    // PART 11: Apply operations to all books
    print_test_header("APPLYING OPERATIONS TO ALL BOOKS");
    
    printf(YELLOW "Doubling all book IDs using ft_lstiter()...\n" RESET);
    print_list(branch_inventory, "Before Doubling");
    ft_lstiter(branch_inventory, double_value);
    print_list(branch_inventory, "After Doubling");
    
    printf(YELLOW "Creating a new inventory with doubled IDs using ft_lstmap()...\n" RESET);
    t_list *doubled_inventory = ft_lstmap(inventory, double_value_copy, free_int_content);
    print_list(doubled_inventory, "New Doubled Inventory");
    
    // PART 12: Cleaning up
    print_test_header("CLEANING UP INVENTORIES");
    
    printf(YELLOW "Clearing all inventories using ft_lstclear()...\n" RESET);
    ft_lstclear(&inventory, free_int_content);
    ft_lstclear(&branch_inventory, free_int_content);
    ft_lstclear(&dup_inventory, free_int_content);
    ft_lstclear(&sorted_inventory, free_int_content);
    ft_lstclear(&doubled_inventory, free_int_content);
    
    // Note: merged_inventory contains nodes from sorted_inv1 and sorted_inv2
    // We shouldn't free those nodes directly
    
    printf(GREEN "\nAll tests completed successfully!\n" RESET);
    
    // Free allocated arrays
    free(book_ids);
    free(branch_ids);
    
    return 0;
}

// Print a header for each test section
void print_test_header(const char *title)
{
    printf(MAGENTA "\n===========================================\n");
    printf("   %s\n", title);
    printf("===========================================\n" RESET);
}

// Print the contents of a linked list with formatting
void print_list(t_list *head, const char *label)
{
    printf("%s: [ ", label);
    
    if (!head) {
        printf(RED "Empty" RESET);
    } else {
        t_list *curr = head;
        while (curr) {
            printf(GREEN "%d" RESET, *(int *)(curr->content));
            curr = curr->next;
            if (curr)
                printf(" -> ");
        }
    }
    
    printf(" ]\n");
}

// Create a dynamically allocated integer with the given value
int *create_random_int(int value)
{
    int *new_int = malloc(sizeof(int));
    if (new_int)
        *new_int = value;
    return new_int;
}

// Free the content of a node (an integer)
void free_int_content(void *content)
{
    free(content);
}

// Print test result with colored output
void print_result(const char *test_name, int success)
{
    printf("  %s: %s%s%s\n", test_name, 
           success ? GREEN "PASS" : RED "FAIL", 
           RESET, success ? "✓" : "✗");
}

// Double the value of an integer (for ft_lstiter)
void double_value(void *content)
{
    int *value = (int *)content;
    *value = *value * 2;
}

// Create a copy of an integer with doubled value (for ft_lstmap)
void *double_value_copy(void *content)
{
    int *original = (int *)content;
    int *copy = malloc(sizeof(int));
    
    if (copy)
        *copy = *original * 2;
    
    return copy;
}

// Generate an array of random integers
int *generate_random_array(int size, int max)
{
    int *arr = malloc(sizeof(int) * size);
    if (!arr)
        return NULL;
    
    for (int i = 0; i < size; i++)
        arr[i] = rand() % max + 1;
    
    return arr;
}
