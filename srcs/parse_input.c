/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhogonca <jhogonca@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 15:23:53 by jhogonca          #+#    #+#             */
/*   Updated: 2023/10/05 15:23:53 by jhogonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

bool	parse_input(t_shell *shell)
{
	if (!lexical_analyzer(shell->input, &shell->tokens))
		return (false);
	if (process_tokens(shell) == false)
		return (false);
	if (handle_pipes(shell) == false)
		return (false);
	print_pipes(shell->pipes);
	return (true);
}
