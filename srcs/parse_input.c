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
	t_pipes	*pipes;
	const char *builtin[7];
	int		is_builtin;

	init_builtin(builtin);
	if (!lexical_analyzer(shell->input, &shell->tokens))
		return (false);
	if (!process_tokens(shell))
		return (false);
	if (!create_pipeline_node(shell))
		return (false);
	if (process_pipeline(shell))
		return (false);
	pipes = shell->pipes;
	print_tokens(shell->tokens);
	exit(0);
	while (pipes)
	{
		is_builtin = ft_is_builtin(builtin, pipes->cmds[0]);
		if (is_builtin != -1)
			;
		else
			ft_access(&pipes->cmds[0], shell);
		pipes = pipes->next;
	}
	return (true);
}
