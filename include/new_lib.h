/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_lib.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/13 19:40:42 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/14 01:13:32 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NEW_LIB_H
#define NEW_LIB_H
#include "ft_string.h"
#include "ft_memory.h"
#include "ft_string.h"
#include "ft_memory.h"
#include "ft_stdio.h"
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

/* Forward declare the readline type used by handlers so we don't pull
   heavy headers into this generic header. */
typedef struct s_readline t_readline;

/* Function-pointer type used for readline key handlers */
typedef void (*nl_hash_fn)(t_readline *);

/* Hash node storing function pointers (used by readline) */
typedef struct s_hash_list
{
    nl_hash_fn data;
    size_t hash_key;
    struct s_hash_list *next;
} t_hash_list;

void ft_putdar(char **arr);
void ft_putdar_fd(char **arr, int fd);

void ft_memdel(void **ap);
char **ft_darnew(size_t size);
size_t ft_darlen(char **dar);
void ft_free_dar(char **arr);
char **ft_darjoin(char **first, char **second);

char **ft_dardup(char **arr);
char *ft_dar2str(char **arr, char *symb);

void ft_dar_sort(char **res);
char **ft_dar_add(char **dest, char **src, char **exc);
int ft_check_exc(char *str, char **exc);

// dir
char **ft_dir_content(char *dirpath, short flag);
short ft_dir_flag(short flag, unsigned char d_type);
char get_type(mode_t mode);
char ft_file_type(char *path);
char *ft_read_file(char *path);
size_t ft_size_dir(char *dirpath);
void ft_write_to_file(char *path, int flags, int perm, char *content);

void ft_str_clear(char *s);
void ft_strdel(char **da);
int ft_strequ(char const *s1, char const *s2);
int ft_strnequ(char const *s1, char const *s2, size_t n);
char *ft_strnew(size_t size);

char *ft_del_sym_str(char *str, char *delsym);
size_t ft_get_cnt_words(const char *str, char *spr);

int ft_is_str_contain(char *str, char sym);
char *ft_last_word_pos(const char *str, char *seps);
int ft_match(char *s1, char *s2);
size_t ft_num_match(char **str, char *arg);
char *ft_stradd(char *buf, char *s, size_t i);
char *ft_strdel_el(char *buf, int i);
char *ft_strdup_realloc(char *str, short len);
void ft_putchar(const int c);
void ft_putstr(const char *str);

size_t djb2_hash_function(char *str);
/* Hash API now works with t_hash_list and function-pointer payloads */
t_hash_list **free_hash_table(t_hash_list **table, size_t len, char free_data);
nl_hash_fn get_hash_data(t_hash_list **table, char *hash_str, size_t size);
t_hash_list **init_hash_table(size_t size);
size_t hash_index(size_t key, size_t size);
t_hash_list *init_hash(void);
t_hash_list **push_hash(t_hash_list **table, char *hash_str, nl_hash_fn data, size_t size);
t_hash_list *set_hash(char *hash_str, nl_hash_fn data, size_t size);
char *ft_strcat(const char *src1, const char *src2);
char *ft_strncat(char *dest, const char *src, size_t n);

/* get_next_line — new prototype returning malloc'd line or NULL */
char *get_next_line(int fd);
void ft_putnbr(int nb);
#endif