/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_tokens.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhogonca <jhogonca@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/28 19:18:08 by jhogonca          #+#    #+#             */
/*   Updated: 2023/09/28 19:18:08 by jhogonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"


static void addtoken(t_token **tokens, char *data, int *quo_err)
{
	t_token *new;
	t_token *last;

	if (data == NULL)
		return ;
	new = malloc(sizeof(t_token));
	if (!new)
		return ;
	new->data = ft_strdup(data);
	free(data);
	new->quote = quo_err[QUOTE];
	new->error = quo_err[ERROR];
	new->next = NULL;
	if (*tokens == NULL)
	{
		*tokens = new;
		new->prev = NULL;
		return;
	}
	last = *tokens;
	while (last->next != NULL)
		last = last->next;
	last->next = new;
	new->prev = last;
	
}

static int	cmds_data(char *input, int i, int start, t_token **tokens)
{
	char	*data;

	if ((input[i] == '>' || input[i] == '<') || (input[i] == '|' || input[i] == '&'))
	{
		if (input[i] == input[i + 1])
		{
			data = ft_substr(input, start, 2);
			i += 1;
		}
		else
			data = ft_substr(input, start, 1);
	}
	else
	{
		while (input[++i] != '\0' && input[i] != ' ' && input[i] \
			!= '>' && input[i] != '<' && input[i] != '|' && input[i] != '$'
			&& input[i] != '"' && input[i] != '\'')
				;
		data = ft_substr(input, start, (i-- - start));
	}
	if ((ft_strncmp(data, "&", 1)) == 0 && ft_strlen(data) == 1)
		addtoken(tokens, data, (int []){NONE, BACKGROUND_NOT_SUPPORTED});
	else if (ft_strncmp(data, "||", 2) == 0 && ft_strlen(data) == 2)
		addtoken(tokens, data, (int []){NONE, D_PIPELINE_NOT_SUPPORTED});
	else
		addtoken(tokens, data, (int []){NONE, NO_ERROR});
	return (i);
}

char	*remove_quotes(char *data)
{
	int		i;
	int		start;
	int		quote_type;
	char	*aux;
	char	*tmp;
	char	*result;

	i = -1;
	start = 0;
	quote_type = 0;
	result = ft_strdup("");
	while (data[++i])
	{
		if (!quote_type && (data[i] == '"' || data[i] == '\'') || (quote_type == data[i]))
		{
			if (quote_type == data[i])
				quote_type = 0;
			else
				quote_type = data[i];
			aux = ft_substr(data, start, i - start);
			tmp = ft_strjoin(result, aux);
			free(result);
			free(aux);
			result = tmp;
			start = i + 1;
		}
	}
	aux = ft_substr(data, start, i - start);
	tmp = ft_strjoin(result, aux);
	free(result);
	free(aux);
	result = tmp;
	free(data);
	return (result);
}

bool	is_special_char(char c)
{
	if (c == '>' || c == '<' || c == '|' || c == '&' || c == '$')
		return (true);
	return (false);
}

bool	lexical(char *input, t_token **tokens)
{
	int		i;
	int		start;
	int		error;
	char	*data;
	char	quote;

	i = -1;
	error = NO_ERROR;
	while (++i < (int)strlen(input))
	{
		start = i;
		if (is_special_char(input[i]))
		{
			i = cmds_data(input, i, start, tokens);
			continue ;
		}
		else 
		{
			while (input[++i] != '\0' && input[i] != ' ' && !is_special_char(input[i]))
			{
				if ((input[i] == '"' || input[i] == '\''))
				{
					quote = input[i];
					while (input[++i] != '\0' && input[i] != quote)
						;
					if (input[i] == '\0')
						error = UNCLOSED_QUOTE;
				}
			}
			data = ft_substr(input, start, (i-- - start));
			if (ft_contains(data, "\'") || ft_contains(data, "\""))
				data = remove_quotes(data);
			if (error != NO_ERROR)
				return (false);
			addtoken(tokens, data, (int []){NONE, error});
		}
	}
	return (error);
}
