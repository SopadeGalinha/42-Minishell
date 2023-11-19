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


static void	read_heredoc_lines(int fd, char *file, t_shell *shell)
{
	char	*line;

	while (true)
	{
		line = readline("heredoc> ");
		if (line == NULL)
		{
			ft_printf_fd(2, "minishell: warning: here-document delimited");
			ft_printf_fd(2, " by end-of-file (wanted `%s')\n", file);
			break ;
		}
		if (ft_strncmp(line, file, ft_strlen(file)) == 0
			&& ft_strlen(line) == ft_strlen(file))
		{
			free(line);
			break ;
		}
		line = expand_env(shell->env, line);
		ft_putendl_fd(line, fd);
		free(line);
	}
}

static bool	redir_heredoc(int *last_valid_fd, char *file, bool *heredoc, t_shell *sh)
{
	int	fd;

	if (*last_valid_fd != -1)
		close(*last_valid_fd);
	if (access(".heredoc", F_OK) != -1)
		unlink(".heredoc");
	fd = open(".heredoc", O_CREAT | O_RDWR | O_TRUNC, PERMISSIONS);
	if (fd == -1)
	{
		perror("minishell");
		return (false);
	}
	read_heredoc_lines(fd, file, sh);
	close(fd);
	fd = open(".heredoc", O_RDONLY);
	*last_valid_fd = fd;
	*heredoc = true;
	*last_valid_fd = fd;
	return (true);
}

static void	process_redir_in(t_shell *shell, t_redir *redir, t_pipes *current)
{
	int		last_valid_fd;
	bool	heredoc;

	last_valid_fd = -1;
	heredoc = false;
	while (redir)
	{
		if (redir->type == REDIR_IN)
			if (!redir_in(&last_valid_fd, redir->file, &heredoc))
				current->fd[IN] = -2;
		if (redir->type == HEREDOC)
		{
			if (!redir_heredoc(&last_valid_fd, redir->file, &heredoc, shell))
				current->fd[IN] = -2;
			else
			{
				free(redir->file);
				redir->file = ft_strdup(".heredoc");
				current->fd[IN] = last_valid_fd;
			}	
		}
		if (current->fd[IN] == -2)
			return ;
		redir = redir->next;
	}
	current->fd[IN] = last_valid_fd;
	current->in = last_valid_fd;
}

static bool	process_redir_out(t_shell *shell, t_redir *redir, t_pipes *current)
{
	int	val_fd;

	val_fd = -1;
	while (redir)
	{
		if (val_fd != -1)
			close(val_fd);
		if (redir->type == REDIR_OUT)
			val_fd = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (redir->type == D_REDIR_OUT)
			val_fd = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (val_fd == -1)
		{
			perror("minishell");
			return (false);
		}
		redir = redir->next;
	}
	current->fd[OUT] = val_fd;
	current->out = val_fd;
	return (true);
}

bool	process_pipeline(t_shell *shell)
{
	t_pipes	*current;

	current = shell->pipes;
	while (current)
	{
		process_redir_in(shell, current->redir_in, current);
		if (!process_redir_out(shell, current->redir_out, current))
			return (false);
		current = current->next;
	}
	return (true);
}
