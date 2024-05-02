# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: crebelo- <crebelo-@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/03/24 22:19:10 by crebelo-          #+#    #+#              #
#    Updated: 2024/05/02 20:31:39 by crebelo-         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME := philo

SRC :=	actions.c \
		init_dinner.c \
		utils1.c \
		utils2.c \
		utils3.c \
		main.c

OBJ := $(SRC:.c=.o)

CC := cc

CFLAGS := -Wall -Wextra -Werror -pthread # -g -fsanitize=thread

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
