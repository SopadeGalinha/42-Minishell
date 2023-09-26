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

t_env *create_env_node(char *line)
{
	t_env *new = (t_env *)malloc(sizeof(t_env));
	if (!new) {
		ft_printf_fd(1, "Failed to allocate memory for t_env");
		return(NULL);
	}
	new->key = ft_strdup_equal_key(line);
	new->value = ft_strdup_equal_value(line);
	new->line = line;
	new->next = NULL;
	return (new);
}

void add_env_node_to_list(t_env **head, char *line) {
	t_env *new_env_node = create_env_node(line);
	new_env_node->next = *head;
	*head = new_env_node;
}

t_env	*init_env(char **envp)
{
	int i = 0;
	t_env *env_list = NULL;

	while(envp[i])
		i++;
	while (--i  >=  0)
		add_env_node_to_list(&env_list, envp[i]);

	return (env_list);
}

void print_env_list(t_env *head)
{
	t_env *current = head;
	while (current != NULL)
	{
		ft_printf_fd(1, "%s\n", current->line);
		current = current->next;
	}
}

int main(int ac, char **av, char **envp)
{
	char *input;
	int		i = 0;
	int		j = 0;
	t_env	*env;

	input = NULL;
	if (ac != 1 || !av)
		return (ft_printf_fd(2, ERROR_ARGS));
	rl_initialize();
	using_history();
	ft_handle_signals();
	env = init_env(envp);
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
		if (ft_strncmp(input, "env", ft_strlen(input)) == 0)
			print_env_list(env);
		add_history(input);
	}
	if (input)
		free(input);
	rl_clear_history();
	ft_printf_fd(1, "Bye!\n");
	return 0;
}
