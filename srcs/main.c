#include "../includes/minishell.h"

void	free_tokens(t_token **tokens)
{
	t_token	*current;
	t_token	*tmp;

	current = *tokens;
	while (current != NULL)
	{
		tmp = current->next;
		free(current->data);
		free(current);
		current = tmp;
	}
	*tokens = NULL;
}

void	get_cmd(t_token **tokens, char *path_env)
{
	t_token	*current;
	int		i;
	char	*path;
	char	**split_path;
	current = *tokens;

	i = -1;
	split_path = ft_split(path_env, ':');
	while (current != NULL)
	{
		i = -1;
		while (split_path[++i])
		{
			path = ft_strjoin(ft_strjoin(split_path[i], "/"), current->data);
			if (access(path, F_OK) == 0)
				if (access(path, X_OK) == 0)
				{
					current->type = CMD;
					free(path);
					break ;
				}
			free(path);
		}
		current = current->next;
	}
}
char	*ft_getenv(char **env, char *var_name)
{
	int		i;
	char	*tmp;

	i = -1;
	while (env[++i] != NULL)
		if (ft_strncmp(env[i], var_name, ft_strlen(var_name)) == 0)
			return (ft_substr(env[i], ft_strlen(var_name) + 1, ft_strlen(env[i])));
	return (NULL);
}

bool	parse_input(char *input, char *path_env, t_token **tokens)
{
	bool error;
	t_token	*current;

	lexical(input, tokens);
	current = *tokens;
	while (current)
	{
		current->type = define_token(current->data);
		current = current->next;
	}
	get_cmd(tokens, path_env);
	print_tokens(*tokens);
	return (true);
}

void	init_shell(void)
{
	rl_initialize();
	using_history();
	ft_handle_signals();
}

int main(int ac, char **av, char **env)
{
	t_shell	shell;
	char	*path_env;

	init_shell();
	shell = (t_shell){0};
	if (ac != 1 || !av)
		return (ft_printf_fd(2, ERROR_ARGS));
	shell.path_env = ft_getenv(env, "PATH");
	if (shell.path_env == NULL)
		return (ft_printf_fd(2, "PATH not found\n"));
	while (true)
	{
		free(shell.input);
		shell.input = readline(MINISHELL);
		if (shell.input == NULL)
			break;
		if (shell.input[0] == '\0')
			continue ;
		if (ft_strncmp(shell.input, "exit", ft_strlen(shell.input)) == 0)
			break ;
		if (ft_strncmp(shell.input, EASTER, ft_strlen(shell.input)) == 0 && ft_strlen(shell.input) > 4)
		{
			while (true)
				ft_printf_fd(1,BOLD_RED EGG"-" EGG"\n");
		}
		add_history(shell.input);
		parse_input(shell.input, shell.path_env, &shell.tokens);
		free_tokens(&shell.tokens);
	}
	if (shell.input)
		free(shell.input);
	if (shell.path_env)
		free(shell.path_env);
	rl_clear_history();
	ft_printf_fd(1, "Bye!\n");
	return 0;
}
