/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhogonca <jhogonca@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/06 15:27:08 by jhogonca          #+#    #+#             */
/*   Updated: 2024/04/07 17:11:02 by jhogonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

bool	print_error(char *error, int exit_code)
{
	ft_printf_fd(2, "%s\n", error);
	g_exit_status = exit_code;
	return (false);
}

char	*get_env_value(t_env *env, char *key)
{
	t_env	*current;

	current = env;
	while (current)
	{
		if (ft_strncmp(current->key, key, ft_strlen(key)) == 0
			&& ft_strlen(current->key) == ft_strlen(key))
			return (ft_strdup(current->value));
		current = current->next;
	}
	return (ft_strdup(""));
}

int	count_pipes(t_token *tokens)
{
	int		i;
	t_token	*current;

	i = 0;
	if (tokens == NULL)
		return (0);
	current = tokens;
	while (current != NULL)
	{
		if (current->type && current->type == PIPELINE)
			i++;
		current = current->next;
	}
	return (i);
}
