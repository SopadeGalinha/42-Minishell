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

static int	quote_data(char *input, int i, int start, t_token **tokens)
{
	char	*data;
	int		quote;
	int		ref;
	int		quo_err[2];

	ref = i - 1;
	quo_err[QUOTE] = SINGLE;
	quo_err[ERROR] = NO_ERROR;
	while (input[i] != '\0' && input[i] != input[ref])
		i++;
	data = ft_substr(input, start + 1, i - start - 1);
	if (input[ref] == '"')
		quo_err[QUOTE] = DOUBLE;
	if (input[i] == '\0')
		quo_err[ERROR] = UNCLOSED_QUOTE;
	addtoken(tokens, data, quo_err);
	return (i);
}

static int	general_data(char *input, int i, int start, t_token **tokens)
{
	int		ref;
	char	*data;
	char	*temp_data;

	while (input[i] && input[i] != ' '
		&& input[i] != '>' && input[i] != '<'
		&& input[i] != '|' && input[i] != '$')
	{
		if (input[i] == '"' || input[i] == '\'')
		{
			start = i + 1;
			i++;
			while (input[i] != '\0' && input[i] != input[start - 1])
				i++;
			temp_data = ft_substr(input, start, i - start);
			data = ft_strjoin(data, temp_data);
			free(temp_data);
			if (input[i] == '\0')
				addtoken(tokens, data, (int []){NONE, UNCLOSED_QUOTE});
			else
				addtoken(tokens, data, (int []){NONE, NO_ERROR});
			// free(data);
			return (i);
		}
		else
			i++;
	}
	data = ft_substr(input, start, i - start);
	addtoken(tokens, data, (int []){NONE, NO_ERROR});
	// free(data);
	return (i);
}

bool	lexical(char *input, t_token **tokens)
{
	int		i;
	int		start;
	bool	error;

	i = -1;
	error = false;
	while (++i <= (int)strlen(input) - 1)
	{
		start = i;
		if ((input[i] == '"' || input[i] == '\''))
			i = quote_data(input, ++i, start, tokens);
		else if (input[i] == '>' || input[i] == '<' || input[i] == '|'
			|| input[i] == '$' || input[i] == '&')
			i = cmds_data(input, i, start, tokens);
		else if (input[i] == 32 || input[i] == '\t' || input[i] == '\n')	
		{
			while (ft_isspace(input[i]))
				i++;
			addtoken(tokens, ft_substr(input, start, i-- - start), (int []){NONE, NO_ERROR});
		}
		else if (input[i] == '2' && input[i + 1] == '>')
		{
			i++;
			addtoken(tokens, ft_substr(input, start, 2), (int []){NONE, NO_ERROR});
		}
		else if (input[i] == ';')
			addtoken(tokens, ft_substr(input, start, 1), (int []){NONE, SEMICOLON_NOT_SUPPORTED});
		else if (input[i] == ' ' || input[i] == '\t' || input[i] == '\n')
			continue ;
		else
			i = general_data(input, i, start, tokens);
		if ((*tokens)->error != NO_ERROR)
			error = true;
	}
	// resolver o problema (?>>)
	return (error);
}
