/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhogonca <jhogonca@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 17:41:43 by jhogonca          #+#    #+#             */
/*   Updated: 2024/04/08 19:38:51 by jhogonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool	create_pipes(t_shell *shell)
{
	int	i;
	int	process_num;

	process_num = count_pipes(shell->tokens) + 1;
	if (process_num < 2)
		return (false);
	shell->pipes_fd = malloc(sizeof(int *) * process_num);
	if (shell->pipes_fd)
	{
		i = -1;
		while (++i < process_num)
		{
			shell->pipes_fd[i] = malloc(sizeof(int) * 2);
			if (!shell->pipes_fd[i])
				return (false);
			if (pipe(shell->pipes_fd[i]) == -1)
				return (false);
		}
	}
	return (true);
}

int	ft_error(char *str, int exit_code)
{
	perror(str);
	return (exit_code);
}

int	ft_list_envsize(t_env *lst)
{
	int	i;

	i = 0;
	while (lst)
	{
		lst = lst->next;
		i++;
	}
	return (i);
}

void	close_pipes(int **pipes, int process_num)
{
	int	pos;

	pos = -1;
	while (++pos < process_num - 1)
	{
		close(pipes[pos][READ_END]);
		close(pipes[pos][WRITE_END]);
	}
}
