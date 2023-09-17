#include "../includes/minishell.h"

void sigint_handler(int signum) {
	printf("Ctrl+C pressed\n");
}

void sigquit_handler(int signum) {
	printf("Ctrl+\\ pressed\n");
}

void ft_handle_signals(void)
{
	struct sigaction sa_int;
	struct sigaction sa_quit;

	// Set up the SIGINT handler
	sa_int.sa_handler = sigint_handler;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = 0;
	sigaction(SIGINT, &sa_int, NULL);

	// Set up the SIGQUIT handler
	sa_quit.sa_handler = sigquit_handler;
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = 0;
	sigaction(SIGQUIT, &sa_quit, NULL);
}

int main(int ac, char **av, char **env)
{
	char	*input;
	
	while (1)
	{
		ft_handle_signals();
		input = readline(M I N I S H E L L Z);
	}
	return 0;
}