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

void	init_shell(t_shell *shell, char **env)
{
	rl_initialize();
	using_history();
	ft_handle_signals();
	shell->env = init_env(env);
	shell->exp = init_export(shell->env);
	shell->path_env = ft_getenv(env, "PATH");
	if (shell->path_env == NULL)
		exit(ft_printf_fd(2, "PATH not found\n"));
}

bool	get_input(t_shell *shell)
{
	free(shell->input);
	shell->input = readline(MINISHELL);
	if (shell->input == NULL)
		return (false);
	return (true);
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
		if ((ft_strncmp(shell.input, "env", ft_strlen(shell.input)) == 0) && shell.env)
			print_list(shell.env, 1);
		if ((ft_strncmp(shell.input, "export", ft_strlen(shell.input)) == 0))
			print_list(shell.exp, 0);
		add_history(shell.input);
		parse_input(shell.path_env, &shell);
		free_tokens(&shell.tokens);
	}
	if (shell.input)
		free(shell.input);
	if (shell.path_env)
		free(shell.path_env);
	rl_clear_history();
	ft_printf_fd(1, "Bye!\n");
	return (0);
}
