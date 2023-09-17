#include "../includes/minishell.h"

void sigint_handler(int signum)
{
	rl_replace_line("", 0);
	printf("\n");
	rl_on_new_line();
	rl_redisplay();
}

void ft_handle_signals(void)
{
	struct sigaction sa_int;

	// Set up the SIGINT handler
	sa_int.sa_handler = sigint_handler;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = 0;
	sigaction(SIGINT,  &sa_int, NULL);
	sigaction(SIGQUIT, &sa_int, NULL);
}

int main(int ac, char **av, char **env)
{
	char	*input;
	printf("PID: %d\n", getpid());
	
	while (1)
	{
		rl_initialize();
		ft_handle_signals();
		input = readline(M I N I S H E L L Z);
	}
	return 0;
}