/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhogonca <jhogonca@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 17:41:43 by jhogonca          #+#    #+#             */
/*   Updated: 2024/04/08 19:38:51 by jhogonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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

char	**get_envp_array(t_shell *shell)
{
	char	**array;
	int		i;
	t_env	*current;

	i = -1;
	current = shell->env;
	array = malloc(sizeof(char *) * (ft_list_envsize(shell->env) + 1));
	if (!array)
		return (NULL);
	while (current != NULL)
	{
		array[++i] = ft_strdup(current->line);
		if (!array[i])
			return (NULL);
		current = current->next;
	}
	array[++i] = NULL;
	return (array);
}

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

bool	create_pipes(t_shell *shell)
{
	int	i;
	int	process_num;
	// verificar depois se o count_pipes está correto
	process_num = count_pipes(shell->tokens) + 1;
	if (process_num < 2)
		return false;
	shell->pipes_fd = malloc(sizeof(int *) * process_num);
	if (shell->pipes_fd)
	{
		i = -1;
		while (++i < process_num)
		{
			shell->pipes_fd[i] = malloc(sizeof(int) * 2);
			if (!shell->pipes_fd[i])
				return false;
			if (pipe(shell->pipes_fd[i]) == -1)
				return false;
		}
	}
	return true;
}

void	run(t_shell *shell, t_pipes *process, int index, const char **builtin)
{
	int	builtin_index;
	builtin_index = ft_is_builtin(builtin, process->cmds[0]);
	process->pid = fork();
	if (process->pid == 0)
	{
		get_redirections(index, shell->pipes_fd,process, shell);
		close_redirections(process, builtin_index, \
		shell->pipes_fd, index);
		if (builtin && builtin_index != -1)
		{
			shell->builtin[builtin_index](shell, process);
			free_struct(shell, 1);
			exit(g_exit_status);
		}
		else
			ft_execve(shell, process);
	}	
	else if(process->pid == -1)
		ft_error("Error creating process", 1);
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

void	waiting(int process_num, t_shell *shell)
{
	int	i;

	i = -1;
	close_pipes(shell->pipes_fd, process_num);
	while (++i < process_num)
	{
		if (/* process_num > 1 &&  */waitpid(-1, &g_exit_status, 0) == -1)
			ft_error("Error waiting for process", 1);
		if (WIFEXITED(g_exit_status))
			g_exit_status = WEXITSTATUS(g_exit_status);
		else if (WIFSIGNALED(g_exit_status))
			g_exit_status = WTERMSIG(g_exit_status) + 128;
	}
}

int	execute(t_shell *shell)
{
	int 	index;
	t_pipes	*process;
	const char	*builtin[7];

	index = -1;
	process = shell->pipes;
	init_builtin(builtin);
	if (!create_pipes(shell) && count_pipes(shell->tokens) > 0)
		return (ft_error("Error creating pipes", 1));
	while (process && ++index > -1)
	{
		if (ft_is_builtin(builtin, process->cmds[0]) != -1
			&& count_pipes(shell->tokens) == 0)
				shell->builtin[ft_is_builtin(builtin, \
				process->cmds[0])](shell, process);
		else
			run(shell, process, index, builtin);
		process = process->next;
	}
	waiting(count_pipes(shell->tokens) + 1, shell);
	return 0;
}
