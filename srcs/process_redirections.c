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

static bool	redir_in(int *last_valid_fd, char *file, t_shell *shell)
{
	int	fd;

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

/*static bool	read_heredoc_lines(char *line, char *file, t_shell *shell, char *tmp)
{
	while (true)
	{
		free(line);
		line = readline(BOLD_ORANGE"> "RESET);
		if (!line)
		{
			ft_printf_fd(2, MS_ERR"heredoc: warning: here-document was delimited");
			ft_printf_fd(2, " by end-of-file (wanted `%s')\n"RESET, file);
			return (false);
		}
		if (ft_strncmp(line, file, ft_strlen(file)) == 0
			&& ft_strlen(line) == ft_strlen(file))
		{
			free(line);
			break ;
		}
		rl_on_new_line();
		tmp = ft_strjoin(line, "\n");
		shell->heredoc = ft_strjoin(shell->heredoc, tmp);
		free(tmp);
	}
	return (0);
}*/

/*static void	redir_heredoc(char *file, t_shell *shell)
{
	(void)file;
	(void)shell;
	 char	*line;
	char	*tmp;

	line = NULL;
	tmp = NULL;
	if (shell->heredoc)
		free(shell->heredoc);
	shell->heredoc = ft_strdup("");
	ft_handle_signals(HEREDOC);
	read_heredoc_lines(line, file, shell, tmp);
}*/

void	process_redir_in(t_shell *shell, t_redir *redir, t_pipes *current)
{
	current->redir_fd[IN] = -1;
	while (redir)
	{
		if (redir->type == REDIR_IN)
			if (!redir_in(&current->redir_fd[IN], redir->file, shell))
				shell->error = 1;
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
