#include "../includes/minishell.h"

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
