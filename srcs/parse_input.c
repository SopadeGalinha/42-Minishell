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
	fd = open(file, O_CREAT | O_WRONLY | O_TRUNC, PERMISSIONS);
	if (fd == -1)
	{
		perror("minishell");
		return (false);
	}
	*last_valid_fd = fd;
	*heredoc = true;
	return (true);
}

bool	process_redir_in(t_shell *shell, t_pipes *current)
{
	t_redir	*redir;
	int		last_valid_fd;
	bool	heredoc;
	int		fd;

	heredoc = false;
	last_valid_fd = -1;
	redir = current->redir_in;
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

static bool	redir_out(int *last_valid_fd, char *file)
{
	if (access(file, F_OK) != -1)
	{
		if (*last_valid_fd != -1)
			close(*last_valid_fd);
		*last_valid_fd = open(file, O_CREAT | O_WRONLY | O_TRUNC, PERMISSIONS);
		if (*last_valid_fd == -1)
		{
			perror("minishell");
			return (false);
		}
	}
	else
	{
		if (*last_valid_fd != -1)
			close(*last_valid_fd);
		perror("minishell");
		return (false);
	}
}

static	bool	dbl_redir_out(int *last_valid_fd, char *file)
{
	if (access(file, F_OK) != -1)
	{
		if (*last_valid_fd != -1)
			close(*last_valid_fd);
		*last_valid_fd = open(file, O_CREAT | O_WRONLY | O_APPEND, PERMISSIONS);
		if (*last_valid_fd == -1)
		{
			perror("minishell");
			return (false);
		}
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

bool process_redir_out(t_shell *shell, t_pipes *current)
{
    t_redir *redir;
    int last_valid_fd;
    int fd;

    last_valid_fd = -1;
    redir = current->redir_out;
    while (redir)
    {
        if (redir->type == REDIR_OUT)
        {
            if (!redir_out(&last_valid_fd, redir->file))
                return false;
        }
        else if (redir->type == D_REDIR_OUT)
        {
            if (!dbl_redir_out(&last_valid_fd, redir->file))
                return false;
        }
        redir = redir->next;
    }
    current->fd[OUT] = last_valid_fd;
    return true;
}

static bool	process_pipeline_node(t_shell *shell, t_pipes *current)
{
	if (!process_redir_in(shell, current))
		return (false);
	if (!process_redir_out(shell, current))
		return (false);
	return (true);
}

static bool	process_pipeline(t_shell *shell)
{
	t_pipes	*current;

	current = shell->pipes;
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
