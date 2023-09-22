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

char	*ft_strdup_equal_value(const char *src)
{
	char	*cpy;
	size_t	i;
	size_t	j;
	size_t	k;

	i = 0;
	j = 0;
	while (src[i] != '=')
		i++;
	k = ++i;
	while (src[i++] != '\0')
		j++;
	cpy = (char *)malloc(sizeof(char) * (j + 1));
	if (!cpy)
		return (NULL);
	j = 0;
	while (src[k] != '\0')
		cpy[j++] = src[k++];
	cpy[j] = '\0';
	return (cpy);
}

char	*ft_strdup_equal_key(const char *src)
{
	char	*cpy;
	size_t	i;

	i = 0;
	while (src[i] != '=')
		i++;
	cpy = (char *)malloc(sizeof(char) * i + 1);
	if (!cpy)
		return (NULL);
	i = 0;
	while (*src != '=')
		cpy[i++] = *src++;
	cpy[i] = '\0';
	return (cpy);
}

char	**get_key(char **envp)
{
	int i = 0;

	while(envp[i])
		i++;
	char **key_array = malloc(sizeof(char *) * i + 1);
	i = 0;
	while (envp[i])
	{
		key_array[i] = ft_strdup_equal_key(envp[i]);
		i++;
	}
	key_array[i] = NULL;
	return (key_array);
}

char	**get_value(char **envp)
{
	int i = 0;

	while(envp[i])
		i++;
	char **value_array = malloc(sizeof(char *) * i + 1);
	i = 0;
	while (envp[i])
	{
		value_array[i] = ft_strdup_equal_value(envp[i]);
		i++;
	}
	value_array[i] = NULL;
	return (value_array);
}


char	**get_full(char **envp)
{
	int i = 0;

	while(envp[i])
		i++;
	char **full_array = malloc(sizeof(char *) * i + 1);
	i = 0;
	while (envp[i])
	{
		full_array[i] = ft_strdup(envp[i]);
		i++;
	}
	full_array[i] = NULL;
	return (full_array);
}

t_env_arr	*init_env(char **envp)
{
	int i = 0;
	char	**key_array;
	char	**value_array;
	char	**full_array;
	t_env_arr	*env;

	env = (t_env_arr *)malloc(sizeof(t_env_arr));
	env->key_array = get_key(envp);
	env->value_array = get_value(envp);
	env->full_array = get_full(envp);

	while(env->key_array[i])
		ft_printf_fd(1, "%s\n", env->key_array[i++]);
	i = 0;
	printf("--------------------------\n");
	while(env->value_array[i])
		ft_printf_fd(1, "%s\n", env->value_array[i++]);
	i = 0;
	printf("--------------------------\n");
	while(env->full_array[i])
		ft_printf_fd(1, "%s\n", env->full_array[i++]);
	return (env);
}

int main(int ac, char **av, char **envp)
{
	char *input;
	int		i = 0;
	int		j = 0;
	t_env_arr	*env_arr;

	input = NULL;
	if (ac != 1 || !av)
		return (ft_printf_fd(2, ERROR_ARGS));
	rl_initialize();
	using_history();
	ft_handle_signals();

	env_arr = init_env(envp);
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
		{
			while (envp[i])
				ft_printf_fd(1, "%s\n", envp[i++]);
			i = 0;
		}
		add_history(input);
	}
	if (input)
		free(input);
	rl_clear_history();
	ft_printf_fd(1, "Bye!\n");
	return 0;
}
