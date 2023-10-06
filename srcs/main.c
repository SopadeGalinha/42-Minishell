#include "../includes/minishell.h"

void	free_tokens(t_shell *shell)
{
	t_token	*current;
	t_token	*tmp;

	current = shell->tokens;
	while (current != NULL)
	{
		tmp = current->next;
		free(current->data);
		free(current);
		current = tmp;
	}
	shell->tokens = NULL;
}

void	init_shell(t_shell *shell, char **env)
{
	rl_initialize();
	using_history();
	ft_handle_signals();
	shell->env = init_env(env);
	shell->exp = init_export(shell->env);
	shell->path_env = ft_getenv(env, "PATH");
	//Correr a lista e quando encontrar a key = PATH voce vai salvar o env->value
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

int create_add_node_to_back(t_env **head, char *line)
{
    t_env *new = (t_env *)malloc(sizeof(t_env));
    t_env *temp = *head;
    if (!new) {
        ft_printf_fd(2, "Failed to allocate memory for t_env\n");
        return (0);
    }
    new->key = ft_strdup_equal_key(line);
    new->value = ft_strdup_equal_value(line);
    new->line = ft_strdup(line);
    new->next = NULL;

    if (!*head) {
        *head = new;  // Se a lista estiver vazia, o novo nó é o único nó na lista.
        return (1);
    }

    while (temp->next) {
        temp = temp->next;
    }
    temp->next = new;  // Adiciona o novo nó após o último nó existente.
    return (1);
}

int create_find_add_insert_node(t_env **head, char *line)
{
    t_env *new = (t_env *)malloc(sizeof(t_env));
    if (!new) {
        ft_printf_fd(2, "Failed to allocate memory for t_env\n");
        return (0);
	}
    new->key = ft_strdup_equal_key(line);
    new->value = ft_strdup_equal_value(line);
    new->line = ft_strdup(line);
    new->next = NULL;

	
    if (!*head) {
        *head = new;  // Se a lista estiver vazia, o novo nó é o único nó na lista.
        return (1);
    }
   	insert_sorted(head, new);
    return (1);
}


void	update_exp(t_shell *shell, char *line)
{
	create_find_add_insert_node(&shell->exp, line);
}
int	find_node(t_env *lst, char *key)
{
	t_env *current = lst;

    while (current != NULL) 
	{
        if (ft_strncmp(current->key, key, ft_strlen(current->key)) == 0) 
            return (1);
        current = current->next;
    }
    return (0);
}
void	update_node(t_env *lst, char *key, char *line)
{
	t_env *current = lst;

    while (current != NULL) {
        if (ft_strncmp(current->key, key, ft_strlen(current->key)) == 0)
		{
            // Atualiza o valor do nó correspondente
			current->key = ft_strdup_equal_key(line);
    		current->value = ft_strdup_equal_value(line);
    		current->line = ft_strdup(line);
            return;
        }
        current = current->next;
    }
}

void	update_lists(t_shell *shell, char *line)
{
	char	*key = ft_strdup_equal_key(line);
	if (find_node(shell->env, key))
		update_node(shell->env, key, line);
	else
		create_add_node_to_back(&shell->env, line);
	
	if (find_node(shell->exp, key))
		update_node(shell->exp, key, line);
	else
		create_find_add_insert_node(&shell->exp, line);

	
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
		parse_input(shell.input, shell.path_env, &shell.tokens);
		if (ft_strncmp(shell.input, "exit", ft_strlen(shell.input)) == 0)
			break ;
		if ((ft_strncmp(shell.input, "env", ft_strlen(shell.input)) == 0) && shell.env)
			print_list(shell.env, 1);
		if ((ft_strncmp(shell.input, "export", ft_strlen(shell.input)) == 0))
			print_list(shell.exp, 0);
		if ((ft_strncmp(shell.tokens->data, "export", ft_strlen(shell.tokens->data)) == 0) && shell.tokens->next)
		{
			printf("%s\n", shell.tokens->next->data);
			if (ft_strchr(shell.tokens->next->data, '='))
			{
				update_lists(&shell, shell.tokens->next->data);
			}
			if (!ft_strchr(shell.tokens->next->data, '='))
			{	
				update_exp(&shell, shell.tokens->next->data);
			}
		}
		add_history(shell.input);
		free_tokens(&shell);
	}

	if (shell.input)
		free(shell.input);
	if (shell.path_env)
		free(shell.path_env);
	rl_clear_history();
	ft_printf_fd(1, "Bye!\n");
	return (0);
}
