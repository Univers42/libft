# If you change SRCS, always run 'make fclean' before 'make' or 'make test'

CC			= 		cc
CFLAGS		= 		-Wall -Wextra -Werror -pedantic -Wshadow -std=c99 -I../include -I./ft_printf/include
DEPFLAGS	= 		-MMD -MP
NAME_LIB	= 		dsa
NAME		= 		$(NAME_LIB).a		
SRCS		=  		$(addprefix circular_linked_list/, cl_access.c cl_create.c cl_destroy.c \
					cl_insert_remove.c cl_pop.c cl_print.c cl_push.c cl_search.c cl_utility.c)
SRCS		+=		$(addprefix doubly_linked_list/, dll_access.c dll_create.c dll_destroy.c \
					dll_insert_remove.c dll_pop_back.c dll_pop_front.c \
					dll_print.c dll_push_back.c dll_push_front.c dll_search.c dll_utility.c)
SRCS		+=		$(addprefix lists/, create_linked_list.c delete_at_position.c delete_head.c \
					create_node.c lstadd_back.c lstadd_front.c lstclear.c lstdelone.c \
					lstiter.c lstlast.c lstmap.c lstsize.c issorted.c kth_end_from_linked_list.c \
					remove_duplicates.c search_content_lst.c sorted_merge.c)
SRCS		+=		$(addprefix queue/, queue_clear.c queue_create.c queue_dequeue.c queue_destroy.c \
					queue_enqueue.c queue_front.c queue_is_empty.c queue_print.c queue_rear.c \
					queue_size.c)
SRCS		+=		$(addprefix vector/, vector_add_back.c vector_binary_search.c vector_capacity.c vector_clear.c \
					vector_create.c vector_destroy.c vector_get.c vector_insert.c vector_pop_back.c \
					vector_print.c vector_remove.c vector_reserve.c vector_resize.c vector_set.c vector_size.c)
OBJ_DIR		=		obj
TEST_DIR	=		unit_test

TEST_SRCS	=		$(TEST_DIR)/test.c
TEST_OBJS	=		$(addprefix $(OBJ_DIR)/, $(TEST_SRCS:%.c=%.o))
TEST_BIN	=		test_$(NAME_LIB)

OBJS		=		$(addprefix $(OBJ_DIR)/, $(SRCS:%.c=%.o))
DPDC		=		$(addprefix $(OBJ_DIR)/, $(SRCS:%.c=%.d))
RM			:=		rm -rf
AR			:=		ar rcs

all: $(NAME)

$(NAME):$(OBJS)
	$(AR) $@ $^

$(OBJ_DIR)/%.o : %.c | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(DEPFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/$(TEST_DIR):
	mkdir -p $(OBJ_DIR)/$(TEST_DIR)

# Build test object
$(OBJ_DIR)/$(TEST_DIR)/%.o : $(TEST_DIR)/%.c | $(OBJ_DIR)/$(TEST_DIR)
	$(CC) $(CFLAGS) -I. -c $< -o $@

# Test target
test: $(NAME) $(TEST_OBJS)
	$(CC) $(CFLAGS) $(TEST_OBJS) $(OBJS) -o $(TEST_BIN)
	@echo "Run ./$(TEST_BIN) to execute tests."

clean:
	$(RM) $(OBJ_DIR)

fclean: clean
	$(RM) $(NAME) $(TEST_BIN)

re: fclean all

.PHONY: all clean fclean re test

-include $(DPDC)