#include "../includes/minishell.h"

// CODE VAULT

int ft_error(char *str, int exit_code)
{
	perror(str);
	return (exit_code);
}

void	signal_hdl(int sig)
{
	if (sig == SIGINT)
	{
		ft_printf_fd(STDOUT_FILENO, "\n");
		return ;
	}
}

// handle sigint and sigquit signals
void	exec_signal_handler(void)
{
	signal(SIGINT, signal_hdl);
	signal(SIGQUIT, SIG_IGN);
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

// Execute the child process with execve
void	child_process(t_shell *shell, t_pipes *pipes_lst, int **pipes, int process_num, int i)
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
int	**get_pipes(int process_num)
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

void	execute(t_shell *shell)
{
	int			i;
	int			is_builtin;
	int 		process_num;
	t_pipes		*pipes_lst;
	const char	*builtin[7];

	i = 0;
	pipes_lst = shell->pipes;
	
	// Count the number of processes in the pipeline
	process_num = count_pipes(shell->tokens) + 1;

	// Initialize the builtin array
	init_builtin(builtin);

	// Create pipes for all processes in the pipeline
	// ex: ls | cat | wc -l -> 2 pipes
	shell->pipes_fd = get_pipes(process_num);
	
	if (!shell->pipes_fd)
	{
		perror("Error with creating pipes");
		return ;
	}
	i = 0;
	while (pipes_lst)
	{
		// handle signals
		exec_signal_handler();
		is_builtin = ft_is_builtin(builtin, pipes_lst->cmds[0]);
		
		// îf it's a builtin, execute it - no need to fork
		if (is_builtin != -1)
			shell->builtin[is_builtin](shell, pipes_lst);
		else
		{
			// fork a child process
			shell->pid = fork();
			if (shell->pid == -1)
			{
				perror("Error with creating process");
				return ;
			}
		}
		// if it's not a builtin, execute it - forked process
		if (is_builtin == -1)
		{
			// if it's a child process, execute it
			if (shell->pid == 0)
			{
				get_redirections(i, shell->pipes_fd, pipes_lst);
				child_process(shell, pipes_lst, shell->pipes_fd, process_num, i);
			}
			i++;
		}
		pipes_lst = pipes_lst->next;
	}
	i = -1;
	// Close all pipes in the parent process
	while (++i < process_num)
	{
		close(shell->pipes_fd[i][0]);
		close(shell->pipes_fd[i][1]);
	}
	i = -1;
	// Wait for all child processes to finish
	while (++i < process_num)
		wait(NULL);
}
