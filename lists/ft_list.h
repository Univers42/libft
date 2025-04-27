#ifndef FT_LIST
# define FT_LIST
#include <stddef.h>
typedef struct s_list
{
    void *content;
    struct s_list *next;
} t_list;

// Add the prototype for ft_lstnew
t_list *ft_lstnew(void *content);

#endif