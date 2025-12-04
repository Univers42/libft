CXX := c++
CXXFLAGS ?= -Wall -Wextra -Werror
INCLUDES := -I../include -I../include/internal

# SRCS is filled by envsubst from test_filepath.cpp main.cpp
SRCS := test_filepath.cpp main.cpp
OBJDIR := obj
OBJS := $(patsubst %.cpp,$(OBJDIR)/%.o,$(patsubst %.c,$(OBJDIR)/%.o,$(SRCS)))
NAME := unit_test

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^

# compile C++ sources into object files under $(OBJDIR)/
$(OBJDIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c -o $@ $<

# compile C sources into object files under $(OBJDIR)/
$(OBJDIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c -o $@ $<

clean:
	rm -rf $(OBJDIR)

fclean:
	rm -f $(NAME)
	rm -rf $(OBJDIR)

re: fclean all

.PHONY: all clean fclean re