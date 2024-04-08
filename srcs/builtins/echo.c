/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhogonca <jhogonca@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/06 15:43:05 by jhogonca          #+#    #+#             */
/*   Updated: 2024/04/08 18:40:36 by jhogonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_echo(t_shell *shell, t_pipes *pipes)
{
	int		i;
	int		j;
	int		n_flag;

	i = 0;
	n_flag = 0;
	while (pipes->cmds[++i]
		&& ft_strncmp(pipes->cmds[i], "-n", 2) == 0)
	{
		j = 0;
		while (pipes->cmds[i][++j] == 'n')
			;
		if ((int)ft_strlen(pipes->cmds[i]) != (j))
			break ;
		n_flag = 1;
	}
	while (pipes->cmds[i])
	{
		ft_printf_fd(STDOUT_FILENO, "%s", pipes->cmds[i]);
		if (pipes->cmds[i++ + 1])
			ft_printf_fd(STDOUT_FILENO, " ");
	}
	if (!n_flag && shell)
		ft_printf_fd(STDOUT_FILENO, "\n");
	g_exit_status = 0;
}
