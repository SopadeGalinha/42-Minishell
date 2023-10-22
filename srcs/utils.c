/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhogonca <jhogonca@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/28 19:43:34 by jhogonca          #+#    #+#             */
/*   Updated: 2023/09/28 19:43:34 by jhogonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

bool	ft_isspace_str(char *str)
{
	int	i;

	i = -1;
	while (str[++i])
		if (str[i] != 32 && (str[i] < 9 || str[i] > 13))
			return (false);
	return (true);
}

void	init_shell(t_shell *shell, char **env)
{
	ft_handle_signals();
	shell->error = NO_ERROR;
	shell->env = init_env(env);
	shell->exp = init_export(shell->env);
}

bool	print_error(char *error, int exit_code)
{
	ft_printf_fd(1, "%s\n", error);
	exit_status = exit_code;
	return (false);
}

bool	input_is_valid(char *input)
{
	int	end;

	end = ft_strlen(input) - 1;
	if (input[0] == ';')
		return (print_error(SYNTAX BOLD_WHITE" `;'"RESET, 258));
	if (input[0] == '|')
		return (print_error(SYNTAX BOLD_WHITE" `|'"RESET, 258));
	if (input[0] == '&')
		return (print_error(SYNTAX BOLD_WHITE" `&'"RESET, 258));
	if (input[end] == ';')
		return (print_error(SYNTAX BOLD_WHITE" `;'"RESET, 258));
	if (input[end] == '|')
		return (print_error(SYNTAX BOLD_WHITE" `|'"RESET, 258));
	if (input[end] == '&')
		return (print_error(SYNTAX BOLD_WHITE" `&'"RESET, 258));
	if (input[end] == '<')
		return (print_error(SYNTAX BOLD_WHITE" `<'"RESET, 258));
	if (input[end] == '>')
		return (print_error(SYNTAX BOLD_WHITE" `>'", 258));
	return (true);
}

void	get_input(t_shell *shell)
{
	shell->input = readline(MINISHELL);
	if (shell->input && !ft_isspace_str(shell->input))
		add_history(shell->input);
	if (shell->input)
		shell->input = ft_strtrim(shell->input, " \t");
}
