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
# include <stdarg.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <sys/types.h>
# include "./libft/libft.h"
# include <readline/history.h>
# include <readline/readline.h>
extern int g_exit_status;

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
	IN = 0,
	OUT,
	PERMISSIONS = 0666,
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
	INDEX = 0,
	START,
	QT,
	QT_TYPE,
	ERR_TYPE,
};

typedef struct s_redir
{
	int				type;
	char			*file;
	struct	s_redir	*next;

}				t_redir;

typedef struct s_pipes
{
	int							id;
	char						**cmds;
	char						*path;
	int							fd[2];
	struct	s_redir				*redir_in;
	struct	s_redir				*redir_out;
	struct s_pipes				*next;
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
	char	*input;
	t_pipes	*pipes;

	t_token	*tokens;
	t_env	*env;
	t_env	*exp;
	char	*oldpwd;
	int		error;
	int		std_in;
	int		std_out;
	void	(*builtin[7])(struct s_shell *shell, t_pipes *pipes);
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

// ERRORS
# define MS_ERR			BOLD_RED	"Minishell Error: "RESET
# define TRY			BOLD_CYAN	"Try: ./minishell\n"RESET
# define ERROR_ARGS		MS_ERR BOLD_RED	"Invalid arguments\n" TRY RESET
# define UNSUP_MCMDS	MS_ERR BOLD_RED	"Unsupported multiple commands\n" RESET
# define UNCLOSED_QT	MS_ERR BOLD_RED	"Unclosed quote\n" RESET
# define SYNTAX			MS_ERR "syntax error near unexpected token"

// BEAUTIFUL PROMPT
# define P	BOLD_BLUE	"mi\001\033[0m\002"
# define R	BOLD_GREY	"ni\001\033[0m\002"
# define O	BOLD_GREEN	"s\001\033[0m\002"
# define M	BOLD_ORANGE	"h\001\033[0m\002"
# define PP	BOLD_RED	"el\001\033[0m\002"
# define T	BOLD_WHITE	"l\001\033[0m\002"
# define MINISHELL	P R O M PP T BOLD_GREY"$> "RESET

# define IN 0
# define OUT 1
//------------------------------ END MACROS ----------------------------------//

/*_______________________________ FUNCTIONS __________________________________*/

//---------------------------- ENVIRONMENT PART ------------------------------//

char		*ft_strdup_equal_value(const char *src);
char		*ft_strdup_equal_key(const char *src);
int			create_add_node_to_list(t_env **head, char *line);
void 		print_list(t_shell *shell, int flag);
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


//MAIN
void	ft_handle_signals(void);
void	execute(t_shell *shell);

//---------------------------- PARSER PART -----------------------------------//

//PARSER
bool	parse_input(t_shell *shell);
bool	lexical_analyzer(char *input, t_token **tokens);
bool	process_tokens(t_shell *shell);
char	*expand_env(t_env *env, char *new_token);
bool	create_pipeline_node(t_shell *shell);
void	redirects(t_token **data, t_redir **r_in, t_redir **r_out, t_shell *sh);
bool	lexical_aux(char *input, t_token **tokens, int *si, char *data);
bool	is_special_char(char c);
void	addtoken(t_token **tokens, char *data, int *quo_err);
bool	process_pipeline(t_shell *shell);

//UTILS
bool	ft_isspace_str(char *str);
bool	ft_isdigit_str(char *str);
bool	input_is_valid(char *input);
bool	print_error(char *error, int exit_code);
void	init_shell(t_shell *shell, char **env);
void	get_input(t_shell *shell);
char	*get_env_value(t_env *env, char *key);

// FREE
bool	free_pipes(t_pipes **pipes);
void	free_struct(t_shell *shell, int	running);

// INUTILS
void	print_tokens(t_token *head);
void	print_pipes(t_pipes *pipes);
int		arg_checker(t_shell *shell, char *str);


// BUILTINS
void	ft_pwd(t_shell *shell, t_pipes *pipes);
void	ft_echo(t_shell *shell, t_pipes *pipes);
void	ft_cd(t_shell *shell, t_pipes *pipes);
void	ft_unset(t_shell *shell, t_pipes *pipes);
void	ft_export(t_shell *shell, t_pipes *pipes);
void	ft_env(t_shell *shell, t_pipes *pipes);
void	ft_exit(t_shell *shell, t_pipes *pipes);

//---------------------------------END FUNCTIONS---------------------------------//

void	execute_pipeline(t_shell *shell);

#endif
