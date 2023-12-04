/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_tokens_aux.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heolivei <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 21:11:07 by heolivei          #+#    #+#             */
/*   Updated: 2023/12/04 21:11:09 by heolivei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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
