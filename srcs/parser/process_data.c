/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_data.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhogonca <jhogonca@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 16:23:26 by jhogonca          #+#    #+#             */
/*   Updated: 2024/04/07 16:25:33 by jhogonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	process_special_tokens(char *data, t_token **tokens)
{
	if (ft_strncmp(data, "&", 1) == 0 && ft_strlen(data) == 1)
		addtoken(tokens, data, (int []){NONE, BACKGROUND_NOT_SUPPORTED});
	else if (ft_strncmp(data, "||", 2) == 0 && ft_strlen(data) == 2)
		addtoken(tokens, data, (int []){NONE, D_PIPELINE_NOT_SUPPORTED});
	else if (ft_strncmp(data, ";", 1) == 0 && ft_strlen(data) == 1)
		addtoken(tokens, data, (int []){NONE, SEMICOLON_NOT_SUPPORTED});
	else if (ft_strncmp(data, "(", 1) == 0 && ft_strlen(data) == 1)
		addtoken(tokens, data, (int []){NONE, PARENTHESIS_NOT_SUPPORTED});
	else if (ft_strncmp(data, ")", 1) == 0 && ft_strlen(data) == 1)
		addtoken(tokens, data, (int []){NONE, PARENTHESIS_NOT_SUPPORTED});
	else
		addtoken(tokens, data, (int []){NONE, NO_ERROR});
}

static int	cmd_aux_1(char *input, int i)
{
	while (input[++i] != '\0' && \
	(!is_special_char(input[i] && input[i] != '$'))
		&& input[i] != '"' && input[i] != '\''
		&& input[i] != ' ' && input[i])
		;
	return (i);
}

static int	cmd_aux_2(char *input, int i)
{
	while (input[++i] != '\0' && !is_special_char(input[i])
		&& input[i] != '"' && input[i] != '\''
		&& input[i] != ' ' && input[i])
		;
	return (i);
}

int	cmds_data(char *input, int i, int start, t_token **tokens)
{
	char	*data;

	if (input[i] == '$')
	{
		i = cmd_aux_1(input, i);
		data = ft_substr(input, start, (i-- - start));
	}
	else if ((input[i] == '>' || input[i] == '<' || input[i] == '(')
		|| (input[i] == '|' || input[i] == '&') || input[i] == ';' || \
		input[i] == ')')
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
		i = cmd_aux_2(input, i);
		data = ft_substr(input, start, (i-- - start));
	}
	process_special_tokens (data, tokens);
	return (i);
}
