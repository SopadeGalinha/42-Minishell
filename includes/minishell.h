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
extern int exit_status;

//---------------------------------END HEADERS--------------------------------//

/*__________________________________STRUCTS___________________________________*/


enum TokenType {
	ENV,
	WORD,
	PIPELINE,
	REDIR_OUT,
	D_REDIR_OUT,
	REDIR_ERR,
	REDIR_IN,
	HEREDOC,
	CMD,
	AND,
	OR,
	SEMICOLON,
	EXIT_STATUS,
	OPTION,
};

enum QuoteType {
	NONE,
	SINGLE	= '\'',
	DOUBLE	= '\"',
};

enum errorType {
	NO_ERROR,
	UNCLOSED_QUOTE,
	BACKGROUND_NOT_SUPPORTED,
	D_PIPELINE_NOT_SUPPORTED,
	SEMICOLON_NOT_SUPPORTED,
	INVALID_COMMAND,
};

enum indexesType {
	QUOTE,
	ERROR,
};

typedef struct s_pipes
{
	char				**cmds;
	struct s_pipelines	*next;
}					t_pipes;

typedef struct s_env
{
	char			*key;
	char			*value;
	char			*line;
	struct s_env	*next;
}					t_env;

typedef struct s_token
{
	char			*data;
	int				type;
	int				quote;
	int				error;
	struct s_token	*next;
	struct s_token	*prev;
}					t_token;

typedef struct s_shell
{
	t_token	*tokens;
	t_token	*cmds;
	t_env	*env;
	t_env	*exp;
	t_pipes	*pipes;
	char	*path_env;
	char	*input;
	int		error;
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

/*__________________________________FUNCTIONS____________________________________*/

char		*ft_strdup_equal_value(const char *src);
char		*ft_strdup_equal_key(const char *src);
int			create_add_node_to_list(t_env **head, char *line);
void 		print_list(t_env *head, int flag);
t_env		*init_env(char **envp);

void		insert_sorted(t_env **export_list, t_env *env);
t_env		*create_node(char *key, char *value, char *line);
t_env		*init_export(t_env *env);


//UPDATE_LISTS
int 	create_add_node_to_back(t_env **head, char *line);
int		find_node(t_env *lst, char *key);
void	update_node(t_env *lst, char *key, char *line);
int 	create_find_add_insert_node(t_env **head, char *line);
void	update_lists(t_shell *shell, char *line, int flag);
void	update_exp(t_shell *shell, char *line);


//UNSET
void	ft_unset(t_shell *shell, char *key);
void	delete_node(t_env **lst, char *key);

//MAIN
void	ft_handle_signals(void);
void	execute(t_shell *shell);

//PARSER
bool	lexical(char *input, t_token **tokens);
char	*ft_getenv(char **env, char *var_name);
bool	parse_input(char *path_env, t_shell *shell);
char	*get_env_value(t_env *env, char *key);

//UTILS
int		define_token(const char *token);
void	print_tokens(t_token *head);
void	free_struct(t_shell *shell, int	running);
void	get_input(t_shell *shell);
void	init_shell(t_shell *shell, char **env);
bool	input_is_valid(char *input);
bool	print_error(char *error, int exit_code);

//---------------------------------END FUNCTIONS---------------------------------//

#endif
