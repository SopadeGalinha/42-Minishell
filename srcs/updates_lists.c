/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   updates_lists.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhogonca <jhogonca@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 14:21:24 by jhogonca          #+#    #+#             */
/*   Updated: 2023/12/03 21:38:31 by jhogonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	create_add_node_to_back(t_env **head, char *line)
{
	t_env	*new;
	t_env	*temp;

	new = (t_env *)malloc(sizeof(t_env));
	temp = *head;
	if (!new)
	{
		ft_printf_fd(2, "Failed to allocate memory for t_env\n");
		return (0);
	}
	new->key = ft_strdup_equal_key(line);
	new->value = ft_strdup_equal_value(line);
	new->line = ft_strdup(line);
	new->next = NULL;
	if (!*head)
	{
		*head = new;
		return (1);
	}
	while (temp->next)
		temp = temp->next;
	temp->next = new;
	return (1);
}

int	find_node(t_env *lst, char *key)
{
	t_env	*current;

	current = lst;
	while (current != NULL)
	{
		if (ft_strcmp(current->key, key) == 0)
			return (1);
		current = current->next;
	}
	return (0);
}

void	update_node(t_env *lst, char *key, char *line)
{
	t_env	*current;

	current = lst;
	while (current && ft_strcmp(current->key, key) != 0)
		current = current->next;
	update_node_values(current, line);
}

int	create_find_add_insert_node(t_env **head, char *line)
{
	t_env	*new;

	new = (t_env *)malloc(sizeof(t_env));
	if (!new)
	{
		ft_printf_fd(2, "Failed to allocate memory for t_env\n");
		return (0);
	}
	new->key = ft_strdup_equal_key(line);
	new->value = ft_strdup_equal_value(line);
	new->line = ft_strdup(line);
	new->next = NULL;
	if (!*head)
	{
		*head = new;
		return (1);
	}
	insert_sorted(head, new);
	return (1);
}

void	update_lists(t_shell *shell, char *line, int flag)
{
	char	*key;

	key = ft_strdup_equal_key(line);
	if (flag)
	{
		if (find_node(shell->env, key))
			update_node(shell->env, key, line);
		else
			create_add_node_to_back(&shell->env, line);
	}
	if (find_node(shell->exp, key))
		update_node(shell->exp, key, line);
	else
		create_find_add_insert_node(&shell->exp, line);
	free(key);
}
