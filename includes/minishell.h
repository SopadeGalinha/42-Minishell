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
# include <dirent.h>
# include <stdarg.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <sys/types.h>
# include "./libft/libft.h"
# include <readline/history.h>
# include <readline/readline.h>

extern int	g_exit_status;

//---------------------------------END HEADERS--------------------------------//

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
# define MS_ERR			"\001\033[1;31m\002Minishell Error: "
# define TRY			"\001\033[1;96m\002Try: ./minishell\n"
# define ERROR_ARGS		"\001\033[1;31m\002Invalid arguments\n"
# define UNSUP_MCMDS	"\001\033[1;31m\002Unsupported multiple commands\n"
# define UNCLOSED_QT	"\001\033[1;31m\002Unclosed quote\n"
# define SYNTAX			"syntax error near unexpected token"

// BEAUTIFUL PROMPT
# define MI	"\001\033[1;31m\002mi\001\033[0m\002"
# define NI	"\001\033[1;97m\002ni\001\033[0m\002"
# define S	"\001\033[1;35m\002s\001\033[0m\002"
# define H	"\001\033[1;33m\002h\001\033[0m\002"
# define EL	"\001\033[1;94m\002el\001\033[0m\002"
# define L	"\001\033[1;33m\002l\001\033[0m\002"
# define D	"\001\033[1;39m\002$> \001\033[0m\002"
/*__________________________________STRUCTS___________________________________*/

enum e_TokenType {
	ENV,
	WORD,
	PIPELINE,
	REDIR_OUT,
	APPEND,
	REDIR_ERR,
	REDIR_IN,
	HEREDOC,
	CMD,
	AND,
	OR,
	SEMICOLON,
	EXIT_STATUS,
	DEFAULT,
};

enum e_QuoteType {
	NONE,
	SINGLE	= '\'',
	DOUBLE	= '\"',
	IN = 0,
	OUT,
	PERMISSIONS = 0666,
};

enum e_ErrorType {
	NO_ERROR,
	UNCLOSED_QUOTE,
	BACKGROUND_NOT_SUPPORTED,
	D_PIPELINE_NOT_SUPPORTED,
	SEMICOLON_NOT_SUPPORTED,
	INVALID_COMMAND,
};

enum e_IndexesType {
	QUOTE,
	ERROR,
	INDEX = 0,
	START,
	QT,
	QT_TYPE,
	ERR_TYPE,
	MAIN = 0,
	WRITE_END,
	READ_END = 0,
	CHILD,
	PARENT,
};

typedef struct s_redir
{
	int				type;
	char			*file;
	struct s_redir	*next;

}				t_redir;

typedef struct s_pipes
{
	char			**cmds;
	int				type;
	pid_t			pid;
	int				redir_fd[2];
	struct s_redir	*redir_in;
	struct s_redir	*redir_out;
	struct s_pipes	*next;
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
	char			*input;
	t_pipes			*pipes;
	t_token			*tokens;
	t_env			*env;
	t_env			*exp;
	char			*heredoc;
	int				error;
	int				**pipes_fd;
	void			(*builtin[7])(struct s_shell *shell, t_pipes *pipes);
}				t_shell;

//------------------------------ END MACROS ----------------------------------//

/*_______________________________ FUNCTIONS __________________________________*/

//---------------------------- ENVIRONMENT PART ------------------------------//

char	*ft_strdup_equal_value(const char *src);
char	*ft_strdup_equal_key(const char *src);
int		create_add_node_to_list(t_env **head, char *line);
void	print_list(t_shell *shell, int flag, t_pipes *pipes);
t_env	*init_env(char **envp);

void	insert_sorted(t_env **export_list, t_env *env);
t_env	*create_node(char *key, char *value, char *line);
t_env	*init_export(t_env *env);

//UPDATE_LISTS
int		create_add_node_to_back(t_env **head, char *line);
int		find_node(t_env *lst, char *key);
void	update_node(t_env *lst, char *key, char *line);
int		create_find_add_insert_node(t_env **head, char *line);
void	update_lists(t_shell *shell, char *line, int flag);
void	update_exp(t_shell *shell, char *line);

//MAIN
void	signals_main(void);
void	signals_child(void);
void	signals_wait(void);
int		execute(t_shell *shell);

//---------------------------- PARSER PART -----------------------------------//

//PARSER
bool	parse_input(t_shell *shell);
bool	lexical_analyzer(char *input, t_token **tokens);
bool	process_tokens(t_shell *shell);
char	*expand_env(t_env *env, char *new_token);
bool	create_pipeline_node(t_shell *shell);
void	redirects(t_token **data, t_redir **r_in, t_redir **r_out);
bool	lexical_aux(char *input, t_token **tokens, int *si, char *data);
bool	is_special_char(char c);
void	addtoken(t_token **tokens, char *data, int *quo_err);
bool	process_redirections(t_shell *shell);
int		count_pipes(t_token *tokens);

//UTILS
bool	ft_isspace_str(char *str);
bool	ft_isdigit_str(char *str);
bool	input_is_valid(char *input);
bool	print_error(char *error, int exit_code);
void	init_shell(t_shell *shell, char **env);
void	get_input(t_shell *shell);
char	*get_env_value(t_env *env, char *key);
void	exec_signal_handler(void);
int		ft_strcmp(const char *s1, const char *s2);

// FREE
bool	free_pipes(t_pipes **pipes);
void	free_struct(t_shell *shell, int running);

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

// EXECUTE_UTILS
void	ft_access(char **cmd, t_shell *shell);
char	**get_envp_array(t_shell *shell);
int		ft_is_builtin(const char *builtin[7], char *cmd);
void	init_builtin(const char *builtin[7]);
//--------------------------------END FUNCTIONS-------------------------------//

#endif
