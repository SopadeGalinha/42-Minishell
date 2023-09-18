#include "../includes/minishell.h"

void sig_handler(int signum)
{
	rl_replace_line("", 0);
	printf("\n");
	rl_on_new_line();
	rl_redisplay();
}

void ft_handle_signals(void)
{
	struct sigaction sig_actions;

	sig_actions.sa_handler = sig_handler;
	sigemptyset(&sig_actions.sa_mask);
	sig_actions.sa_flags = 0;
	sigaction(SIGINT,  &sig_actions, NULL);
	sigaction(SIGQUIT, &sig_actions, NULL);
}

int main(int ac, char **av, char **env)
{
	char	*input;
	
	rl_initialize();
	ft_handle_signals();
	while (1)
	{
		input = readline(M I N I S H E L L Z);
	}
	return 0;
}
