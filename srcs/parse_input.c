/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhogonca <jhogonca@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 15:23:53 by jhogonca          #+#    #+#             */
/*   Updated: 2023/10/05 15:23:53 by jhogonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/* char	*ft_getenv(char **env, char *var_name)
{
	int		i;
	char	*tmp;

	i = -1;
	while (env[++i] != NULL)
		if (ft_strncmp(env[i], var_name, ft_strlen(var_name)) == 0)
			return (ft_substr(env[i], ft_strlen(var_name) + 1, ft_strlen(env[i])));
	return (NULL);
}
 */

char	*get_env_value(t_env *env, char *key)
{
	t_env	*current;

	current = env;
	while (current)
	{
		if (ft_strncmp(current->key, key, ft_strlen(key)) == 0 && ft_strlen(current->key) == ft_strlen(key))
			return (current->value);
		current = current->next;
	}
	return (ft_strdup(""));
}

char	*get_key(char *token, int i)
{
	int		start;
	char	*key;

	start = i;
	while (ft_isalnum(token[++i]) || token[i] == '_')
		;
	key = ft_substr(token, start + 1, i - start - 1);
	return (key);
	printf("key: %s\n", key);
	exit(0);
}

char *expand_env(t_env *env, char *token)
{
	int		i;
	int		start;
	char	*aux;
	char	*key;
	char	*value;

	i = -1;
	while (token[++i])
	{
		if (token[i] == '$')
		{
			start = i;
			while (ft_isalnum(token[++i]) || token[i] == '_' || token[i] == '?')
				;
			key = ft_substr(token, start + 1, i - start - 1);
			value = get_env_value(env, key);
			free(key);
			key = ft_substr(token, 0, start);
			aux = ft_strjoin(key, value);
			free(key);
			key = ft_substr(token, i, ft_strlen(token) - i);
			free(token);
			token = ft_strjoin(aux, key);
			i = start + ft_strlen(value) - 1;
			free(aux);
			free(key);
			free(value);
		}
	}
	return (token);
}

bool	parse_input(char *path_env, t_shell *shell)
{
	t_token	*current;

	if (lexical(shell->input, &shell->tokens) != NO_ERROR)
		return (false);
	current = shell->tokens;
	while (current != NULL)
	{
		current->type = define_token(current->data);
		if (current->quote != SINGLE)
		{
			if (current->prev != NULL && current->prev->type == HEREDOC)
				;
			else
				current->data = expand_env(shell->env, current->data);
		}
		if (current->type == OR || current->type == AND || current->type == SEMICOLON)
			if (current->quote == NONE)
				return (print_error("This beautiful shell does not support multiple commands yet", 1));
		current = current->next;
	}
	return (true);
}
