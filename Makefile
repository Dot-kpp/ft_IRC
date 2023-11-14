

G = \033[0;32m
R = \033[0;31m
RT = \033[0m

NAME = ircserv
CC = c++
CFLAGS = -Wall -Wextra -Werror -std=c++98
OBJDIR = bin
SRCDIR = src
SRCS = $(shell find $(SRCDIR) -type f -name '*.cpp')
OBJS = $(addprefix $(OBJDIR)/, $(notdir $(SRCS:.cpp=.o)))

all: mk_bin $(NAME)

mk_bin:
	@mkdir -p $(OBJDIR)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | mk_bin
	@$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	@$(CC) $(OBJS) -o $(NAME)
	@echo "$(G)\n ║ MAKE COMPLETE ║$(RT)"

clean: rm_bin
	@echo "\n$(R)clean target called$(RT)"
	@rm -f $(OBJS)

fclean: clean rm_bin
	@echo "\n$(R)fclean target called$(RT)"
	@rm -f $(NAME)

re: fclean all

run: re
	@echo "\n$(G) ║ RUN PROGRAM ║$(RT)"
	@./$(NAME) $(ARGS)

leaks: 
	@leaks --atExit -- ./$(NAME)

rm_bin:
	@rm -rf $(OBJDIR)

.PHONY: all mk_bin clean fclean rm_bin re run