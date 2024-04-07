/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhogonca <jhogonca@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/06 13:38:15 by jhogonca          #+#    #+#             */
/*   Updated: 2024/04/06 15:12:19 by jhogonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	u_insert_sorted(t_env **export_list, t_env *env)
{
	t_env	*current;

	current = *export_list;
	if (*export_list == NULL || ft_strncmp(env->key, \
		(*export_list)->key, ft_strlen(env->key)) < 0)
	{
		env->next = *export_list;
		*export_list = env;
	}
	else
	{
		current = *export_list;
		while (current->next != NULL
			&& ft_strncmp(env->key, \
			current->next->key, ft_strlen(env->key)) > 0)
			current = current->next;
		env->next = current->next;
		current->next = env;
	}
}

static t_env	*create_node(char *key, char *value, char *line)
{
	t_env	*new_node;

	new_node = (t_env *)malloc(sizeof(t_env));
	if (!new_node)
	{
		perror("Failed to allocate memory for t_env");
		exit(EXIT_FAILURE);
	}
	new_node->key = ft_strdup(key);
	new_node->value = ft_strdup(value);
	new_node->line = ft_strdup(line);
	new_node->next = NULL;
	return (new_node);
}

t_env	*init_export(t_env *env)
{
	t_env	*export_list;
	t_env	*current;
	t_env	*copy_node;

	export_list = NULL;
	current = env;
	while (current != NULL)
	{
		copy_node = create_node(current->key, current->value, current->line);
		u_insert_sorted(&export_list, copy_node);
		current = current->next;
	}
	return (export_list);
}
