#include "../includes/minishell.h"

void	free_tokens(t_token *tokens)
{
	t_token	*tmp;

	while (tokens != NULL)
	{
		tmp = tokens;
		tokens = tokens->next;
		free(tmp->data);
		free(tmp);
	}
}

void	get_cmd(t_token *tokens, char **env)
{
	t_token *current;
	int i;
	char *path;
	char **split_path;
	current = tokens;

	i = -1;
	while (env[++i] != NULL)
		if (ft_strncmp(env[i], "PATH", 4) == 0)
		{
			path = ft_substr(env[i], 5, ft_strlen(env[i]));
			split_path = ft_split(path, ':');
			free(path);
			break ;
		}
	while (current != NULL)
	{
		i = -1;
		while (split_path[++i])
		{
			path = ft_strjoin(split_path[i], "/");
			path = ft_strjoin(path, current->data);
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

int main(int ac, char **av, char **env)
{
	t_token *tokens;
	char 	*input;
	char	*path_env;

	input = NULL;
	if (ac != 1 || !av)
		return (ft_printf_fd(2, ERROR_ARGS));
	rl_initialize();
	using_history();
	ft_handle_signals();
	path_env = ft_getenv(env, "PATH");
	if (path_env == NULL)
		return (ft_printf_fd(2, "PATH not found\n"));
	while (true)
	{
		free(input);
		input = readline(MINISHELL);
		if (input == NULL)
			break;
		if (input[0] == '\0')
			continue ;
		if (ft_strncmp(input, "exit", ft_strlen(input)) == 0)
			break ;
		// parse_input(input, path_env);
		tokens = lexical(input);
		get_cmd(tokens, env);
		print_tokens(tokens);
		add_history(input);
		free_tokens(tokens);
	}
	if (input)
		free(input);
	rl_clear_history();
	ft_printf_fd(1, "Bye!\n");
	return 0;
}
