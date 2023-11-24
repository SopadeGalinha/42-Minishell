#include "../includes/minishell.h"

// CODE VAULT

void	child_process(t_shell *shell, t_pipes *pipes_lst, int pipes[][2], int process_num, int i)
{
	int	j;
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
	j = -1;
	while (++j < process_num)
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

int ft_error(char *str, int exit_code)
{
	perror(str);
	return (exit_code);
}
void	execute(t_shell *shell)
{
	int			i;
	int			is_builtin;
	int 		process_num;
	t_pipes		*pipes_lst;
	const char	*builtin[7];
	pid_t		pid;

	// create a function that will initialize the pipes later
	process_num = count_pipes(shell->tokens) + 1;
	int			pipes[process_num][2];

	i = 0;
	init_builtin(builtin);
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
		is_builtin = ft_is_builtin(builtin, pipes_lst->cmds[0]);
		if (is_builtin != -1)
			shell->builtin[is_builtin](shell, pipes_lst);
		else
		{
			pid = fork();
			if (pid == -1)
			{
				perror("Error with creating process");
				return ;
			}
		}
		if (is_builtin == -1)
		{
			if (pid == 0)
				child_process(shell, pipes_lst, pipes, process_num, i);
			i++;
		}
		pipes_lst = pipes_lst->next;
	}
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
