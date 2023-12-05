/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhogonca <jhogonca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/05 00:15:26 by heolivei          #+#    #+#             */
/*   Updated: 2023/12/05 01:19:16 by jhogonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	**create_pipes(int process_num)
{
	int	**pipes;
	int	i;

	if (process_num <= 1)
		return (NULL);
	pipes = malloc(sizeof(int *) * process_num);
	if (!pipes)
		return (NULL);
	i = -1;
	while (++i < process_num)
	{
		pipes[i] = malloc(sizeof(int) * 2);
		if (!pipes[i])
			return (NULL);
	}
	i = -1;
	while (++i < process_num)
	{
		if (pipe(pipes[i]) == -1)
			return (NULL);
	}
	return (pipes);
}

void	ft_heredoc(t_pipes *pipes_lst)
{
	pipe(pipes_lst->redir_fd);
	ft_printf_fd(pipes_lst->redir_fd[WRITE_END], "%s", pipes_lst->heredoc);
	close(pipes_lst->redir_fd[WRITE_END]);
	dup2(pipes_lst->redir_fd[READ_END], STDIN_FILENO);
	close(pipes_lst->redir_fd[READ_END]);
}
