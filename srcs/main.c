/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhogonca <jhogonca@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/28 17:28:01 by jhogonca          #+#    #+#             */
/*   Updated: 2023/11/01 08:43:27 by jhogonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	g_exit_status;

//linha 29: O nó a ser excluído é o primeiro da lista
//linha 34: O nó a ser excluído não é o primeiro
//linha 35: Libera a memória alocada para o nó excluído
void	delete_node(t_env **lst, char *key)
{
	t_env	*current;
	t_env	*prev;

	prev = NULL;
	current = *lst;
	while (current != NULL)
	{
		if (strncmp(current->key, key, strlen(current->key)) == 0)
		{
			if (prev == NULL)
				*lst = current->next;
			else
				prev->next = current->next;
			free(current->key);
			free(current->value);
			free(current->line);
			free(current);
			return ;
		}
		prev = current;
		current = current->next;
	}
}

void	ft_unset(t_shell *shell, char *key)
{
	delete_node(&shell->env, key);
	delete_node(&shell->exp, key);
}

int	main(int ac, char **av, char **envp)
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
				if (parse_input(&shell))
					execute(&shell);
		free_struct(&shell, 0);
	}
	free_struct(&shell, 1);
	rl_clear_history();
	ft_printf_fd(1, "Bye!\n");
	return (0);
}
