/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heolivei <heolivei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/09 18:18:41 by jhogonca          #+#    #+#             */
/*   Updated: 2023/11/10 12:59:34 by heolivei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"


bool	ft_access(t_shell *shell)
{
	t_pipes	*tmp;
	char	**array;
	int		i;
	char	*path;

	i = -1;
	tmp = shell->pipes;
	path = get_env_value(shell->env, "PATH");
	array = ft_split(path, ':');
	while (array[++i])
	{
		free(path);
		path = ft_strjoin(array[i], "/");
		path = ft_strjoin(path, tmp->cmds[0]);
		if (access(path, F_OK) == 0)
		{
			tmp->path = ft_strdup(path);
			free(path);
			break ;
		}
	}
	execve(tmp->path, tmp->cmds, NULL);
	return (true);
}

void setup_redirections(t_pipes *pipes)
{
	int in_fd;
	int out_fd;

	while (pipes->redir_in)
	{
		in_fd = open(pipes->redir_in->file, O_RDONLY);
		if (in_fd == -1)
		{
			ft_printf_fd(2, "minishell: cannot open %s: %s\n", pipes->redir_in->file, strerror(errno));
			g_exit_status = 1;
			// exit(1);
		}
		if (dup2(in_fd, 0) == -1)
		{
			ft_printf_fd(2, "minishell: dup2 failed: %s\n", strerror(errno));
			g_exit_status = 1;
			// exit(1);
		}
		close(in_fd);
		pipes->redir_in = pipes->redir_in->next;
	}

	while (pipes->redir_out)
	{
		if (pipes->redir_out->type == D_REDIR_OUT)
			out_fd = open(pipes->redir_out->file, O_WRONLY | O_APPEND | O_CREAT, 0644);
		else
			out_fd = open(pipes->redir_out->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (out_fd == -1)
		{
			ft_printf_fd(2, "minishell: cannot open %s: %s\n", pipes->redir_out->file, strerror(errno));
			g_exit_status = 1;
			// exit(1);
		}
		if (dup2(out_fd, 1) == -1)
		{
			ft_printf_fd(2, "minishell: dup2 failed: %s\n", strerror(errno));
			g_exit_status = 1;
			// exit(1);
		}
		close(out_fd);
		pipes->redir_out = pipes->redir_out->next;
	}
}

void ft_execve(t_shell *shell, t_pipes *pipes)
{
	pid_t pid;
	int status;

	pid = fork();
	if (pid == 0)
	{
		setup_redirections(pipes); // Set up redirections
		if (ft_strchr(pipes->cmds[0], '/'))
		{
			if (access(pipes->cmds[0], F_OK) == 0)
				execve(pipes->cmds[0], pipes->cmds, NULL);
			else
			{
				ft_printf_fd(2, "minishell: %s: %s\n", pipes->cmds[0], strerror(errno));
				g_exit_status = 127;
				// exit(127);
			}
		}
		else
		{
			if (ft_access(shell))
			{
				ft_printf_fd(2, "minishell: %s: command not found\n", pipes->cmds[0]);
				g_exit_status = 127;
				// exit(127);
			}
		}
	}
	else if (pid < 0)
	{
		ft_printf_fd(2, "minishell: %s\n", strerror(errno));
		g_exit_status = 1;
		// exit(1);
	}
	else
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			g_exit_status = WEXITSTATUS(status);
	}
}

int	ft_pipelstsize(t_pipes *lst)
{
	int	i;

	i = 0;
	if (!lst)
		return (0);
	while (lst)
	{
		lst = lst->next;
		i++;
	}
	return (i);
}



void	execution(t_shell *shell, const char **builtins_array)
{
	t_pipes	*pipes;
	int		function;

	pipes = shell->pipes;
	if (pipes->cmds[0] == NULL)
		return ;
	function = -1;

	while (++function < 7)
	{
		if (ft_strncmp(pipes->cmds[0], builtins_array[function], \
		ft_strlen(builtins_array[function])) == 0
			&& ft_strlen(pipes->cmds[0]) == ft_strlen(builtins_array[function]))
			break ;
	}
	if (function < 7)
		shell->builtin[function](shell, pipes);
	else
		ft_execve(shell, pipes);
}

void	close_write_read(int *pipes)
{
	close(pipes[0]);
	close(pipes[1]);
}

void	parent(int fd_in, int count_pipes)
{
	int	i;

	i = 0;
	close(fd_in);
	while (i < count_pipes)
	{
		wait(NULL);
		i++;
	}
}

void	children(t_shell *shell, int fd_in, int count_pipes)
{
	int		i;
	int		pipes[2];
	pid_t	pid;

	i = 0;
	while (i < count_pipes)
	{
		pipe(pipes);
		pid = fork();
		if (pid == 0)
		{
			if (i != 0)
				dup2(fd_in, STDIN_FILENO);
			if ((i + 1) != count_pipes)
				dup2(pipes[1], STDOUT_FILENO);
			close_write_read(pipes);
			close(fd_in);
			execute(shell);
			exit(g_exit_status);
		}
		dup2(pipes[0], fd_in);
		close_write_read(pipes);
		shell->pipes = shell->pipes->next;
		i++;
	}
}

void	open_pipe(t_shell *shell)
{
	int		fd_in;

	int		count_pipes;

	count_pipes = ft_pipelstsize(shell->pipes);
	if (count_pipes > 1)
	{
		fd_in = dup(STDIN_FILENO);
		children(shell, fd_in, count_pipes);
		parent(fd_in, count_pipes);
	}
	else
		execute(shell);
}
void	execute(t_shell *shell)
{
	const char	*builtin[7];

	builtin[0] = "pwd";
	builtin[1] = "cd";
	builtin[2] = "echo";
	builtin[3] = "export";
	builtin[4] = "exit";
	builtin[5] = "unset";
	builtin[6] = "env";
	//ft_cd(shell, shell->pipes);

	execution(shell, builtin);
}