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
	t_token	*token;

	token = shell->tokens;
	while (token->next)
	{
		if ((token->type == PIPELINE && token->next->type == PIPELINE)
			|| (token->type == PIPELINE && token->quote != NONE))
			return (print_error(SYNTAX BOLD_WHITE" `|'"RESET, 2));
		if (token->type == REDIR_IN && token->next->type != WORD)
			return (print_error(SYNTAX BOLD_WHITE " `<'" RESET, 2));
		if (token->type == REDIR_OUT && token->next->type != WORD)
			return (print_error(SYNTAX BOLD_WHITE " `>'" RESET, 2));
		if (token->type == APPEND && token->next->type != WORD)
			return (print_error(SYNTAX BOLD_WHITE " `>>'" RESET, 2));
		if (token->type == HEREDOC && token->next->type != WORD)
			return (print_error(SYNTAX BOLD_WHITE " `<<'" RESET, 2));
		token = token->next;
	}
	if (is_special_char(token->data[0]) && token->data[0] != '$')
	{
		ft_printf_fd(2, SYNTAX BOLD_WHITE" `%c'"RESET, token->data[0]); 
		return (print_error("", 2));
	}
	return (true);
}

static void	get_cmd_path(t_shell *shell)
{
	t_pipes		*pipes;
	int			is_builtin;
	const char	*builtin[7];
	
	is_builtin = -1;
	init_builtin(builtin);
	pipes = shell->pipes;
	if (!pipes->cmds || !pipes->cmds[0])
		return ;
	while (pipes)
	{
		is_builtin = ft_is_builtin(builtin, pipes->cmds[0]);
		if (is_builtin != -1 || ft_strcmp(pipes->cmds[0], "") == 0)
			;
		else if (pipes->cmds && pipes->cmds[0])
			ft_access(&pipes->cmds[0], shell);
		pipes = pipes->next;
	}
}

bool	parse_input(t_shell *shell)
{
	if (!lexical_analyzer(shell->input, &shell->tokens))
		return (false);
	if (!process_tokens(shell))
		return (false);
	if (!validate_tokens(shell))
		return (false);
	if (!create_pipeline_node(shell))
		return (false);
	if (!process_redirections(shell))
		return (false);
	get_cmd_path(shell);
	return (true);
}
