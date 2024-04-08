/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhogonca <jhogonca@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 19:31:09 by jhogonca          #+#    #+#             */
/*   Updated: 2024/04/08 19:31:48 by jhogonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_heredoc(t_pipes *pipes_lst)
{
	pipe(pipes_lst->redir_fd);
	ft_printf_fd(pipes_lst->redir_fd[WRITE_END], "%s", pipes_lst->heredoc);
	close(pipes_lst->redir_fd[WRITE_END]);
	dup2(pipes_lst->redir_fd[READ_END], STDIN_FILENO);
	close(pipes_lst->redir_fd[READ_END]);
}

void	close_redirections(t_pipes *pipes_lst, int \
process_num, int **pipes, int pos)
{
	int	i;

	i = -1;
	while (++i < process_num)
	{
		if (i != pos && i != pos - 1)
		{
			close(pipes[i][READ_END]);
			close(pipes[i][WRITE_END]);
		}
	}
	if (pipes_lst->redir_in)
		close(pipes_lst->redir_fd[IN]);
	if (pipes_lst->redir_out)
		close(pipes_lst->redir_fd[OUT]);
}

void	redirect_input( t_pipes *pipes_lst, \
int **pipes)
{
	(void)pipes;
	if (!pipes_lst->do_heredoc)
	{
		if (pipes_lst->redir_fd[IN] == -1)
		{
			perror("Error opening file for input redirection");
			exit(EXIT_FAILURE);
		}
		else
		{
			dup2(pipes_lst->redir_fd[IN], STDIN_FILENO);
			close(pipes_lst->redir_fd[IN]);
		}
	}
	else
		ft_heredoc(pipes_lst);
}

void	get_redirections(int pos, int **pipes, t_pipes *pipes_lst, \
t_shell *shell)
{
	(void)shell;
	if (pos != 0)
	{
		dup2(pipes[pos - 1][READ_END], STDIN_FILENO);
		close(pipes[pos - 1][READ_END]);
		close(pipes[pos - 1][WRITE_END]);
	}
	if (pipes_lst->next)
	{
		dup2(pipes[pos][WRITE_END], STDOUT_FILENO);
		close(pipes[pos][READ_END]);
		close(pipes[pos][WRITE_END]);
	}
	if (pipes_lst->redir_in)
		redirect_input(pipes_lst, pipes);
	if (pipes_lst->redir_out)
	{
		dup2(pipes_lst->redir_fd[OUT], STDOUT_FILENO);
		close(pipes_lst->redir_fd[OUT]);
	}
}
