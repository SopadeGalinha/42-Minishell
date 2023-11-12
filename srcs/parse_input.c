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

static bool process_redirections(t_shell *shell, t_pipes *current)
{
	t_redir	*redir;
	int		flag_heredoc;
	int		fd;
	int		last_valid_fd;

	flag_heredoc = 0;
	last_valid_fd = -1;
	redir = current->redir_in;
	while (redir)
	{
		if (redir->type == REDIR_IN)
		{
			if (access(redir->file, F_OK) != -1)
			{
				if (last_valid_fd != -1)
					close(last_valid_fd);
				fd = open(redir->file, O_RDONLY);
				if (fd == -1)
				{
					perror("minishell");
					return (false);
				}
				last_valid_fd = fd;
			}
			else
			{
				if (last_valid_fd != -1)
					close(last_valid_fd);
				perror("minishell");
				return false;
			}
		}
		redir = redir->next;
	}
	shell->std_in = last_valid_fd;
	return true;
}

static bool	process_pipeline_node(t_shell *shell, t_pipes *current)
{
	if (!process_redirections(shell, current))
		return (false);
}

static bool	process_pipeline(t_shell *shell)
{
	t_pipes	*current;
	
	current = shell->pipes;
	print_pipes(shell->pipes);
	exit(0);
	while (current)
	{
		if (!process_pipeline_node(shell, current))
			return (false);
		current = current->next;
	}
	return (true);
}

bool	parse_input(t_shell *shell)
{
	if (!lexical_analyzer(shell->input, &shell->tokens))
		return (false);
	if (!process_tokens(shell))
		return (false);
	if (!create_pipeline_node(shell))
		return (false);
	if (!process_pipeline(shell))
		return (false);
	return (true);
}
