/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhogonca <jhogonca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/09 18:18:41 by jhogonca          #+#    #+#             */
/*   Updated: 2023/11/06 18:39:36 by jhogonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	str_isalpha(char *str, int flag)
{
	int	i;

	i = 0;
	if (flag)
		i = 1;
	while (str[i])
	{
		if (!isalpha(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int	arg_checker(t_shell *shell, char *str)
{
	int	i = 0;

	if (ft_isdigit(str[0]))
	{
		ft_printf_fd(2, "minishell: export: `%s': not a valid identifier\n", str);
		return (-1);
	}
	else if (str[0] == '_' && str[1])
	{
		if ((!ft_isalnum(str[1]) && str[1] != '_') || str_isalpha(str, 1) == 0)
		{
			ft_printf_fd(2, "minishell: export: `%s': not a valid identifier\n", str);;
			return (-1);
		}
		else
			return (1);
	}
	else if (str[0] == '_' && (str[1] == '=' || str[1] == '\0'))
	{
		return (-2);
	}
	else if (str_isalpha(str, 0) == 0)
	{
		ft_printf_fd(2, "minishell: export: `%s': not a valid identifier\n", str);
		return (-1);
	}
	return (1);
}

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

void	execution(t_shell *shell, const char **builtins_array)
{
	t_pipes	*pipes;
	int		function;

	pipes = shell->pipes;
	if (pipes->cmds[0] == NULL)
		return ;
	function = -1;
	while (pipes)
	{
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
		pipes = pipes->next;
	}
	
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
	execution(shell, builtin);
}
