NAME := philo

SRC :=	src/actions.c \
		src/init_dinner.c \
		src/utils1.c \
		src/utils2.c \
		src/main.c

OBJ := $(SRC:.c=.o)

CC := cc

CFLAGS := -Wall -Wextra -Werror -g #-pthread -fsanitize=thread

INCLUDES := -I.libft/includes

GREEN := \033[0;32m
CYAN := \033[0;36m
RED := \033[0;31m
NC := \033[0m

all:	$(OBJ) $(NAME)

$(OBJ): %.o: %.c
	@echo "$(CYAN)	Compiling $@...$(NC)"
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(INCLUDES) $(OBJ) -lreadline -o $(NAME)
	@echo "$(GREEN)\n###############################$(NC)"
	@echo "$(GREEN)	$(NAME) is ready!$(NC)"
	@echo "$(GREEN)###############################$(NC)"

clean:
	@echo "$(CYAN)\n# Cleaning object files...\n$(NC)"
	rm -f $(OBJ)
	@echo "$(GREEN)\n###########################################$(NC)"
	@echo "$(GREEN)	$(NAME) object files cleaned!$(NC)"
	@echo "$(GREEN)###########################################$(NC)"

fclean: clean
	@echo "$(CYAN)\n# Cleaning $(NAME)\n$(NC)"
	rm -f $(NAME)
	@echo "$(GREEN)\n########################################$(NC)"
	@echo "$(GREEN)	$(NAME) $(NC)"
	@echo "$(GREEN)########################################$(NC)"

re: fclean all

.PHONY: all clean fclean re
