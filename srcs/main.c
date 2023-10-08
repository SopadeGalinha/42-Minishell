#include "../includes/minishell.h"

bool	is_builtin(char *cmd)
{
	if (ft_strncmp(cmd, "echo", ft_strlen("echo")) == 0)
		return (true);
	if (ft_strncmp(cmd, "cd", ft_strlen("cd")) == 0)
		return (true);
	if (ft_strncmp(cmd, "pwd", ft_strlen("pwd")) == 0)
		return (true);
	if (ft_strncmp(cmd, "export", ft_strlen("export")) == 0)
		return (true);
	if (ft_strncmp(cmd, "unset", ft_strlen("unset")) == 0)
		return (true);
	if (ft_strncmp(cmd, "env", ft_strlen("env")) == 0)
		return (true);
	if (ft_strncmp(cmd, "exit", ft_strlen("exit")) == 0
		|| ft_strncmp(cmd, "quit", ft_strlen("quit")) == 0)
		return (true);
	return (false);
}

void	ft_echo(t_token *tokens)
{
	while (tokens)
	{
		ft_printf_fd(1, "%s", tokens->data);
		if (tokens->next)
			ft_printf_fd(1, " ");
		tokens = tokens->next;
	}
	ft_printf_fd(1, "\n");
}

void	ft_cd(t_token *tokens, t_env *env)
{
	char	*path;

	if (tokens == NULL)
	{
		path = get_env_value(env, "HOME");
		if (path == NULL){
			ft_printf_fd(2, "cd: HOME not set\n");
			return ;
		}
	}
	else
		path = tokens->data;
	if (chdir(path) == -1)
		ft_printf_fd(2, "cd: %s: %s\n", path, strerror(errno));
}

void	ft_pwd(void)
{
	char	*path;

	path = getcwd(NULL, 0);
	ft_printf_fd(1, "%s\n", path);
	free(path);
}

static bool	ft_isdigit_str(char *str)
{
	int	i;

	i = -1;
	if (str[0] == '-' || str[0] == '+')
		i = 0;
	while (str[++i])
		if (str[i] < '0' || str[i] > '9')
			return (false);
	return (true);
}

void	execute_builtin(t_shell *shell)
{
	if (ft_strncmp(shell->tokens->data, "echo", ft_strlen("echo")) == 0)
		ft_echo(shell->tokens->next);
	else if (ft_strncmp(shell->tokens->data, "cd", ft_strlen("cd")) == 0)
		ft_cd(shell->tokens->next, shell->env);
	else if (ft_strncmp(shell->tokens->data, "pwd", ft_strlen("pwd")) == 0)
		ft_pwd();
	else if (ft_strncmp(shell->tokens->data, "export", ft_strlen("export")) == 0)
		print_list(shell->exp, 0);
	/* else if (ft_strncmp(shell->tokens->data, "unset", ft_strlen("unset")) == 0)
		ft_unset(shell->tokens->next, shell->env, shell->exp); */
	else if (ft_strncmp(shell->tokens->data, "env", ft_strlen("env")) == 0)
		print_list(shell->env, 1);
	/* else if (ft_strncmp(shell->tokens->data, "exit", ft_strlen("exit")) == 0
		|| ft_strncmp(shell->tokens->data, "quit", ft_strlen("quit")) == 0)
	{
	 	if (shell->tokens->next != NULL && ft_isdigit_str(shell->tokens->next->data))
			exit(ft_atoi(shell->tokens->next->data));
		exit(0); */
}

void	execute(t_shell *shell)
{
	t_shell	*sh;

	sh = shell;
	while (sh->tokens)
	{
		if (sh->tokens->type == CMD)
		{
			if (is_builtin(sh->tokens->data))
				execute_builtin(sh);
			else
				ft_printf_fd(2, "execute cmd\n");
				// execute_cmd(sh);
		}
		sh->tokens = sh->tokens->next;
	}
}

int main(int ac, char **av, char **envp)
{
	t_shell	shell;

	if (ac != 1 || !av)
		return (ft_printf_fd(2, ERROR_ARGS));
  	shell = (t_shell){0};
	init_shell(&shell, envp);
  	while (true)
	{
		if (!get_input(&shell))
			break;
		if (shell.input[0] == '\0')
			continue ;
		if (ft_strncmp(shell.input, "exit", ft_strlen(shell.input)) == 0)
			break ;
		add_history(shell.input);
		parse_input(shell.path_env, &shell);
		if (shell.error == NO_ERROR)
			execute(&shell);
		free_struct(&shell, 0);
	}
	rl_clear_history();
	ft_printf_fd(1, "Bye!\n");
	return (0);
}
