G = \033[0;32m
R = \033[0;31m
RT = \033[0m

ARG1=6667
ARG2=password1

CC = c++
CFLAGS = -Wall -Wextra -Werror -std=c++98
OBJDIR = bin
SRCDIR = src
SRCS = $(shell find $(SRCDIR) -type f -name '*.cpp')
OBJS = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRCS))
NAME=ft_IRC
PDF=https://cdn.intra.42.fr/pdf/pdf/94731/en.subject.pdf
NOTION=https://zagpyfaq.notion.site/ft_IRC-6a989eba71844ddaaea3a91e7f1cf406?pvs=4

all: mk_bin $(NAME)

mk_bin:
	@mkdir -p $(OBJDIR)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	@$(CC) $(OBJS) -o $(NAME)
	@echo "$(G)\n ║ MAKE COMPLETE ║$(RT)"

weechat:
	@cd WeeChat_Docker && docker build -t my-weechat-image . && docker run -it --rm my-weechat-image

notion:
	$(shell open $(NOTION))

pdf:
	$(shell open $(PDF))

clean: rm_bin
	@echo "\n$(R)clean target called$(RT)"
	@rm -f $(OBJS)

fclean: clean rm_bin
	@echo "\n$(R)fclean target called$(RT)"
	@rm -f $(NAME)

re: fclean all

run: re
	@echo "\n$(G) ║ RUN PROGRAM ║$(RT)"
	@./$(NAME) $(ARG1) $(ARG2)

leaks: 
	@leaks --atExit -- ./$(NAME)

rm_bin:
	@rm -rf $(OBJDIR)
	
.PHONY: all clean fclean re run rm_bin leaks pdf