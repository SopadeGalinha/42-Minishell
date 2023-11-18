/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_pipeline.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhogonca <jhogonca@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 14:36:21 by jhogonca          #+#    #+#             */
/*   Updated: 2023/11/13 14:36:21 by jhogonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static bool	redir_in(int *last_valid_fd, char *file, bool *heredoc)
{
	int	fd;

	if (access(file, F_OK) != -1)
	{
		if (*last_valid_fd != -1 && !(*heredoc))
			close(*last_valid_fd);
		fd = open(file, O_RDONLY);
		if (fd == -1)
		{
			perror("minishell");
			return (false);
		}
		if (!(*heredoc))
			*last_valid_fd = fd;
	}
	else
	{
		if (*last_valid_fd != -1)
			close(*last_valid_fd);
		perror("minishell");
		return (false);
	}
	return (true);
}

static bool	redir_heredoc(int *last_valid_fd, char *file, bool *heredoc)
{
	int	fd;

	if (*last_valid_fd != -1)
		close(*last_valid_fd);
	fd = open(file, O_RDONLY);
	if (fd == -1)
	{
		perror("minishell");
		return (false);
	}
	*last_valid_fd = fd;
	*heredoc = true;
	return (true);
}

static bool	process_redir_in(t_shell *shell, t_redir *redir, t_pipes *current)
{
	int		last_valid_fd;
	bool	heredoc;

	last_valid_fd = -1;
	heredoc = false;
	while (redir)
	{
		if (redir->type == REDIR_IN)
			if (!redir_in(&last_valid_fd, redir->file, &heredoc))
				return (false);
		else if (redir->type == HEREDOC)
			if (!redir_heredoc(&last_valid_fd, redir->file, &heredoc))
				return (false);
		redir = redir->next;
	}
	current->fd[IN] = last_valid_fd;
	return (true);
}

static bool	process_redir_out(t_shell *shell, t_redir *redir, t_pipes *current)
{
	int	last_valid_fd;

	last_valid_fd = -1;
	while (redir)
	{
		if (redir->type == REDIR_OUT)
		{
			if (last_valid_fd != -1)
				close(last_valid_fd);
			last_valid_fd = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (last_valid_fd == -1)
			{
				perror("minishell");
				return (false);
			}
		}
		else if (redir->type == D_REDIR_OUT)
		{
			if (last_valid_fd != -1)
				close(last_valid_fd);
			last_valid_fd = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (last_valid_fd == -1)
			{
				perror("minishell");
				return (false);
			}
		}
		redir = redir->next;
	}
	current->fd[OUT] = last_valid_fd;
	return (true);
}

bool	process_pipeline(t_shell *shell)
{
	t_pipes	*current;

	current = shell->pipes;
	while (current)
	{
		// nao retornar false em erro
		if (!process_redir_in(shell, current->redir_in, current))
			return (false);
		if (!process_redir_out(shell, current->redir_out, current))
			return (false);
		current = current->next;
	}
	return (true);
}
