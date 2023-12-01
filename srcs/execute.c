#include "../includes/minishell.h"

int ft_error(char *str, int exit_code)
{
	perror(str);
	return (exit_code);
}

static void	get_redirections(int pos, int **pipes, t_pipes *pipes_lst, int process_num, t_shell *shell)
{
	int	i;

	i = -1;
	if (pos != 0)
	{
		dup2(pipes[pos - 1][READ_END], STDIN_FILENO);
		close(pipes[pos - 1][READ_END]);
		close(pipes[pos - 1][WRITE_END]);
	}
	if (pipes_lst->next)
	{
		dup2(pipes[pos][WRITE_END], STDOUT_FILENO);
		close(pipes[pos][READ_END]);
		close(pipes[pos][WRITE_END]);
	}
	if (pipes_lst->redir_in)
	{
		if (pipes_lst->redir_fd[IN] == -1)
		{
			perror("Error opening file for input redirection");
			exit(EXIT_FAILURE);
		}
		if (pipes_lst->redir_fd[IN] == -2 && shell->heredoc)
		{
			write(pipes[pos][WRITE_END], shell->heredoc, strlen(shell->heredoc));
			close(pipes[pos][WRITE_END]);
		}
		else
		{
			dup2(pipes_lst->redir_fd[IN], STDIN_FILENO);
			close(pipes_lst->redir_fd[IN]);
		}
	}
	if (pipes_lst->redir_out)
	{
		dup2(pipes_lst->redir_fd[OUT], STDOUT_FILENO);
		close(pipes_lst->redir_fd[OUT]);
	}
	for (i = 0; i < process_num - 1; ++i)
	{
		if (i != pos && i != pos - 1)
		{
			close(pipes[i][READ_END]);
			close(pipes[i][WRITE_END]);
		}
	}
}

int	**create_pipes(int process_num)
{
	int	**pipes;

	int	i;

	if (process_num <= 1)
		return (NULL);
	pipes = malloc(sizeof(int *) * process_num);
	if (!pipes)
		return (NULL);
	i = -1;
	while (++i < process_num)
	{
		pipes[i] = malloc(sizeof(int) * 2);
		if (!pipes[i])
			return (NULL);
	}
	i = -1;
	while (++i < process_num)
	{
		if (pipe(pipes[i]) == -1)
			return (NULL);
	}
	return (pipes);
}

void close_pipes(int **pipes, int process_num)
{
	int pos;

	
	for (pos = 0; pos < process_num - 1; ++pos)
	{
		close(pipes[pos][READ_END]);
		close(pipes[pos][WRITE_END]);
	}
}

void	ft_execve(t_shell *shell, t_pipes *pipes_lst)
{
	char	**envp;

	envp = get_envp_array(shell);
	if (execve(pipes_lst->cmds[0], pipes_lst->cmds, envp) == -1)
	{
		ft_printf_fd(2, MS_ERR"%s: %s\n", pipes_lst->cmds[0], strerror(errno));
		ft_free_2d_array((void **)envp, 0);
		g_exit_status = 127;
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
	{
		pipes_lst->pid = fork();
		if (pipes_lst->pid == -1)
		{
			perror("Error with creating process");
			g_exit_status = 1;
			return (EXIT_FAILURE);
		}
		if (pipes_lst->pid == 0)
		{
			if (pipes_lst->redir_out)
			{
				dup2(pipes_lst->redir_fd[OUT], STDOUT_FILENO);
				close(pipes_lst->redir_fd[OUT]);
			}
			if (pipes_lst->redir_in)
			{
				if (pipes_lst->redir_fd[IN] == -1)
				{
					perror("Error opening file for input redirection");
					exit(EXIT_FAILURE);
				}
				else
				{
					dup2(pipes_lst->redir_fd[IN], STDIN_FILENO);
					close(pipes_lst->redir_fd[IN]);
				}
			}
			ft_execve(shell, pipes_lst);
		}
		else
		{
			if (waitpid(pipes_lst->pid, &g_exit_status, 0) == -1)
				ft_error("Error waiting for process", 1);
			if (WIFEXITED(g_exit_status))
				g_exit_status = WEXITSTATUS(g_exit_status);
			else if (WIFSIGNALED(g_exit_status))
				g_exit_status = WTERMSIG(g_exit_status) + 128;
			g_exit_status = WEXITSTATUS(g_exit_status);
		}
	}
	return (g_exit_status);
}

int	ft_multiple_cmds(t_shell *shell, t_pipes *pipes_lst, int process_num, const char **builtin)
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
		{
			get_redirections(i, shell->pipes_fd, pipes_lst, process_num, shell);
			if (is_builtin != -1)
			{
				shell->builtin[is_builtin](shell, pipes_lst);
				exit(g_exit_status);
			}
			else
				ft_execve(shell, pipes_lst);
		}
		pipes_lst = pipes_lst->next;
	}
	return (0);
}

void	waiting(int process_num, t_shell *shell)
{
	int	i;

	i = -1;
	close_pipes(shell->pipes_fd, process_num);
	while (++i < process_num)
	{
		if (waitpid(-1, &g_exit_status, 0) == -1)
			ft_error("Error waiting for process", 1);
		if (WIFEXITED(g_exit_status))
			g_exit_status = WEXITSTATUS(g_exit_status);
		else if (WIFSIGNALED(g_exit_status))
			g_exit_status = WTERMSIG(g_exit_status) + 128;
		g_exit_status = WEXITSTATUS(g_exit_status);
	}
}

int	execute(t_shell *shell)
{
	int 		process_num;
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
