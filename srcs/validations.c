/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validations.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhogonca <jhogonca@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/28 17:34:52 by jhogonca          #+#    #+#             */
/*   Updated: 2023/10/28 17:34:52 by jhogonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

bool	input_is_valid(char *input)
{
	int	end;

	end = ft_strlen(input) - 1;
	if (input[0] == ';')
		return (print_error(MS_ERR RESET STX BOLD_WHITE" `;'"RESET, 2));
	if (input[0] == '|')
		return (print_error(MS_ERR RESET STX BOLD_WHITE" `|'"RESET, 2));
	if (input[0] == '&')
		return (print_error(MS_ERR RESET STX BOLD_WHITE" `&'"RESET, 2));
	if (input[end] == ';')
		return (print_error(MS_ERR RESET STX BOLD_WHITE" `;'"RESET, 2));
	if (input[end] == '|')
		return (print_error(MS_ERR RESET STX BOLD_WHITE" `|'"RESET, 2));
	if (input[end] == '&')
		return (print_error(MS_ERR RESET STX BOLD_WHITE" `&'"RESET, 2));
	if (input[end] == '<')
		return (print_error(MS_ERR RESET STX BOLD_WHITE" `<'"RESET, 2));
	if (input[end] == '>')
		return (print_error(MS_ERR RESET STX BOLD_WHITE" `>'", 2));
	return (true);
}

bool	ft_isdigit_str(char *str)
{
	int	i;

	i = -1;
	if (str[0] == '-' || str[0] == '+')
		i = 0;
	while (str[++i])
		if (str[i] < '0' || str[i] > '9')
			return (false);
	return (true);
}

bool	ft_isspace_str(char *str)
{
	int	i;

	i = -1;
	while (str[++i])
		if (str[i] != 32 && (str[i] < 9 || str[i] > 13))
			return (false);
	return (true);
}

int	cmd_aux_1(char *input, int i)
{
	while (input[++i] != '\0' && \
	(!is_special_char(input[i] && input[i] != '$'))
		&& input[i] != '"' && input[i] != '\''
		&& input[i] != ' ' && input[i])
		;
	return (i);
}

int	cmd_aux_2(char *input, int i)
{
	while (input[++i] != '\0' && !is_special_char(input[i])
		&& input[i] != '"' && input[i] != '\''
		&& input[i] != ' ' && input[i])
		;
	return (i);
}
