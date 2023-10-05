/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhogonca <jhogonca@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 02:54:11 by jhogonca          #+#    #+#             */
/*   Updated: 2023/09/14 02:54:11 by jhogonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/*__________________________________HEADERS___________________________________*/

# include <math.h>
# include <errno.h>
# include <signal.h>
# include <string.h>
# include <dirent.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <sys/types.h>
# include "./libft/libft.h"
# include <readline/history.h>
# include <readline/readline.h>

//---------------------------------END HEADERS--------------------------------//

/*__________________________________STRUCTS___________________________________*/

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}					t_env;

enum TokenType {
	WORD,
	ENV,
	PIPELINE,
	D_PIPES,
	REDIR_OUT,
	D_REDIR_OUT,
	REDIR_IN,
	HEREDOC,
	CMD,
	REDIR_ERR,
	EXIT_STATUS,
};

enum QuoteType {
	NONE,
	SINGLE,
	DOUBLE
};

typedef struct s_token
{
	char			*data;
	int				type;
	int				quote;
	struct s_token	*next;
}					t_token;

typedef struct s_shell
{
	t_token	*tokens;
	char	*path_env;
	char	*input;
}				t_shell;

/*__________________________________MACROS____________________________________*/

// COLORS
# define RESET			"\001\033[0m\002"
# define BOLD_RED		"\001\033[1;31m\002"
# define BOLD_ORANGE	"\001\033[1;33m\002"
# define BOLD_PURPLE	"\001\033[1;35m\002"
# define BOLD_GREY		"\001\033[1;39m\002" 
# define BOLD_GREEN		"\001\033[1;92m\002"
# define BOLD_BLUE		"\001\033[1;94m\002"
# define BOLD_CYAN		"\001\033[1;96m\002"
# define BOLD_WHITE		"\001\033[1;97m\002"
# define EASTER			"luiza"
# define EGG			"code destroyer found"

// ERRORS
# define TRY		BOLD_CYAN	" Try: ./minishell\n"RESET
# define ERROR_ARGS	BOLD_RED	"Invalid arguments\n" TRY RESET

// BEAUTIFUL PROMPT
# define P	BOLD_BLUE	"mi\001\033[0m\002"
# define R	BOLD_GREY	"ni\001\033[0m\002"
# define O	BOLD_GREEN	"s\001\033[0m\002"
# define M	BOLD_ORANGE	"h\001\033[0m\002"
# define PP	BOLD_RED	"el\001\033[0m\002"
# define T	BOLD_WHITE	"l\001\033[0m\002"
# define MINISHELL	P R O M PP T BOLD_GREY"$> "RESET
//---------------------------------END MACROS---------------------------------//


/*_________________________________FUNCTIONS__________________________________*/

//MAIN
void ft_handle_signals(void);

//PARSER
void	lexical(char *input, t_token **tokens);


//UTILS
int	define_token(const char *token);
void	print_tokens(t_token *head);

#endif