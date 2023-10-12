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

static void	addtoken(t_token **tokens, char *data, int type, int quote)
{
	t_token	*new;
	t_token	*last;

	new = malloc(sizeof(t_token));
	new->data = data;
	new->type = type;
	new->quote = quote;
	new->next = NULL;
	if (*tokens == NULL)
	{
		*tokens = new;
		return ;
	}
	last = *tokens;
	while (last->next != NULL)
		last = last->next;
	last->next = new;
}

static int	cmds_data(char *input, int i, int start, t_token **tokens)
{
	char	*data;

	if (input[i] == '>' || input[i] == '<' || input[i] == '|')
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
			!= '>' && input[i] != '<' && input[i] != '|' && input[i] != '$')
			;
		data = ft_substr(input, start, i - start);
	}
	addtoken(tokens, data, define_token(data), NONE);
	return (i);
}

static int	quote_data(char *input, int i, int start, t_token **tokens)
{
	char	*data;
	int		quote;
	int		ref;

	ref = i - 1;
	quote = SINGLE;
	while (input[i] != '\0' && input[i] != input[ref])
		i++;
	data = ft_substr(input, start + 1, i - start - 1);
	if (input[ref] == '"')
		quote = DOUBLE;
	if (ft_strlen(data) > 0)
		addtoken(tokens, data, define_token(data), quote);
	return (i);
}

static int	general_data(char *input, int i, int start, t_token **tokens)
{
	char	*data;

	while (input[i] && input[i] != ' ' && input[i] \
		!= '>' && input[i] != '<' && input[i] != '|' && input[i] != '$')
		i++;
	data = ft_substr(input, start, i - start);
	addtoken(tokens, data, define_token(data), NONE);
	return (i);
}

void	lexical(char *input, t_token **tokens)
{
	int		i;
	int		start;

	i = -1;
	while (++i <= (int)strlen(input) - 1)
	{
		start = i;
		if ((input[i] == '"' || input[i] == '\''))
			i = quote_data(input, ++i, start, tokens);
		else if (input[i] == '>' || input[i] == '<' || input[i] == '|'
			|| input[i] == '$')
			i = cmds_data(input, i, start, tokens);
		else if (input[i] == '2' && input[i + 1] == '>')
		{
			i++;
			addtoken(tokens, ft_substr(input, start, 2), define_token("2>"), NONE);
		}
		else if (input[i] == ' ' || input[i] == '\t' || input[i] == '\n')
			continue ;
		else
			i = general_data(input, i, start, tokens);
	}
}
