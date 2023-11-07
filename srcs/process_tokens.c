/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_tokens.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhogonca <jhogonca@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 12:27:34 by jhogonca          #+#    #+#             */
/*   Updated: 2023/10/22 12:27:34 by jhogonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	define_token(const char *token)
{
	if (ft_strncmp(token, "|", ft_strlen("|")) == 0 && ft_strlen(token) == 1)
		return (PIPELINE);
	if (ft_strncmp(token, ">", ft_strlen(">")) == 0 && ft_strlen(token) == 1)
		return (REDIR_OUT);
	if (ft_strncmp(token, "<", ft_strlen("<")) == 0 && ft_strlen(token) == 1)
		return (REDIR_IN);
	if (ft_strncmp(token, ">>", ft_strlen(">>")) == 0 && ft_strlen(token) == 2)
		return (D_REDIR_OUT);
	if (ft_strncmp(token, "<<", ft_strlen("<<")) == 0 && ft_strlen(token) == 2)
		return (HEREDOC);
	if (ft_strncmp(token, "2>", ft_strlen("2>")) == 0 && ft_strlen(token) == 2)
		return (REDIR_ERR);
	if (ft_strncmp(token, "$?", ft_strlen("$?")) == 0 && ft_strlen(token) == 2)
		return (EXIT_STATUS);
	if (ft_strncmp(token, "&&", ft_strlen("&&")) == 0 && ft_strlen(token) == 2)
		return (AND);
	if (ft_strncmp(token, "||", ft_strlen("||")) == 0 && ft_strlen(token) == 2)
		return (OR);
	if (ft_strncmp(token, ";", ft_strlen(";")) == 0)
		return (SEMICOLON);
	if (token[0] == '-' && ft_isalpha(token[1]))
		return (OPTION);
	if (token[0] == '$')
		return (ENV);
	return (WORD);
}

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

static char	*expand_env(t_env *env, char *new_token)
{
	int		si[2];

	si[START] = 0;
	si[INDEX] = -1;
	if (!new_token)
		return (NULL);
	while (new_token[++si[INDEX]] != '\0')
	{
		if (new_token[si[INDEX]] == '$')
		{
			si[START] = si[INDEX];
			while (ft_isalnum(new_token[++si[INDEX]]))
				;
			new_token = aux_expand(env, new_token, si);
			if (!new_token)
				return (NULL);
			si[INDEX] = -1;
		}
	}

	return (new_token);
}

bool	process_tokens(t_shell *shell)
{
	t_token	*current;

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
			if (current->data == NULL)
				return (false);
			if (ft_strncmp(current->data, "~", 1) == 0 && ft_strlen(current->data) == 1)
				if (current->quote != DOUBLE && current->quote != SINGLE)
				{
					free(current->data);
					current->data = ft_strdup(get_env_value(shell->env, "HOME"));
				}
		}
		if (current->type == OR
			|| current->type == AND || current->type == SEMICOLON)
			if (current->quote == NONE)
				return (print_error(UNSUP_MCMDS, 258));
		current = current->next;
	}
	return (true);
}
