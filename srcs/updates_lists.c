/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   updates_lists.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhogonca <jhogonca@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 14:21:24 by jhogonca          #+#    #+#             */
/*   Updated: 2023/11/01 14:21:53 by jhogonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	create_add_node_to_back(t_env **head, char *line)
{
	t_env	*new;
	t_env *temp;

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
		*head = new;  // Se a lista estiver vazia, o novo nó é o único nó na lista.
		return (1);
	}

	while (temp->next)
		temp = temp->next;
	temp->next = new;  // Adiciona o novo nó após o último nó existente.
	return (1);
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

	while (current != NULL)
	{
		if (ft_strncmp(current->key, key, ft_strlen(current->key)) == 0)
		{
			if (current->key)
				free(current->key);
			if (current->value)
				free(current->value);
			if (current->line)
				free(current->line);
			// Atualiza o valor do nó correspondente
			current->key = ft_strdup_equal_key(line);
			current->value = ft_strdup_equal_value(line);
			current->line = ft_strdup(line);
			return;
		}
		current = current->next;
	}
}

int create_find_add_insert_node(t_env **head, char *line)
{
	t_env *new;

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
