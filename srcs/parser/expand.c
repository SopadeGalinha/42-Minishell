/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhogonca <jhogonca@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 16:48:19 by jhogonca          #+#    #+#             */
/*   Updated: 2024/04/07 16:50:15 by jhogonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*aux_expand(t_env *env, char *token, int *si)
{
	char	*aux;
	char	*key;
	char	*value;
	char	*new_token;

	key = ft_substr(token, si[START] + 1, si[INDEX] - si[START] - 1);
	value = get_env_value(env, key);
	free(key);
	key = ft_substr(token, 0, si[START]);
	aux = ft_strjoin(key, value);
	free(key);
	free(value);
	if (!token[si[INDEX]])
	{
		free(token);
		return (aux);
	}
	key = ft_substr(token, si[INDEX], ft_strlen(token) - si[INDEX]);
	new_token = ft_strjoin(aux, key);
	free(token);
	free(aux);
	free(key);
	return (new_token);
}

static char	*pre_aux_expand(t_env *env, char *new_token, int *si)
{
	while (new_token[++si[INDEX]] != '\0')
	{
		if (new_token[si[INDEX]] == '$')
		{
			si[START] = si[INDEX];
			while (isalnum(new_token[++si[INDEX]]))
				;
			new_token = aux_expand(env, new_token, si);
			if (!new_token)
				return (NULL);
			si[INDEX] = -1;
		}
	}
	return (new_token);
}

char	*expand_env(t_env *env, char *new_token)
{
	int		si[2];

	si[START] = 0;
	si[INDEX] = -1;
	if (!new_token)
		return (NULL);
	if (ft_strcmp(new_token, "$$") == 0)
	{
		free(new_token);
		return (ft_itoa(getpid()));
	}
	if (ft_strcmp(new_token, "$") == 0)
	{
		free(new_token);
		return (ft_strdup("$"));
	}
	return (pre_aux_expand(env, new_token, si));
}
