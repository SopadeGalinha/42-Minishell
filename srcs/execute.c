#include "../includes/minishell.h"

/* void	process(t_shell *shell, int process, const char *builtin[7], int pid)
{
	t_pipes	*pipes;
	int		is_builtin;
	char	**envp;

	envp = get_envp_array(shell);
	pipes = shell->pipes;
	if (pid != 0)
	{
		while (process-- > 0)
			wait(NULL);
		return ;
	}
	while ( pipes)
	{
		if (pipe(pipes->pipe_fd) == -1)
		{
			perror("minishell");
			exit(1);
		}
		is_builtin = ft_is_builtin(pipes->cmds[0], builtin);
		if (is_builtin)
			ft_is_builtin(pipes->cmds[0], builtin);
		else
		{
			if (execve(pipes->cmds[0], pipes->cmds, envp) == -1)
			{
				perror("minishell");
				exit(1);
			}
		}
	}
}
	/* const char	*builtin[7];
	int			process_num;
	int			pid;

	init_builtin(builtin);
	process_num = count_pipes(shell->tokens) + 1;
	pid = fork();
	process(shell, process_num, builtin, pid);

	*/
// CODE VAULT START

int ft_error(char *str, int exit_code)
{
	perror(str);
	return (exit_code);
}
void	execute(t_shell *shell)
{
	t_pipes *pipes_lst;
	int i;
	int process_num;

	i = 0;
	process_num = count_pipes(shell->tokens) + 1;

	int pipes[process_num][2];
	pipes_lst = shell->pipes;
	// Create all pipes
	while (i < process_num)
	{
		if (pipe(pipes[i]) == -1)
		{
			perror("Error with creating pipe");
			return;
		}
		i++;
	}

	i = 0;
	while (pipes_lst)
	{
		pid_t pid = fork();

		if (pid == -1)
		{
			perror("Error with creating process");
			return;
		}

		if (pid == 0)
		{
			// Child process
			if (i != 0)
			{
				// Redirect input for all processes except the first one
				dup2(pipes[i - 1][0], STDIN_FILENO);
				close(pipes[i - 1][0]);
				close(pipes[i - 1][1]);
			}

			// Redirect output for all processes except the last one
			if (pipes_lst->next)
			{
				dup2(pipes[i][1], STDOUT_FILENO);
				close(pipes[i][0]);
				close(pipes[i][1]);
			}

			// Close all pipes in the child process
			int j;
			for (j = 0; j < process_num; j++)
			{
				close(pipes[j][0]);
				close(pipes[j][1]);
			}

			// Execute the command
			if (execve(pipes_lst->cmds[0], pipes_lst->cmds, NULL) == -1)
			{
				perror("Error executing command");
				return ;
			}
		}
		i++;
		pipes_lst = pipes_lst->next;
	}

	// Close all pipes in the parent process
	for (i = 0; i < process_num; i++)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
	}

	// Wait for all child processes to finish
	i = 0;
	while (i < process_num)
	{
		wait(NULL);
		i++;
	}
}
