#include "../includes/minishell.h"

void sig_handler(int signum)
{
	rl_replace_line("", 0);
	ft_printf_fd(0, "\n");
	rl_on_new_line();
	rl_redisplay();
}

void ft_handle_signals(void)
{
	struct sigaction sig_actions;

	signal(SIGQUIT, SIG_IGN);
	sig_actions.sa_handler = sig_handler;
	sigemptyset(&sig_actions.sa_mask);
	sig_actions.sa_flags = 0;
	sigaction(SIGINT, &sig_actions, NULL);
}

int main(int ac, char **av, char **envp)
{
	char *input;
	//int		i = 0;
	//int		j = 0;
	t_env	*env;
	t_env	*exp;

	input = NULL;
	if (ac != 1 || !av)
		return (ft_printf_fd(2, ERROR_ARGS));
	rl_initialize();
	using_history();
	ft_handle_signals();
	env = init_env(envp);
	exp = init_export(env);
	while (1)
	{
		free(input);
		input = readline(MINISHELL);
		if (input == NULL)
			break;
		if (input[0] == '\0')
			continue ;
		if (ft_strncmp(input, "exit", ft_strlen(input)) == 0)
			break ;
		if ((ft_strncmp(input, "env", ft_strlen(input)) == 0) && env)
			print_list(env, 1);
		if ((ft_strncmp(input, "export", ft_strlen(input)) == 0))
			print_list(exp, 0);
		add_history(input);
	}
	if (input)
		free(input);
	rl_clear_history();
	ft_printf_fd(1, "Bye!\n");
	return 0;
}
