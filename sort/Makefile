# If you change SRCS, always run 'make fclean' before 'make' or 'make test'

CC			= 		cc
CFLAGS		= 		-Wall -Wextra -Werror -pedantic -Wshadow -std=c99 -I../include
LDFLAGS		= 		-MMD -MP
NAME_LIB	= 		sort
NAME		= 		$(NAME_LIB).a
SRCS		=		3-way-merge_sort.c          chunk_sort_helpers.c   heap_sort.c       pancake_sort.c     tim_sort.c \
					3-way-merge_sort_helpers.c  coktail_shaker_sort.c  hybrid.c          pigeonhole_sort.c  tree.c \
                	comb_sort.c            hybrid2.c         radix_sort.c       tree_sort.c 	\
	                counting_sort.c        insertion_sort.c  selection_sort.c   utils.c \
					bitonic_sort.c              cycle_sort.c           intro_sort.c      shell_sort.c       utils2.c \
					bubble_sort.c               ft_quick_sort.c        ksort.c           sleep_sort.c \
					cartesian_sort.c            gnome_sort.c           merge_sort.c      stooge_sort.c \
					chunk_process.c             greedy_helpers.c       mini_push_swap.c  strand_sort.c \
					chunk_sort.c                greedy_sort.c          odd_even_sort.c   tag_sort.c

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