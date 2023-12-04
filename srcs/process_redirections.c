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


void	hdl_sigint_heredoc(int sig)
{
	if (sig == SIGINT)
	{
		ft_printf_fd(STDOUT_FILENO, "\n");
		return ;
	}
}

void	signals_heredoc(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_IGN);
	return ;
}

void	heredoc(char *target, t_pipes *current, t_shell *shell)
{
	char	*line;
	char	*aux;

	line = NULL;
	aux = NULL;
	current->do_heredoc = true;
	if (current->heredoc)
		free(current->heredoc);
	current->heredoc = ft_strdup("");
	signals_heredoc();
	while (true)
	{
		line = readline(BOLD_GREEN"heredoc> "RESET);
		if (!line)
		{
			ft_printf_fd(STDERR_FILENO, MS_ERR RESET "warning: here-document delimited by end-of-file (wanted `%s')\n", target);
			break ;
		}
		if (ft_strcmp(line, target) == 0)
		{
			free(line);
			break ;
		}
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
			heredoc(redir->file, current, shell);
		redir = redir->next;
	}
}

static bool	process_redir_out(t_redir *redir, t_pipes *current)
{
	int	val_fd;

	val_fd = -1;
	while (redir)
	{
		if (val_fd != -1)
			close(val_fd);
		if (redir->type == REDIR_OUT)
			val_fd = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, PERMISSIONS);
		if (redir->type == APPEND)
			val_fd = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, PERMISSIONS);
		if (val_fd == -1)
			return (print_error("minishell: ", 1));
		redir = redir->next;
	}
	current->redir_fd[OUT] = val_fd;
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
