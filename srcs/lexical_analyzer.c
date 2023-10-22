/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexical_analyzer.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhogonca <jhogonca@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 12:49:50 by jhogonca          #+#    #+#             */
/*   Updated: 2023/10/22 12:49:50 by jhogonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static t_token	*create_token(char *data, int *quo_err)
{
	t_token	*new;

	new = malloc(sizeof(t_token));
	if (!new)
		return (NULL);
	if (quo_err[QUOTE] != SINGLE && quo_err[QUOTE] != DOUBLE)
		quo_err[QUOTE] = NONE;
	new->data = ft_strdup(data);
	free(data);
	new->quote = quo_err[QUOTE];
	new->error = quo_err[ERROR];
	new->next = NULL;
	return (new);
}

static void	addtoken(t_token **tokens, char *data, int *quo_err)
{
	t_token	*new;
	t_token	*last;

	if (data == NULL)
		return ;
	new = create_token(data, quo_err);
	if (new == NULL)
		return ;
	if (*tokens == NULL)
	{
		*tokens = new;
		new->prev = NULL;
		return ;
	}
	last = *tokens;
	while (last->next != NULL)
		last = last->next;
	last->next = new;
	new->prev = last;
}

bool	is_special_char(char c)
{
	if (c == '>' || c == '<' || c == '|' || c == '$' || c == ';')
		return (true);
	return (false);
}

static void	process_special_tokens(char *data, t_token **tokens)
{
	if (ft_strncmp(data, "&", 1) == 0 && ft_strlen(data) == 1)
		addtoken(tokens, data, (int []){NONE, BACKGROUND_NOT_SUPPORTED});
	else if (ft_strncmp(data, "||", 2) == 0 && ft_strlen(data) == 2)
		addtoken(tokens, data, (int []){NONE, D_PIPELINE_NOT_SUPPORTED});
	else if (ft_strncmp(data, ";", 1) == 0 && ft_strlen(data) == 1)
		addtoken(tokens, data, (int []){NONE, SEMICOLON_NOT_SUPPORTED});
	else
		addtoken(tokens, data, (int []){NONE, NO_ERROR});
}

static int	cmds_data(char *input, int i, int start, t_token **tokens)
{
	char	*data;

	if ((input[i] == '>' || input[i] == '<')
		|| (input[i] == '|' || input[i] == '&') || input[i] == ';')
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
		while (input[++i] != '\0' && !is_special_char(input[i])
			&& input[i] != '"' && input[i] != '\''
			&& input[i] != ' ' && input[i])
			;
		data = ft_substr(input, start, (i-- - start));
	}
	process_special_tokens (data, tokens);
	return (i);
}

char	*remove_quotes(char *data)
{
	int		si[2];
	int		quote_type;
	char	*aux;
	char	*tmp;
	char	*result;

	si[INDEX] = -1;
	si[START] = 0;
	quote_type = 0;
	result = ft_strdup("");
	while (data[++si[INDEX]])
	{
		if (!quote_type && (data[si[INDEX]] == '"' \
			|| data[si[INDEX]] == '\'') || (quote_type == data[si[INDEX]]))
		{
			if (quote_type == data[si[INDEX]])
				quote_type = 0;
			else
				quote_type = data[si[INDEX]];
			aux = ft_substr(data, si[START], si[INDEX] - si[START]);
			tmp = ft_strjoin(result, aux);
			free(result);
			free(aux);
			result = tmp;
			si[START] = si[INDEX] + 1;
		}
	}
	aux = ft_substr(data, si[START], si[INDEX] - si[START]);
	tmp = ft_strjoin(result, aux);
	free(result);
	free(aux);
	result = tmp;
	free(data);
	return (result);
}

static bool	lexical_aux_aux(int quote, char *data, int error, t_token **tokens, int *si, int quote_type, char *input)
{
	if (quote != 0)
		error = UNCLOSED_QUOTE;
	data = ft_substr(input, si[START], si[INDEX] - si[START]);
	data = remove_quotes(data);
	if (quote != 0)
		error = UNCLOSED_QUOTE;
	addtoken(tokens, data, (int []){quote_type, error});
	if (error != NO_ERROR)
		return (print_error(UNCLOSED_QT, 1));
	return (true);
}

bool	lexical_aux(char *input, t_token **tokens, int *si, char quote, int error, int quote_type, char *data)
{
	while (ft_isspace(input[si[INDEX]]))
	{
		si[INDEX]++;
		si[START]++;
	}
	while (input[si[INDEX]] && (!ft_isspace(input[si[INDEX]]) \
		|| quote != 0))
	{
		if (input[si[INDEX]] == '"' || input[si[INDEX]] == '\'')
		{
			quote_type = input[si[INDEX]];
			if (quote == 0)
				quote = input[si[INDEX]];
			else if (quote == input[si[INDEX]])
				quote = 0;
		}
		si[INDEX]++;
	}
	return (lexical_aux_aux(quote, data, error, tokens, si, quote_type, input));
}

bool	lexical_analyzer(char *input, t_token **tokens)
{
	int		si[2];
	int		error;
	char	*data;
	int		quote;
	int		quote_type;

	si[INDEX] = -1;
	si[START] = 0;
	error = NO_ERROR;
	while (++si[INDEX] < (int)strlen(input))
	{
		quote = 0;
		si[START] = si[INDEX];
		quote_type = 0;
		if (is_special_char(input[si[INDEX]]))
		{
			si[INDEX] = cmds_data(input, si[INDEX], si[START], tokens);
			continue ;
		}
		else
			if (!lexical_aux(input, tokens, si, quote, error, quote_type, data))
				return (false);
	}
	return (true);
}
