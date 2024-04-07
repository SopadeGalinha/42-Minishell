# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jhogonca <jhogonca@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/04/08 01:54:21 by jhogonca          #+#    #+#              #
#    Updated: 2023/04/08 01:54:21 by jhogonca         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

.SILENT:

NAME	:= libft.a
		
SRCS := ft_strtrim.c ft_atoi.c ft_isprint.c ft_strlen.c\
		ft_strchr.c ft_strrchr.c ft_strncmp.c ft_strnstr.c\
		ft_isalpha.c ft_isdigit.c ft_isalnum.c ft_isascii.c\
		ft_strlcpy.c ft_strlcat.c ft_toupper.c ft_tolower.c\
		ft_bzero.c ft_memcpy.c ft_memmove.c\
		ft_putendl_fd.c ft_putnbr_fd.c ft_putchar_fd.c ft_putstr_fd.c\
		ft_itoa.c ft_split.c ft_strjoin.c ft_substr.c\
		ft_strmapi.c ft_striteri.c ft_calloc.c ft_strdup.c\
		ft_memchr.c ft_memcmp.c ft_memset.c\
		ft_lstnew.c ft_lstmap.c ft_lstlast.c ft_lstiter.c\
		ft_lstsize.c ft_lstclear.c ft_lstdelone.c\
		ft_lstadd_back.c ft_lstadd_front.c ft_atoi_base.c\
		get_next_line.c ft_printf.c ft_printf_fd.c ft_isspace.c\
		ft_free_2d_array.c ft_contains.c ft_str_replace.c \
		ft_count_words.c ft_strcmp.c

CC := cc
RM := /bin/rm -f
CFLAGS := -Wall -Wextra -Werror -I.

CL := ar -rc
RL := ranlib

NUM_SRCS := $(words $(SRCS))
COMPILE_COUNT := 0

OBJS := ${SRCS:.c=.o}

${OBJS}: %.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@
	@$(eval COMPILE_COUNT=$(shell echo $$(($(COMPILE_COUNT)+1))))
	@printf "\r[\033[32m%3d%%\033[0m] Compiling: $<" $$(($(COMPILE_COUNT) * 100 / $(NUM_SRCS)))

${NAME}: ${OBJS}
	@${CL} ${NAME} ${OBJS}
	@${RL} ${NAME}
	@printf "\r[\033[32m100%%\033[0m] \001\033[1;97m\002- Libft compiled successfully! \n \001\033[0m\002"

all: ${NAME}

clean:
	${RM} ${OBJS}

fclean: clean
	${RM} ${NAME}

re: fclean all

norm:
	@norminette -R CheckForbiddenSourceHeader *.c > /dev/null 2>&1
	@norminette -R CheckDefine *.h > /dev/null 2>&1
	@printf '\033[3;3;33mlibft OK[✔ ]\n\033[0m'

author: 
	echo 'jhogonca'

.PHONY: clean fclean norm all re author