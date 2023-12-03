/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   updates_lists.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhogonca <jhogonca@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 14:21:24 by jhogonca          #+#    #+#             */
/*   Updated: 2023/12/03 17:20:51 by jhogonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	create_add_node_to_back(t_env **head, char *line, char *expand_value)
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
	if (expand_value)
	{
		new->value = ft_strdup(expand_value);
		new->line = ft_strjoin(line, expand_value);
	}
	else
	{
		new->value = ft_strdup_equal_value(line);
		new->line = ft_strdup(line);
	}
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

void	update_node(t_env *lst, char *key, char *line, char *expand_value)
{
	t_env	*current;

	current = lst;
	while (current != NULL)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			if (ft_strdup_equal_value(line))
			{
				printf("%s\n", expand_value);
				if (current->key)
					free(current->key);
				if (current->value)
					free(current->value);
				if (current->line)
					free(current->line);
				current->key = ft_strdup_equal_key(line);
				if (expand_value)
				{
					current->value = ft_strdup(expand_value);
					current->line = ft_strjoin(line, expand_value);
				}
				else
				{
					current->value = ft_strdup_equal_value(line);
					current->line = ft_strdup(line);
				}

			}
			return ;
		}
		
		current = current->next;
	} 	
}

int	create_find_add_insert_node(t_env **head, char *line, char *expand_value)
{
	t_env	*new;

	new = (t_env *)malloc(sizeof(t_env));
	if (!new)
	{
		ft_printf_fd(2, "Failed to allocate memory for t_env\n");
		return (0);
	}
	new->key = ft_strdup_equal_key(line);

	if (expand_value)
	{
		new->value = ft_strdup(expand_value);
		new->line = ft_strjoin(line, expand_value);
	}
	else
	{
		new->value = ft_strdup_equal_value(line);
		new->line = ft_strdup(line);
	}
	new->next = NULL;
	if (!*head)
	{
		*head = new;
		return (1);
	}
	insert_sorted(head, new);
	return (1);
}

void	update_lists(t_shell *shell, char *line, int flag, char *expand_value)
{
	char	*key;

	key = ft_strdup_equal_key(line);
	if (flag)
	{
		if (find_node(shell->env, key))
			update_node(shell->env, key, line, expand_value);
		else
			create_add_node_to_back(&shell->env, line, expand_value);
	}
	if (find_node(shell->exp, key))
		update_node(shell->exp, key, line, expand_value);
	else
		create_find_add_insert_node(&shell->exp, line, expand_value);
	free(key);
}
