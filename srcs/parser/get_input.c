/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_input.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhogonca <jhogonca@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/06 15:52:03 by jhogonca          #+#    #+#             */
/*   Updated: 2024/04/06 15:58:59 by jhogonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static bool	ft_isspace_str(char *str)
{
	int	i;

	i = -1;
	while (str[++i])
		if (str[i] != 32 && (str[i] < 9 || str[i] > 13))
			return (false);
	return (true);
}

static bool	input_is_valid(char *input)
{
	int	end;

	end = ft_strlen(input) - 1;
	if (input[0] == ';')
		return (print_error(MS_ERR RESET STX BOLD_WHITE" `;'"RESET, 2));
	if (input[0] == '|')
		return (print_error(MS_ERR RESET STX BOLD_WHITE" `|'"RESET, 2));
	if (input[0] == '&')
		return (print_error(MS_ERR RESET STX BOLD_WHITE" `&'"RESET, 2));
	if (input[0] == '(' || input[end] == '(')
		return (print_error(MS_ERR RESET STX BOLD_WHITE" `('", 2));
	if (input[0] == ')' || input[end] == ')')
		return (print_error(MS_ERR RESET STX BOLD_WHITE" `)'", 2));
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

bool	get_input(t_shell *shell)
{
	char	*aux;

	shell->input = NULL;
	shell->error = NO_ERROR;
	shell->pipes_fd = NULL;
	shell->input = readline(MI NI S H EL L D);
	if (shell->input && !ft_isspace_str(shell->input))
		add_history(shell->input);
	if (shell->input)
	{
		aux = ft_strtrim(shell->input, " \t");
		free (shell->input);
		shell->input = aux;
	}
	if (shell->input == CTRL_D)
		ft_exit(shell, NULL);
	if (shell->input[0] && input_is_valid(shell->input))
		return (true);
	return (false);
}
