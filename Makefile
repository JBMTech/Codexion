# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jabuleje <jabuleje@student.42madrid.c      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/06/15 10:40:56 by jabuleje          #+#    #+#              #
#    Updated: 2026/06/15 10:41:00 by jabuleje         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

GREEN	= \033[0;32m
YELLOW	= \033[0;33m
RED		= \033[0;31m
BLUE	= \033[0;34m
RESET	= \033[0m

NAME		= codexion

CC			= cc
CFLAGS		= -Wall -Wextra -Werror -pthread

SRCS_DIR	= src
OBJ_DIR		= obj

FILES		= codexion.c \
			  check_dongle.c \
			  check_scheduler.c \
			  coder.c \
			  init_mutex.c \
			  getter.c \
			  init_proyect.c \
			  setter.c \
			  init_thread.c \
			  monitoring.c \
			  logs.c \
			  management_dongle.c \
			  management_queue.c \
			  management_time.c \
			  init_parser.c \
			  queue_dfe.c \
			  destroy_exit.c

SRCS		= $(addprefix $(SRCS_DIR)/,$(FILES))
OBJS		= $(addprefix $(OBJ_DIR)/,$(FILES:.c=.o))

INCLUDE		= -Iinclude

all: $(NAME)

$(NAME): $(OBJS)
	@echo "$(GREEN)Linking $(NAME)...$(RESET)"
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	@echo "$(GREEN)✓ $(NAME) created$(RESET)"

$(OBJ_DIR)/%.o: $(SRCS_DIR)/%.c
	@mkdir -p $(dir $@)
	@echo "$(BLUE)Compiling $<$(RESET)"
	@$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

re:
	@echo "$(YELLOW)Rebuilding project...$(RESET)"
	@$(MAKE) fclean
	@$(MAKE) all

clean:
	@rm -rf $(OBJ_DIR)
	@echo "$(YELLOW)Object files removed$(RESET)"

fclean: clean
	@rm -f $(NAME)
	@echo "$(RED)Executable removed$(RESET)"


.PHONY: all re clean fclean
