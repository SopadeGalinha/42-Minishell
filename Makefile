.SILENT:

NAME		= minishell

# Compiler
CC			= cc
RM			= /bin/rm -rf
FLAGS		= #-g -Wall -Wextra -Werror -fsanitize=address

# Libft
LIBFT_A		= libft.a
SRCS_DIR	= srcs/
LIBFT_DIR	= includes/libft/
LIBFT		= $(addprefix $(LIBFT_DIR), $(LIBFT_A))

# Objects
OBJS_DIR	= objs/
OBJS_DIRS   = $(sort $(dir $(OBJS)))

# Directories containing source files
SRC_DIRS := srcs

# Find all .c files recursively within SRC_DIRS
SRCS := $(shell find $(SRC_DIRS) -name '*.c')

# Generate object file paths from source file paths
OBJS := $(patsubst $(SRCS_DIR)%.c,$(OBJS_DIR)%.o,$(SRCS))

# Compilation output
COMPILE_COUNT = 0
NUM_SRCS	= $(words $(SRCS))

all: $(NAME)

$(NAME): $(LIBFT) $(OBJS_DIRS) $(OBJS)
	@$(CC) $(FLAGS) $(OBJS) -L$(LIBFT_DIR) -lft -lreadline -o $(NAME)
	@printf "\r[\033[32m100%%\033[0m] \001\033[1;97m\002- 42-Minishell compiled successfully! \n \001\033[0m\002"
	@printf "\n\e[1;97;3m	Welcome to 42-Minishell! \n\e[0m"
	@printf "\n\e[1;97;3mA project developed by the skilled hands\n of Ruben Soares and Jhonata Gonçalves.\n\e[0m"
	@printf "\nRun \001\033[1;97m\002./minishell\001\033[0m\002 to start the program.\n"

$(OBJS_DIR)%.o: $(SRCS_DIR)%.c
	@mkdir -p $(@D)
	@$(CC) $(FLAGS) -c $< -o $@
	@$(eval COMPILE_COUNT=$(shell echo $$(($(COMPILE_COUNT)+1))))
	@printf "\r[\033[32m%3d%%\033[0m] Compiling: $<" $$(($(COMPILE_COUNT) * 100 / $(NUM_SRCS)))

$(LIBFT):
	@make re -s -C $(LIBFT_DIR)

clean:
	$(RM) $(OBJS_DIR)
	@make clean -s -C $(LIBFT_DIR)

fclean: clean
	$(RM) $(NAME)
	$(RM) *.txt
	$(RM) .nfs*
	@make -s fclean -C $(LIBFT_DIR)

leak: re
	@valgrind -q --track-origins=yes --leak-check=full --show-leak-kinds=all --suppressions=readline_supression ./$(NAME)
log: re
	@valgrind --track-origins=yes --leak-check=full --show-leak-kinds=all --suppressions=readline_supression --log-file=log%p.txt ./$(NAME)

re: fclean all

$(OBJS_DIRS):
	mkdir -p $(OBJS_DIRS)

.PHONY: all clean fclean re leak log
