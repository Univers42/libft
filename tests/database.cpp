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
#include <ctype.h>

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

// file-scope mapper used by db_map_column
static char uppercase_mapper(unsigned int i, char c)
{
    (void)i;
    unsigned char uc = (unsigned char)c;
    if (uc >= 'a' && uc <= 'z')
        return (char)toupper(uc);
    return c;
}

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
        // enable auto-increment id mode for this demo
        {
            t_display_config cfg = db->config;
            cfg.auto_increment_id = false;
            t_format_style custom_style = db->style;
            custom_style.title = make_color(255, 100, 255);    // Magenta
            custom_style.header = make_color(100, 255, 100);   // Light green
            custom_style.even_row = make_color(255, 255, 200); // Light yellow
            custom_style.odd_row = make_color(200, 255, 255);  // Light cyan
            custom_style.footer = make_color(0,0,255);
            custom_style.number_align = ALIGN_RIGHT;
            custom_style.string_align = ALIGN_LEFT;
            db_set_style(db, custom_style);
            db_set_config(db, cfg);
        }
        if (db_load_from_csv(db, argv[1]) == 0) {
            db_add_column(db, "city", TYPE_INT, ALIGN_LEFT);
            /* populate city column with sample values (wrap if fewer entries than rows) */
            {
                const char *cities[] = {"Stockholm", "Paris", "Berlin", "London", "Malmö", "Unknown", "Dublin", "Oslo"};
                size_t ncities = sizeof(cities) / sizeof(cities[0]);
                t_col *city_col = &db->cols[db->ncols - 1];
                size_t ri;
                for (ri = 0; ri < db->nrows; ++ri)
                {
                    add_value(db, (int)ri, city_col, (char *)cities[ri % ncities]);
                }
            }
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

        // Demonstrate mapping a column to uppercase using ft_strmapi via db_map_column
        db_map_column(db, 1, uppercase_mapper);
        printf("\n=== AFTER MAPPING FIRST NAME TO UPPERCASE ===\n");
        db_render(db);
 
        // Modify a single cell in-place
        db_set_cell(db, 0, 2, "Doe-Smith");
        printf("\n=== AFTER SETTING ROW0 LAST NAME ===\n");
        db_render(db);

        // Use add_value_idx_column to change ID of row 2 to 42
        add_value_idx_column(db, &db->cols[0], 2, 42);
        printf("\n=== AFTER add_value_idx_column row2 ID=42 ===\n");
        db_render(db);

        // Check for existing ids
        bool exists = is_id_already_existing(db, "3", 0);
        printf("ID '3' exists? %s\n", exists ? "yes" : "no");
        exists = is_id_already_existing(db, "42", 0);
        printf("ID '42' exists? %s\n", exists ? "yes" : "no");
        
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

        // AUTO-INCREMENT ID DEMO
        printf("\n\n=== AUTO-INCREMENT ID DEMO ===\n");
        {
            t_database *db2 = db_create("AutoID Demo");
            if (db2)
            {
                t_display_config cfg2 = db2->config;
                cfg2.auto_increment_id = true;
                db_set_config(db2, cfg2);

                db_add_column(db2, "ID", TYPE_INT, ALIGN_RIGHT);
                db_add_column(db2, "Name", TYPE_STRING, ALIGN_LEFT);
                db_add_column(db2, "Role", TYPE_STRING, ALIGN_LEFT);

                const char *a1[] = {"", "Alice", "Engineer"};
                const char *a2[] = {"", "Bob", "Manager"};
                const char *a3[] = {"", "Carol", "Designer"};

                db_add_row_with_label(db2, "r1", a1, 3);
                db_add_row_with_label(db2, "r2", a2, 3);
                db_add_row_with_label(db2, "r3", a3, 3);

                db_set_footer(db2, "Auto-generated IDs");
                db_render(db2);

                printf("Generated IDs: %s, %s, %s\n",
                    db2->rows[0].data[0], db2->rows[1].data[0], db2->rows[2].data[0]);

                db_free(db2);
            }
        }
    }
    
    // Cleanup
    db_free(db);
    
    return 0;
}