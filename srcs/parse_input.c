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

static bool	validate_tokens(t_shell *shell)
{
	// mexer dps
/* 	t_token	*token;

	token = shell->tokens;
	while (token)
	{
		if (token->type == TOKEN_PIPE)
		{
			if (!token->next || token->next->type == TOKEN_PIPE)
				return (false);
		}
		else if (token->type == TOKEN_REDIR_IN || token->type == TOKEN_REDIR_OUT)
		{
			if (!token->next || token->next->type == TOKEN_PIPE)
				return (false);
		}
		else if (token->type == TOKEN_REDIR_APPEND)
		{
			if (!token->next || token->next->type == TOKEN_PIPE)
				return (false);
		}
		else if (token->type == TOKEN_REDIR_HERE_DOC)
		{
			if (!token->next || token->next->type == TOKEN_PIPE)
				return (false);
		}
		else if (token->type == TOKEN_REDIR_HERE_STR)
		{
			if (!token->next || token->next->type == TOKEN_PIPE)
				return (false);
		}
		else if (token->type == TOKEN_WORD)
		{
			if (!token->next || token->next->type == TOKEN_PIPE)
				return (false);
		}
		else if (token->type == TOKEN_ENV)
		{
			if (!token->next || token->next->type == TOKEN_PIPE)
				return (false);
		}
		else if (token->type == TOKEN_ENV_EXP)
		{
			if (!token->next || token->next->type == TOKEN_PIPE)
				return (false);
		}
		else if (token->type == TOKEN_ENV_ASSIGN)
		{
			if (!token->next || token->next->type == TOKEN_PIPE)
				return (false);
		}
		token = token->next;
	}
	*/
	return (true);
}

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
	if (!validate_tokens(shell))
		return (false);
	if (!create_pipeline_node(shell))
		return (false);
	process_pipeline(shell);
	pipes = shell->pipes;
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
