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

int	define_token(const char *token)
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

char	*get_env_value(t_env *env, char *key)
{
	t_env	*current;

	current = env;
	while (current)
	{
		if (ft_strncmp(current->key, key, ft_strlen(key)) == 0
			&& ft_strlen(current->key) == ft_strlen(key))
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

char	*expand_env(t_env *env, char *token)
{
	int		si[2];
	char	*aux;
	char	*key;
	char	*value;

	si[START] = 0;
	si[INDEX] = -1;
	
	while (token[++si[INDEX]])
	{
		if (token[si[INDEX]] == '$')
		{
			si[START] = si[INDEX];
			while (ft_isalnum(token[++si[INDEX]]) || token[si[INDEX]] == '_' || token[si[INDEX]] == '?')
				;
			key = ft_substr(token, si[START] + 1, si[INDEX] - si[START] - 1);
			value = get_env_value(env, key);
			free(key);
			key = ft_substr(token, 0, si[START]);
			
			aux = ft_strjoin(key, value);
			free(key);
			key = ft_substr(token, si[INDEX], ft_strlen(token) - si[INDEX]);
			free(token);
			token = ft_strjoin(aux, key);
			si[INDEX] = si[START] + ft_strlen(value) - 1;
			free(aux);
			free(key);
			free(value);
		}
	}
	return (token);
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
			else{
				current->data = expand_env(shell->env, current->data);
				exit(0);
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
