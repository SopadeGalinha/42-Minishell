/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhogonca <jhogonca@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/06 15:32:44 by jhogonca          #+#    #+#             */
/*   Updated: 2024/04/06 16:23:49 by jhogonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	aux(char **cmds)
{
	int	i;
	int	flag;

	i = 0;
	flag = 0;
	if (ft_count_words(cmds) == 2)
	{
		if (cmds[1][0] == '-' || cmds[1][0] == '+')
			i++;
		while (cmds[1][i])
		{
			if (!ft_isdigit(cmds[1][i]))
			{
				print_error("minishell: exit: numeric argument required", 2);
				flag = 1;
				break ;
			}
			i++;
		}
		if (!flag)
			g_exit_status = ft_atoi(cmds[1]);
	}
}

void	ft_exit(t_shell *shell, t_pipes *pipes)
{
	if (count_pipes(shell->tokens) == 0)
		ft_printf_fd(STDOUT_FILENO, "exit\n");
	if (pipes && pipes->cmds && ft_count_words(pipes->cmds) > 2)
	{
		print_error("minishell: exit: too many arguments", 1);
		return ;
	}
	if (pipes && pipes->cmds)
		aux(pipes->cmds);
	free_struct(shell, 1);
}
