/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhogonca <jhogonca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/05 00:15:19 by heolivei          #+#    #+#             */
/*   Updated: 2023/12/05 01:17:07 by jhogonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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
	int	i;

	(void)pipes;
	i = -1;
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

void	single_cmd_child(t_shell *shell, t_pipes *pipes_lst)
{
	signals_child();
	if (pipes_lst->redir_out)
	{
		dup2(pipes_lst->redir_fd[OUT], STDOUT_FILENO);
		close(pipes_lst->redir_fd[OUT]);
	}
	if (pipes_lst->redir_in)
	{
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
	ft_execve(shell, pipes_lst);
}

void	single_cmd_aux(t_shell *shell, t_pipes *pipes_lst)
{
	pipes_lst->pid = fork();
	if (pipes_lst->pid == -1)
	{
		perror("Error with creating process");
		g_exit_status = 1;
		exit(EXIT_FAILURE);
	}
	if (pipes_lst->pid == 0)
		single_cmd_child(shell, pipes_lst);
	signals_wait();
	waitpid(pipes_lst->pid, &g_exit_status, 0);
	if (WIFEXITED(g_exit_status))
		g_exit_status = WEXITSTATUS(g_exit_status);
	else if (WIFSIGNALED(g_exit_status))
		g_exit_status = WTERMSIG(g_exit_status) + 128;
}
