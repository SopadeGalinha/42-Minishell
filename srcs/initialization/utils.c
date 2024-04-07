/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhogonca <jhogonca@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/06 14:06:31 by jhogonca          #+#    #+#             */
/*   Updated: 2024/04/06 15:13:00 by jhogonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	create_find_add_insert_node(t_env **head, char *line)
{
	t_env	*new;

	new = (t_env *)malloc(sizeof(t_env));
	if (!new)
	{
		ft_printf_fd(2, "Failed to allocate memory for t_env\n");
		return (0);
	}
	new->key = u_ft_strdup_equal_key(line);
	new->value = u_ft_strdup_equal_value(line);
	new->line = ft_strdup(line);
	new->next = NULL;
	if (!*head)
	{
		*head = new;
		return (1);
	}
	u_insert_sorted(head, new);
	return (1);
}

static int	create_add_node_to_back(t_env **head, char *line)
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
	new->key = u_ft_strdup_equal_key(line);
	new->value = u_ft_strdup_equal_value(line);
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

static int	find_node(t_env *lst, char *key)
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

void	u_update_lists(t_shell *shell, char *line, int flag)
{
	char	*key;

	key = u_ft_strdup_equal_key(line);
	if (flag)
	{
		if (find_node(shell->env, key))
			u_update_node(shell->env, key, line);
		else
			create_add_node_to_back(&shell->env, line);
	}
	if (find_node(shell->exp, key))
		u_update_node(shell->exp, key, line);
	else
		create_find_add_insert_node(&shell->exp, line);
	free(key);
}

char	*u_ft_strdup_equal_value(const char *src)
{
	char	*cpy;
	size_t	i;
	size_t	j;
	size_t	k;

	i = 0;
	j = 0;
	while (src[i] != '=' && src[i])
		i++;
	if (src[i] == '\0')
		return (NULL);
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
