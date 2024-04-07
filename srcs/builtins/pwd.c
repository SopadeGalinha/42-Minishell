/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhogonca <jhogonca@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/06 15:29:54 by jhogonca          #+#    #+#             */
/*   Updated: 2024/04/06 15:44:16 by jhogonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_pwd(t_shell *shell, t_pipes *pipes)
{
	char	*pwd;

	(void)shell;
	(void)pipes;
	if (pipes->cmds[1] && pipes->cmds[1][0] == '-')
	{
		if (pipes->cmds[1][1])
		{
			ft_printf_fd(STDOUT_FILENO, "pwd does not support options\n");
			g_exit_status = 1;
			return ;
		}
	}
	pwd = getcwd(NULL, 0);
	ft_printf_fd(STDOUT_FILENO, "%s\n", pwd);
	free(pwd);
	g_exit_status = 0;
}
