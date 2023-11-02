/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexical_analyzer.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhogonca <jhogonca@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/28 12:34:19 by jhogonca          #+#    #+#             */
/*   Updated: 2023/10/28 12:34:19 by jhogonca         ###   ########.fr       */
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

void	addtoken(t_token **tokens, char *data, int *quo_err)
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

bool	lexical_analyzer(char *input, t_token **tokens)
{
	int		si[4];
	char	*data;

	data = NULL;
	si[INDEX] = 0;
	si[START] = 0;
	while (si[INDEX] < (int)strlen(input))
	{
		si[QT] = 0;
		si[START] = si[INDEX];
		si[QT_TYPE] = 0;
		if (is_special_char(input[si[INDEX]]))
			si[INDEX] = cmds_data(input, si[INDEX], si[START], tokens);
		else
			if (!lexical_aux(input, tokens, si, data))
				return (false);
		si[INDEX]++;
	}
	return (true);
}
