/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhogonca <jhogonca@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/06 13:10:31 by jhogonca          #+#    #+#             */
/*   Updated: 2024/04/06 15:24:08 by jhogonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	create_add_node_to_list(t_env **head, char *line)
{
	t_env	*new;

	new = (t_env *)malloc(sizeof(t_env));
	if (!new)
	{
		ft_printf_fd(STDOUT_FILENO, "Failed to allocate memory for t_env");
		return (0);
	}
	new->key = u_ft_strdup_equal_key(line);
	new->value = u_ft_strdup_equal_value(line);
	new->line = ft_strdup(line);
	if (!head)
		new->next = NULL;
	else
		new->next = *head;
	*head = new;
	return (0);
}

t_env	*init_env(char **envp)
{
	int		i;
	t_env	*env_list;

	i = 0;
	env_list = NULL;
	while (envp[i])
		i++;
	while (--i >= 0)
		create_add_node_to_list(&env_list, envp[i]);
	return (env_list);
}
