# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: liamcohen <liamcohen@student.42.fr>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/12/12 17:31:35 by licohen           #+#    #+#              #
#    Updated: 2025/03/05 07:10:12 by liamcohen        ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

#### VARIABLES #################################################################

CC = cc
CFLAGS = -Wall -Wextra -Werror
LDFLAGS = -lreadline -lncurses

RM = rm -f

LIBDIR = Libft
LIB = $(LIBDIR)/libft.a
INCLUDE = include
SRC_DIR = src
OBJ_DIR = obj

SRC_SUBDIRS = error exec init parsing builtins signals

OBJ_SUBDIRS = $(addprefix $(OBJ_DIR)/, $(SRC_SUBDIRS))

PARSING_SRCS = expand_args_1.c expand_args_2.c expand_args_3.c expand_args_4.c expand_args_5.c \
               heredoc_utils.c heredoc.c line_validation_1.c line_validation_2.c \
               line_validation_utils.c parsing_1.c parsing_2.c parsing_3.c \
               parsing_utils.c tokenization_utils.c tokenization.c

SIGNALS_SRCS = signals.c signals2.c signals_utils.c

ERROR_SRCS = error_message.c error_message_2.c

EXEC_SRCS = command_exec1.c command_exec2.c command_path.c environnement2.c \
            environnement1.c pipeline1.c pipeline2.c redirection1.c redirections2.c \
			 redirections3.c utils.c memory_management1.c memory_management2.c
            
BUILTINS_SRCS = builtins_utils.c builtins.c ft_cd.c ft_cd_2.c \
                ft_echo.c ft_env.c ft_exit.c ft_export.c ft_export_2.c \
                ft_pwd.c ft_unset.c

INIT_SRCS = init.c init2.c

MAIN_SRCS = main.c

SRCS = $(MAIN_SRCS:%.c=$(SRC_DIR)/%.c) \
       $(PARSING_SRCS:%.c=$(SRC_DIR)/parsing/%.c) \
       $(SIGNALS_SRCS:%.c=$(SRC_DIR)/signals/%.c) \
       $(ERROR_SRCS:%.c=$(SRC_DIR)/error/%.c) \
       $(EXEC_SRCS:%.c=$(SRC_DIR)/exec/%.c) \
       $(BUILTINS_SRCS:%.c=$(SRC_DIR)/builtins/%.c) \
       $(INIT_SRCS:%.c=$(SRC_DIR)/init/%.c)

OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
	
GREEN = \033[0;32m
BLUE = \033[0;34m
YELLOW = \033[0;33m
RESET = \033[0m


all: makedirs $(NAME)

makedirs: $(OBJ_DIR) $(OBJ_SUBDIRS)

$(OBJ_DIR) $(OBJ_SUBDIRS):
	@mkdir -p $@

$(NAME): $(OBJS) $(LIB)
	@echo "$(YELLOW)🔨 Linking objects...$(RESET)"
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(LIB) $(LDFLAGS)
	@echo "$(GREEN)✅ Build successful!$(RESET)"

$(LIB):
	@echo "$(YELLOW)Adding Libft$(RESET)"
	@$(MAKE) -C $(LIBDIR) > /dev/null 2>&1

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@echo "$(BLUE)Compiling $<...$(RESET)"
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c -I $(INCLUDE) $< -o $@

clean:
	@echo "$(YELLOW)🧹 Cleaning object files...$(RESET)"
	@$(RM) -r $(OBJ_DIR)
	@$(MAKE) -C $(LIBDIR) clean > /dev/null 2>&1
	@echo "$(GREEN)✓ Cleaned$(RESET)"

fclean: clean
	@echo "$(YELLOW)🗑️ Full cleanup...$(RESET)"
	@$(RM) $(NAME)
	@$(MAKE) -C $(LIBDIR) fclean > /dev/null 2>&1
	@if [ -d tests ]; then \
		$(MAKE) -C tests fclean > /dev/null 2>&1 || true; \
	fi
	@if [ -d tests/tests_parsing ]; then \
		$(MAKE) -C tests/tests_parsing fclean > /dev/null 2>&1 || true; \
	fi
	@echo "$(GREEN)✓ Fully cleaned$(RESET)"
	
re: fclean all

tmp:
	@mkdir -p $(SRC_DIR)/tmp

.PHONY: all clean fclean re test tests_parsing makedirs tmp

test:
	@echo "$(YELLOW)🧪 Running tests...$(RESET)"
	@make -C tests
	@echo "$(GREEN)✓ Tests completed$(RESET)"

tests_parsing:
	@echo "$(YELLOW)🧪 Running parsing tester...$(RESET)"
	@make -C tests/tests_parsing
	@echo "$(GREEN)✓Parsing tester ready$(RESET)"

