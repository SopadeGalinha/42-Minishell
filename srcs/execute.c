/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhogonca <jhogonca@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/09 18:18:41 by jhogonca          #+#    #+#             */
/*   Updated: 2023/11/01 08:38:26 by jhogonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_pwd(t_shell *shell)
{
	char	*pwd;

	(void)shell;
	pwd = getcwd(NULL, 0);
	ft_printf_fd(shell->stdout, "%s\n", pwd);
	free(pwd);
}

void	ft_cd(t_shell *shell)
{
	char	*param;
	char	*pwd;

	param = shell->pipes->cmds[1];
	if (param == NULL)
	{
		pwd = getcwd(NULL, 0);
		ft_printf_fd(1, "%s\n", pwd);
		free(pwd);
	}
	else if (chdir(param) == -1)
	{
		ft_printf_fd(2, "cd: %s: %s\n", param, strerror(errno));
		g_exit_status = 1;
	}
}

void	ft_echo(t_shell *shell)
{
	int		i;
	int		n_flag;

	i = 0;
	n_flag = 0;
	while (shell->pipes->cmds[++i]
		&& ft_strncmp(shell->pipes->cmds[i], "-n", 2) == 0)
		n_flag = 1;
	while (shell->pipes->cmds[i])
	{
		ft_printf_fd(1, "%s", shell->pipes->cmds[i]);
		if (shell->pipes->cmds[i + 1])
			ft_printf_fd(shell->stdout, " ");
		i++;
	}
	if (!n_flag)
		ft_printf_fd(shell->stdout, "\n");
}

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
	}
	if (function < 3)
		shell->builtin[function](shell);
}
