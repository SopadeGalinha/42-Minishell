#include "../includes/minishell.h"

int exit_status;

void delete_node(t_env **lst, char *key)
{
	t_env *current = *lst;
	t_env *prev = NULL;

	while (current != NULL) {
		if (strncmp(current->key, key, strlen(current->key)) == 0) {
			if (prev == NULL) {
				// O nó a ser excluído é o primeiro da lista
				*lst = current->next;
			} else {
				// O nó a ser excluído não é o primeiro
				prev->next = current->next;
			}
			// Libera a memória alocada para o nó excluído
			free(current->key);
			free(current->value);
			free(current->line);
			free(current);
			return;
		}
		prev = current;
		current = current->next;
	}
}

void ft_unset(t_shell *shell, char *key)
{
	delete_node(&shell->env, key);
	delete_node(&shell->exp, key);
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
		get_input(&shell);
		if (shell.input == NULL)
			continue ;
		if (shell.input)
			if (shell.input[0] && input_is_valid(shell.input))
				if (parse_input(shell.path_env, &shell))
					execute(&shell);
		free_struct(&shell, 0);
	}
	free_struct(&shell, 1);
	rl_clear_history();
	ft_printf_fd(1, "Bye!\n");
	return (0);
}
