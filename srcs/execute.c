#include "../includes/minishell.h"

// CODE VAULT

int ft_error(char *str, int exit_code)
{
	perror(str);
	return (exit_code);
}

static void	get_redirections(int pos, int **pipes, t_pipes *pipes_lst)
{
	int	i;

	i = -1;
	// Redirect input for all processes except the first one
	if (pos != 0)
	{
		dup2(pipes[pos - 1][0], STDIN_FILENO);
		close(pipes[pos - 1][0]);
		close(pipes[pos - 1][1]);
	}
	// Redirect output for all processes except the last one
	if (pipes_lst->next)
	{
		dup2(pipes[pos][1], STDOUT_FILENO);
		close(pipes[pos][0]);
		close(pipes[pos][1]);
	}
	// Close all pipes in the child process that are not needed
	while (++i < pos)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
	}
}

void	ft_execve(t_shell *shell, t_pipes *pipes_lst, int **pipes, int process_num, int i)
{
	char	**envp;

	envp = get_envp_array(shell);
	if (execve(pipes_lst->cmds[0], pipes_lst->cmds, envp) == -1)
	{
		perror("Error executing command");
		exit(1);
	}
}
// Return an array of pipes for all processes
int	**create_pipes(int process_num)
{
	int	**pipes;
	int	i;

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

void	close_pipes(int **pipes, int process_num)
{
	int	i;

	i = -1;
	while (++i < process_num)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
	}
	i = -1;
	while (++i < process_num)
		wait(NULL);
}

int	execute(t_shell *shell)
{
	int			i;
	int			is_builtin;
	int 		process_num;
	t_pipes		*pipes_lst;
	const char	*builtin[7];

	i = 0;
	pipes_lst = shell->pipes;
	process_num = count_pipes(shell->tokens) + 1;

	init_builtin(builtin);
	shell->pipes_fd = create_pipes(process_num);
	if (!shell->pipes_fd)
		return (ft_error("Error creating pipes", 1));
	i = -1;
	while (pipes_lst && ++i != -2)
	{
		exec_signal_handler();
		is_builtin = ft_is_builtin(builtin, pipes_lst->cmds[0]);
		if (is_builtin != -1)
			shell->builtin[is_builtin](shell, pipes_lst);
		else
		{
			shell->pid = fork();
			if (shell->pid == -1)
			{
				perror("Error with creating process");
				return (1);
			}
		}
		if (is_builtin == -1)
		{
			if (shell->pid == 0)
			{
				get_redirections(i, shell->pipes_fd, pipes_lst);
				ft_execve(shell, pipes_lst, shell->pipes_fd, process_num, i);
			}
		}
		pipes_lst = pipes_lst->next;
	}
	close_pipes(shell->pipes_fd, process_num);
	return (0);
}
