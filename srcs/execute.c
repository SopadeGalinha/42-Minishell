/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhogonca <jhogonca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/03 21:27:48 by jhogonca          #+#    #+#             */
/*   Updated: 2023/12/04 22:26:11 by jhogonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_execve(t_shell *shell, t_pipes *pipes_lst)
{
	char	**envp;

	if (!pipes_lst->cmds[0] || !pipes_lst->cmds)
	{
		free_struct(shell, 1);
		exit (0);
	}
	envp = get_envp_array(shell);
	if (execve(pipes_lst->cmds[0], pipes_lst->cmds, envp) == -1)
	{
		ft_printf_fd(2, MS_ERR RESET"%s: %s\n", \
		pipes_lst->cmds[0], strerror(errno));
		ft_free_2d_array((void **)envp, 0);
		g_exit_status = 127;
		free_struct(shell, 1);
		exit(g_exit_status);
	}
	ft_free_2d_array((void **)envp, 0);
	g_exit_status = WEXITSTATUS(g_exit_status);
	exit (g_exit_status);
}

int	ft_single_cmd(t_shell *shell, t_pipes *pipes_lst)
{
	int			is_builtin;
	const char	*builtin[7];
	int			stdout_backup;

	init_builtin(builtin);
	is_builtin = ft_is_builtin(builtin, pipes_lst->cmds[0]);
	if (is_builtin != -1)
	{
		if (pipes_lst->redir_out)
		{
			stdout_backup = dup(STDOUT_FILENO);
			dup2(pipes_lst->redir_fd[OUT], STDOUT_FILENO);
			close(pipes_lst->redir_fd[OUT]);
		}
		shell->builtin[is_builtin](shell, pipes_lst);
		if (pipes_lst->redir_out)
		{
			dup2(stdout_backup, STDOUT_FILENO);
			close(stdout_backup);
		}
	}
	else
		single_cmd_aux(shell, pipes_lst);
	return (g_exit_status);
}

void	ft_multiple_cmds_child(int i, t_shell *shell, \
t_pipes *pipes_lst, int process_num)
{
	int			is_builtin;
	const char	*builtin[7];

	init_builtin(builtin);
	is_builtin = ft_is_builtin(builtin, pipes_lst->cmds[0]);
	signals_child();
	get_redirections(i, shell->pipes_fd, pipes_lst, shell);
	close_redirections(pipes_lst, process_num, shell->pipes_fd, i);
	if (is_builtin != -1)
	{
		shell->builtin[is_builtin](shell, pipes_lst);
		free_struct(shell, 1);
		exit(g_exit_status);
	}
	else
		ft_execve(shell, pipes_lst);
}

int	ft_multiple_cmds(t_shell *shell, t_pipes *pipes_lst, \
int process_num, const char **builtin)
{
	int	i;
	int	is_builtin;

	i = -1;
	while (pipes_lst && ++i != -2)
	{
		pipes_lst->pid = fork();
		is_builtin = ft_is_builtin(builtin, pipes_lst->cmds[0]);
		if (pipes_lst->pid == -1)
			return (ft_error("Error creating process", 1));
		if (pipes_lst->pid == 0)
			ft_multiple_cmds_child(i, shell, pipes_lst, process_num);
		else
			signals_wait();
		pipes_lst = pipes_lst->next;
	}
	return (0);
}

int	execute(t_shell *shell)
{
	int			process_num;
	t_pipes		*pipes_lst;
	const char	*builtin[7];

	pipes_lst = shell->pipes;
	init_builtin(builtin);
	if (!pipes_lst->cmds || !pipes_lst->cmds[0])
	{
		g_exit_status = 0;
		return (ft_printf_fd(STDOUT_FILENO, "\n"));
	}
	process_num = count_pipes(shell->tokens) + 1;
	shell->pipes_fd = create_pipes(process_num);
	if (!shell->pipes_fd && process_num > 1)
		return (ft_error("Error creating pipes", 1));
	if (!pipes_lst->next)
		return (ft_single_cmd(shell, pipes_lst));
	else
		ft_multiple_cmds(shell, pipes_lst, process_num, builtin);
	waiting(process_num, shell);
	return (g_exit_status);
}
