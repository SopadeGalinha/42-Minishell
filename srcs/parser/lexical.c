/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexical.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhogonca <jhogonca@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 16:14:02 by jhogonca          #+#    #+#             */
/*   Updated: 2024/04/07 16:44:31 by jhogonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	remove_quotes_aux(char *data, int *si, char **res, int *quote_type)
{
	char	*aux;
	char	*tmp;

	if ((!*quote_type && (data[si[INDEX]] == '\"' || data[si[INDEX]] == '\''))
		|| (*quote_type == data[si[INDEX]]))
	{
		if (*quote_type == data[si[INDEX]])
			*quote_type = 0;
		else
			*quote_type = data[si[INDEX]];
		aux = ft_substr(data, si[START], si[INDEX] - si[START]);
		tmp = ft_strjoin(*res, aux);
		free(*res);
		free(aux);
		*res = tmp;
		si[START] = si[INDEX] + 1;
	}
}

static char	*remove_quotes(char *data)
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
		remove_quotes_aux(data, si, &result, &quote_type);
	aux = ft_substr(data, si[START], si[INDEX] - si[START]);
	tmp = ft_strjoin(result, aux);
	free(result);
	free(aux);
	result = tmp;
	free(data);
	return (result);
}

static bool	auxiliar(char *data, t_token **tokens, int *si, char *input)
{
	int	error;

	error = NO_ERROR;
	if (si[QT] != 0)
		error = UNCLOSED_QUOTE;
	data = ft_substr(input, si[START], si[INDEX] - si[START]);
	data = remove_quotes(data);
	if (si[QT] != 0)
		error = UNCLOSED_QUOTE;
	addtoken(tokens, data, (int []){si[QT_TYPE], error});
	if (error != NO_ERROR)
		return (print_error(MS_ERR UNCLOSED_QT, 1));
	return (true);
}

static bool	analyse_quotes(char *input, t_token **tokens, int *si, char *data)
{
	while (ft_isspace(input[si[INDEX]]) && si[INDEX]++ && si[START]++)
		;
	while (input[si[INDEX]] && (!ft_isspace(input[si[INDEX]]) \
		|| si[QT] != 0))
	{
		if (input[si[INDEX]] == '=' && input[si[INDEX] + 1]
			&& input[si[INDEX] + 1] == '$')
		{
			si[INDEX] += 2;
			continue ;
		}
		if (is_special_char(input[si[INDEX]]) && si[QT] == 0)
			break ;
		if (input[si[INDEX]] == '"' || input[si[INDEX]] == '\'')
		{
			si[QT_TYPE] = input[si[INDEX]];
			if (si[QT] == 0)
				si[QT] = input[si[INDEX]];
			else if (si[QT] == input[si[INDEX]])
				si[QT] = 0;
		}
		si[INDEX]++;
	}
	return (auxiliar(data, tokens, si, input));
}

bool	lexical(char *input, t_token **tokens)
{
	int		si[4];
	char	*data;

	data = NULL;
	si[INDEX] = 0;
	si[START] = 0;
	while (si[INDEX] < (int)ft_strlen(input))
	{
		si[QT] = 0;
		si[QT_TYPE] = 0;
		while (input[si[INDEX]] == ' ')
			si[INDEX]++;
		if (si[INDEX] >= (int)ft_strlen(input))
			break ;
		si[START] = si[INDEX];
		if (is_special_char(input[si[INDEX]]))
		{
			si[INDEX] = cmds_data(input, si[INDEX], si[START], tokens);
			si[INDEX]++;
		}
		else
			if (!analyse_quotes(input, tokens, si, data))
				return (false);
	}
	return (true);
}
