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

int main(int ac, char **av, char **env)
{
	char *input;
	t_token *tokens;

	input = NULL;
	if (ac != 1 || !av)
		return (ft_printf_fd(2, ERROR_ARGS));
	rl_initialize();
	using_history();
	ft_handle_signals();
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
		tokens = lexical(input);
		print_tokens(tokens);
		add_history(input);
		// execute(tokens, env);
		free_tokens(tokens);
	}
	if (input)
		free(input);
	rl_clear_history();
	ft_printf_fd(1, "Bye!\n");
	return 0;
}
/* 
void execute(t_token *tokens, char **env)
{
	t_token *current;
	int i;

	current = tokens;
	i = 0;
	while (current != NULL)
	{
		if (current->type == WORD)
		{
			if (i == 0)
			{
				if (ft_strncmp(current->data, "echo", ft_strlen(current->data)) == 0)
					ft_echo(current->next);
				else if (ft_strncmp(current->data, "cd", ft_strlen(current->data)) == 0)
					ft_cd(current->next);
				else if (ft_strncmp(current->data, "pwd", ft_strlen(current->data)) == 0)
					ft_pwd();
				else if (ft_strncmp(current->data, "export", ft_strlen(current->data)) == 0)
					ft_export(current->next, env);
				else if (ft_strncmp(current->data, "unset", ft_strlen(current->data)) == 0)
					ft_unset(current->next, env);
				else if (ft_strncmp(current->data, "env", ft_strlen(current->data)) == 0)
					ft_env(env);
				else if (ft_strncmp(current->data, "exit", ft_strlen(current->data)) == 0)
					ft_exit(current->next);
				else
					ft_exec(current->data, current->next, env);
			}
			else
				ft_exec(current->data, current->next, env);
			i++;
		}
		current = current->next;
	}
}

void ft_exit(t_token *tokens)
{
	int i;

	i = 0;
	while (tokens != NULL)
	{
		if (tokens->type == WORD)
		{
			if (i == 0)
			{
				if (ft_isdigit(tokens->data[0]))
					exit(ft_atoi(tokens->data));
				else
					exit(255);
			}
			else
				exit(255);
			i++;
		}
		tokens = tokens->next;
	}
	exit(0);
}

void ft_env(char **env)
{
	int i;

	i = 0;
	while (env[i] != NULL)
	{
		ft_printf_fd(1, "%s\n", env[i]);
		i++;
	}
}

void ft_echo(t_token *tokens)
{
	int i;

	i = 0;
	while (tokens != NULL)
	{
		if (tokens->type == WORD)
		{
			if (i == 0)
				ft_printf_fd(1, "%s", tokens->data);
			else
				ft_printf_fd(1, " %s", tokens->data);
			i++;
		}
		tokens = tokens->next;
	}
	ft_printf_fd(1, "\n");
} */