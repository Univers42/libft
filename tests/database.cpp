/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   database.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 01:13:52 by marvin            #+#    #+#             */
/*   Updated: 2025/12/22 01:13:52 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/wait.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C"
{
#endif

#include "database.h"
#include "pipe.h"
#include "lifoba.h"

#ifdef __cplusplus
}
#endif

// ============= EXAMPLE USAGE =============

void create_sample_database(t_database *db) {
    // Add columns
    db_add_column(db, "ID", TYPE_INT, ALIGN_RIGHT);
    db_add_column(db, "First Name", TYPE_STRING, ALIGN_LEFT);
    db_add_column(db, "Last Name", TYPE_STRING, ALIGN_LEFT);
    db_add_column(db, "Age", TYPE_INT, ALIGN_RIGHT);
    db_add_column(db, "Salary", TYPE_FLOAT, ALIGN_RIGHT);
    db_add_column(db, "Department", TYPE_STRING, ALIGN_LEFT);
    
    // Add rows with labels
    const char *row1[] = {"1", "John", "Doe", "30", "75000.50", "Engineering"};
    db_add_row_with_label(db, "Employee 1", row1, 6);
    
    const char *row2[] = {"2", "Jane", "Smith", "25", "82000.00", "Marketing"};
    db_add_row_with_label(db, "Employee 2", row2, 6);
    
    const char *row3[] = {"3", "Bob", "Johnson", "40", "95000.75", "Sales"};
    db_add_row_with_label(db, "Employee 3", row3, 6);
    
    const char *row4[] = {"4", "Alice", "Williams", "35", "88000.25", "HR"};
    db_add_row_with_label(db, "Employee 4", row4, 6);
    
    const char *row5[] = {"5", "Charlie", "Brown", "28", "79000.00", "Engineering"};
    db_add_row_with_label(db, "Employee 5", row5, 6);
}

int main(int argc, char **argv) {
    t_database *db;
    t_display_config cfg;
    
    // Create database
    db = db_create("Employee Database");
    if (!db) return 1;
    
    // Check if file argument provided
    if (argc > 1) {
        printf("Loading data from file: %s\n", argv[1]);
        if (db_load_from_csv(db, argv[1]) == 0) {
            db->config.show_left_attributes = false;
            db_set_footer(db, "Data loaded from CSV file");
            db_render(db);
        }
    } else {
        // Generate sample data
        printf("No file provided, generating sample data...\n\n");
        
        create_sample_database(db);
        
        // Standard rendering
        db_set_footer(db, "Total Employees: 5");
        db_render(db);
        
        // Example with left attributes
        printf("\n\n=== WITH LEFT ATTRIBUTES ===\n");
        cfg = db->config;
        cfg.show_left_attributes = true;
        db_set_config(db, cfg);
        db_render(db);
        
        // Example with custom colors
        printf("\n\n=== WITH CUSTOM COLORS ===\n");
        t_format_style custom_style = db->style;
        custom_style.title = make_color(255, 100, 255);    // Magenta
        custom_style.header = make_color(100, 255, 100);   // Light green
        custom_style.even_row = make_color(255, 255, 200); // Light yellow
        custom_style.odd_row = make_color(200, 255, 255);  // Light cyan
        db_set_style(db, custom_style);
        db_render(db);
        
        // Example without alternating colors
        printf("\n\n=== WITHOUT ALTERNATING COLORS ===\n");
        cfg.alternating_colors = false;
        cfg.show_left_attributes = false;
        db_set_config(db, cfg);
        db_render(db);
    }
    
    // Cleanup
    db_free(db);
    
    return 0;
}