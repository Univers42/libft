NAME = lists_tester

SRC_DIR = ../../lists
SRC_FILES = create_linked_list.c ft_create_node.c deletehead.c deleteAtPosition.c sorted_merge.c \
            search_content_lst.c remove_duplicates.c kth_end_from_linked_list.c issorted.c \
            ft_lstsize.c ft_lstnew.c ft_lstmap.c ft_lstlast.c ft_lstiter.c ft_lstdelone.c \
            ft_lstclear.c ft_lstadd_front.c ft_lstadd_back.c

TEST_SRC = lists_tester.c

SRCS = $(addprefix $(SRC_DIR)/, $(SRC_FILES))

OBJS = $(SRCS:.c=.o) $(TEST_SRC:.c=.o)

CC = gcc
CFLAGS = -Wall -Wextra -Werror -I../../

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
