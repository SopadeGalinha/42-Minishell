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
# define STX			"syntax error near unexpected token"

// BEAUTIFUL PROMPT
# define MI	"\001\033[1;31m\002mi\001\033[0m\002"
# define NI	"\001\033[1;97m\002ni\001\033[0m\002"
# define S	"\001\033[1;35m\002s\001\033[0m\002"
# define H	"\001\033[1;92m\002h\001\033[0m\002"
# define EL	"\001\033[1;94m\002el\001\033[0m\002"
# define L	"\001\033[1;92m\002l\001\033[0m\002"
# define D	"\001\033[1;39m\002$> \001\033[0m\002"
/*__________________________________STRUCTS___________________________________*/

enum e_TokenType
{
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
	PARENTHESIS_OPEN,
	PARENTHESIS_CLOSE,
	EXIT_STATUS,
	DEFAULT,
};

enum e_QuoteType
{
	NONE,
	SINGLE	= '\'',
	DOUBLE	= '\"',
	IN = 0,
	OUT,
	PERMISSIONS = 0666,
};

enum e_ErrorType
{
	NO_ERROR,
	UNCLOSED_QUOTE,
	BACKGROUND_NOT_SUPPORTED,
	D_PIPELINE_NOT_SUPPORTED,
	SEMICOLON_NOT_SUPPORTED,
	PARENTHESIS_NOT_SUPPORTED,
	INVALID_COMMAND,
	CTRL_D = 0
};

enum e_IndexesType
{
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
	char			*heredoc;
	bool			do_heredoc;
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

// INITIALIZATION
t_env	*init_env(char **envp);
t_env	*init_export(t_env *env);
void	init_shell(t_shell *shell, char **env);
void	init_builtin(const char *builtin[7]);

// UTILS
void	u_print_list(t_shell *shell, int flag, t_pipes *pipes);
int		u_error_arg(const char *str);
char	*u_ft_strdup_equal_value(const char *src);
void	u_update_lists(t_shell *shell, char *line, int flag);
char	*u_ft_strdup_equal_key(const char *src);
void	u_update_node(t_env *lst, char *key, char *line);
void	u_insert_sorted(t_env **export_list, t_env *env);
char	*get_env_value(t_env *env, char *key);
bool	print_error(char *error, int exit_code);
int		count_pipes(t_token *tokens);

// BUILTINS
void	ft_cd(t_shell *shell, t_pipes *pipes);
void	ft_pwd(t_shell *shell, t_pipes *pipes);
void	ft_env(t_shell *shell, t_pipes *pipes);
void	ft_exit(t_shell *shell, t_pipes *pipes);
void	ft_echo(t_shell *shell, t_pipes *pipes);
void	ft_unset(t_shell *shell, t_pipes *pipes);
void	ft_export(t_shell *shell, t_pipes *pipes);

// SOURCES
void	free_struct(t_shell *shell, int running);
bool	get_input(t_shell *shell);

// PARSER
bool	parser(t_shell *shell);
int		cmds_data(char *input, int i, int start, t_token **tokens);
bool	lexical(char *input, t_token **tokens);
void	addtoken(t_token **tokens, char *data, int *quo_err);
bool	is_special_char(char c);
bool	process_tokens(t_shell *shell);
char	*expand_env(t_env *env, char *new_token);
bool	create_pipeline_node(t_shell *shell);
bool	process_redirections(t_shell *shell);
void	heredoc(char *target, t_pipes *current, t_shell *shell);
int		ft_is_builtin(const char *builtin[7], char *cmd);
void	ft_access(char **cmd, t_shell *shell);

// EXECUTE
int		execute(t_shell *shell);
int		ft_list_envsize(t_env *lst);
bool	create_pipes(t_shell *shell);
int		ft_error(char *str, int exit_code);
void	close_pipes(int **pipes, int process_num);

// REDIRECTIONS
void	get_redirections(int pos, int **pipes, t_pipes *pipes_lst, \
t_shell *shell);
void	close_redirections(t_pipes *pipes_lst, int \
process_num, int **pipes, int pos);

//--------------------------------END FUNCTIONS-------------------------------//

#endif
