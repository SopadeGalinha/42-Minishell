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

static bool	redir_in(int *last_valid_fd, char *file,
t_shell *shell, t_pipes *pipes)
{
	int	fd;

	pipes->do_heredoc = false;
	if (access(file, F_OK) != -1)
	{
		if (*last_valid_fd != -1)
			close(*last_valid_fd);
		fd = open(file, O_RDONLY);
		*last_valid_fd = fd;
		if (fd == -1)
		{
			if (shell->error == 0)
				ft_printf_fd(2, "minishell: %s: %s\n", file, strerror(errno));
			g_exit_status = 1;
			return (false);
		}
	}
	else
	{
		if (shell->error == 0)
			ft_printf_fd(2, MS_ERR RESET" %s %s\n", file, strerror(errno));
		g_exit_status = 1;
		return (false);
	}
	return (true);
}

static void	heredoc(char *target, t_pipes *current, t_shell *shell)
{
	char	*line;
	char	*aux;

	line = NULL;
	aux = NULL;
	current->do_heredoc = true;
	if (current->heredoc)
		free(current->heredoc);
	current->heredoc = ft_strdup("");
	while (true)
	{
		line = readline(BOLD_GREEN"heredoc> "RESET);
		if (heredoc_validate(line, target) == false)
			break ;
		line = expand_env(shell->env, line);
		aux = ft_strjoin(line, "\n");
		free(line);
		line = ft_strjoin(current->heredoc, aux);
		free(aux);
		free(current->heredoc);
		current->heredoc = line;
	}
}

void	process_redir_in(t_shell *shell, t_redir *redir, t_pipes *current)
{
	current->redir_fd[IN] = -1;
	while (redir)
	{
		if (redir->type == REDIR_IN)
		{
			if (!redir_in(&current->redir_fd[IN], redir->file, shell, current))
				shell->error = 1;
		}
		else if (redir->type == HEREDOC)
		{
			heredoc(redir->file, current, shell);
		}
		redir = redir->next;
	}
}

static bool	process_redir_out(t_redir *redir, t_pipes *current)
{
	int	fd;

	fd = -1;
	while (redir)
	{
		if (fd != -1)
			close(fd);
		if (redir->type == REDIR_OUT)
			fd = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, PERMISSIONS);
		if (redir->type == APPEND)
			fd = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, PERMISSIONS);
		if (fd == -1)
			return (print_error("minishell: ", 1));
		redir = redir->next;
	}
	current->redir_fd[OUT] = fd;
	return (true);
}

bool	process_redirections(t_shell *shell)
{
	t_pipes	*current;

	current = shell->pipes;
	shell->error = 0;
	while (current)
	{
		process_redir_in(shell, current->redir_in, current);
		if (!process_redir_out(current->redir_out, current))
			return (false);
		current = current->next;
	}
	if (shell->error)
		return (false);
	return (true);
}
