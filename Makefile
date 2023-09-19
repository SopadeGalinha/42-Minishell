# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jhogonca <jhogonca@student.42porto.com>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/09/14 03:02:21 by jhogonca          #+#    #+#              #
#    Updated: 2023/09/14 03:02:21 by jhogonca         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

.SILENT:
.ONE_SHELL:

NAME		= minishell

CC			= cc
RM			= /bin/rm -rf
FLAGS		= -g # -Wall -Wextra -Werror -fsanitize=address

SRCS_DIR	= srcs/
OBJS_DIR	= srcs/objs/
LIBFT_DIR	= includes/libft/
commit_msg	= "auto commit"

SRCS_LIST	= main.c

SRCS 		= $(addprefix $(SRCS_DIR), $(SRCS_LIST))
OBJS 		= $(addprefix $(OBJS_DIR), $(SRCS_LIST:.c=.o))
LIBFT		= $(addprefix $(LIBFT_DIR), libft.a)


all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(FLAGS) $(OBJS) -L$(LIBFT_DIR) -lft  -lreadline -o $(NAME)

$(OBJS_DIR)%.o: $(SRCS_DIR)%.c | $(OBJS_DIR)
	$(CC) $(FLAGS) -c $< -o $@

$(OBJS_DIR):
	mkdir $(OBJS_DIR)

$(LIBFT):
	@make -s -C $(LIBFT_DIR)

clean:
	$(RM) $(OBJS_DIR)
	@make -s clean -C $(LIBFT_DIR)

fclean: clean
	$(RM) $(NAME)
#@make -s fclean -C $(LIBFT_DIR)


git: fclean
ifdef M
	$(eval commit_msg = $(M))
endif
	$(RM) $(NAME)
	git add .
	git commit -m "$(commit_msg)"
	git push

re: fclean all

norm:
	printf '\033[1;3;34mRunning Norminette!\n\033[0m'
	make -s norm -C $(LIBFT_DIR)
	norminette -R CheckDefine includes/*.h > /dev/null 2>&1
	norminette -R CheckForbiddenSourceHeader $(SRCS_DIR) > /dev/null 2>&1
	printf '\033[3;3;33mminishell OK[✔ ]\n\033[0m'
	printf '\033[1;3;34mNorminette Done!\n\033[0m'
