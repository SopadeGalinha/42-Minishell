/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhogonca <jhogonca@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/09 18:18:41 by jhogonca          #+#    #+#             */
/*   Updated: 2023/11/01 16:13:18 by jhogonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	execute(t_shell *shell)
{
	t_pipes		*pipes;
	int			function;
	const char	*builtin[7];

	builtin[0] = "pwd";
	builtin[1] = "cd";
	builtin[2] = "echo";
	builtin[3] = "export";
	builtin[4] = "exit";
	builtin[5] = "unset";
	builtin[6] = "env";
	
	function = -1;
	pipes = shell->pipes;
	while (++function < 7)
	{	
		if (ft_strncmp(pipes->cmds[0], builtin[function], \
		ft_strlen(builtin[function])) == 0
			&& ft_strlen(pipes->cmds[0]) == ft_strlen(builtin[function]))
			break ;
	}
	if (function < 7)
		shell->builtin[function](shell);
	else
		ft_printf_fd(shell->std_out, "Not a builtin\n");
}
