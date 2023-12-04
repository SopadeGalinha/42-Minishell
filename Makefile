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

.ONE_SHELL:

NAME		= minishell

CC			= cc
RM			= /bin/rm -rf
FLAGS		= -g -Wall -Wextra -Werror #-fsanitize=address

LIBFT_A		= libft.a
SRCS_DIR	= srcs/
OBJS_DIR	= srcs/objs/
LIBFT_DIR	= includes/libft/
commit_msg	= "auto commit"

PARSER_LIST	=	parse_input.c lexical_analyzer.c lexer_aux.c \
				process_tokens_1.c create_pipeline.c redirects.c \
				process_redirections.c

SRCS_LIST	=	main.c utils.c ft_handle_signals.c env.c \
				export.c ft_frees.c execute.c updates_lists.c \
				validations.c builtins_1.c builtins_2.c \
				$(PARSER_LIST) ft_cd.c execute_utils.c \
				builtins_2_utils.c process_tokens_2.c \
				execute_utils_2.c

SRCS 		= $(addprefix $(SRCS_DIR), $(SRCS_LIST))
OBJS 		= $(addprefix $(OBJS_DIR), $(SRCS_LIST:.c=.o))
LIBFT		= $(addprefix $(LIBFT_DIR), $(LIBFT_A))

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
	$(RM) *.txt
	$(RM) .nfs*
	@make -s fclean -C $(LIBFT_DIR)

leak: re
	@valgrind -q --track-origins=yes --leak-check=full --show-leak-kinds=all --suppressions=readline_supression ./$(NAME)
log: re
	@valgrind --track-origins=yes --leak-check=full --show-leak-kinds=all --suppressions=readline_supression --log-file=log%p.txt ./$(NAME)

git: fclean
ifdef M
	$(eval commit_msg = $(M))
else
	$(eval commit_msg = auto commit)
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
