/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhogonca <jhogonca@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/06 15:30:41 by jhogonca          #+#    #+#             */
/*   Updated: 2024/04/06 15:45:14 by jhogonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	delete_node(t_env **lst, char *key)
{
	t_env	*current;
	t_env	*prev;

	prev = NULL;
	current = *lst;
	while (current != NULL)
	{
		if (ft_strcmp(current->key, key) == 0)
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

void	ft_unset(t_shell *shell, t_pipes *pipes)
{
	int	i;

	i = 0;
	while (pipes->cmds[++i])
	{
		delete_node(&shell->env, pipes->cmds[i]);
		delete_node(&shell->exp, pipes->cmds[i]);
	}
	g_exit_status = 0;
}
