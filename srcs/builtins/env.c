/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhogonca <jhogonca@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/06 15:17:26 by jhogonca          #+#    #+#             */
/*   Updated: 2024/04/06 15:18:05 by jhogonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_env(t_shell *shell, t_pipes *pipes)
{
	if (pipes->cmds[1])
	{
		ft_printf_fd(STDERR_FILENO, "env: ");
		ft_printf_fd(STDERR_FILENO, "‘%s’: No such file or directory\n",
			pipes->cmds[1]);
		g_exit_status = 127;
	}
	else
		u_print_list(shell, 1, pipes);
}
