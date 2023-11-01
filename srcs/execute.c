/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhogonca <jhogonca@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/09 18:18:41 by jhogonca          #+#    #+#             */
/*   Updated: 2023/11/01 14:12:27 by jhogonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	execute(t_shell *shell)
{
	t_pipes		*pipes;
	int			function;
	const char	*builtin[3];

	builtin[0] = "pwd";
	builtin[1] = "cd";
	builtin[2] = "echo";
	function = -1;
	pipes = shell->pipes;
	while (++function < 3)
	{	
		if (ft_strncmp(pipes->cmds[0], builtin[function], \
		ft_strlen(builtin[function])) == 0
			&& ft_strlen(pipes->cmds[0]) == ft_strlen(builtin[function]))
			break ;
		if (ft_strncmp(pipes->cmds[0], "exit", 4) == 0 \
			&& ft_strlen(pipes->cmds[0]) == 4)
			free_struct(shell, 1);
		if (ft_strncmp(pipes->cmds[0], "unset", 5) == 0 \
			&& ft_strlen(pipes->cmds[0]) == 5)
			ft_unset(shell, pipes->cmds[1]);
		if (ft_strncmp(pipes->cmds[0], "export", 6) == 0 \
			&& ft_strlen(pipes->cmds[0]) == 6)
			print_list(shell, 0);
		if (ft_strncmp(pipes->cmds[0], "env", 3) == 0 \
		&& ft_strlen(pipes->cmds[0]) == 3)
			print_list(shell, 1);
	}
	if (function < 3)
		shell->builtin[function](shell);
}

