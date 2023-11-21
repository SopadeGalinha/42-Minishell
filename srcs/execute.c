
#include "../includes/minishell.h"

void setup_redirection(t_pipes *pipes, int prev_pipe[2], t_shell *shell)
{
	int	out_fd;
	if (prev_pipe[0] != -1)
	{
		if (dup2(prev_pipe[0], STDIN_FILENO) == -1)
		{
			perror("dup2");
			exit(EXIT_FAILURE);
		}
		close(prev_pipe[0]);
	}
	if (pipes->redir_in != NULL)
	{
		int in_fd = pipes->in;
		if (in_fd == -1)
		{
			perror("open");
			exit(EXIT_FAILURE);
		}
		if (dup2(in_fd, STDIN_FILENO) == -1)
		{
			perror("dup2");
			exit(EXIT_FAILURE);
		}
		close(in_fd);
	}
	if (pipes->fd[OUT] != shell->std_out)
	{
		if (dup2(pipes->fd[OUT], STDOUT_FILENO) == -1)
		{
			perror("dup2");
			exit(EXIT_FAILURE);
		}
		close(pipes->fd[OUT]);
	}
	if (pipes->redir_out != NULL)
	{
		out_fd = pipes->out;
		if (out_fd == -1)
		{
			perror("open");
			exit(EXIT_FAILURE);
		}
		if (dup2(out_fd, STDOUT_FILENO) == -1)
		{
			perror("dup2");
			exit(EXIT_FAILURE);
		}
		close(out_fd);
	}
}

void child_process(t_shell *shell, t_pipes *pipes, int prev_pipe[2])
{
	char **envp;

	envp = get_envp_array(shell);
	setup_redirection(pipes, prev_pipe, shell);
	if (execve(pipes->cmds[0], pipes->cmds, envp) == -1)
	{
		perror("execve");
		exit(EXIT_FAILURE);
	}
	ft_free_array(envp);
}

void execute_cmd(t_shell *shell, t_pipes *pipes, int prev_pipe[2])
{
	pid_t	pid;
	int		status;
	char	**envpp;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	if (pid == 0)
		child_process(shell, pipes, prev_pipe);
	else
	{
		if (waitpid(pid, &status, 0) == -1)
		{
			perror("waitpid");
			exit(EXIT_FAILURE);
		}
		if (WIFEXITED(status))
			g_exit_status = WEXITSTATUS(status);
	}
}


void execute_pipeline(t_shell *shell)
{
	t_pipes     *pipes;
	const char  *builtin[7];
	int         is_builtin;
	int         prev_pipe[2];

	pipes = shell->pipes;
	init_builtin(builtin);
	prev_pipe[0] = -1;
	prev_pipe[1] = -1;

	while (pipes)
	{
		if (pipes->fd[OUT] == -1)
			pipes->fd[OUT] = shell->std_out;

		if (pipes->next)
		{
			if (pipe(pipes->fd) == -1)
			{
				perror("pipe");
				exit(EXIT_FAILURE);
			}
		}

		is_builtin = ft_is_builtin(builtin, pipes->cmds[0]);

		pid_t pid = fork();
		if (pid == -1)
		{
			perror("fork");
			exit(EXIT_FAILURE);
		}
		else if (pid == 0)
		{
			if (is_builtin != -1)
			{
				shell->std_out = pipes->fd[OUT];
				shell->builtin[is_builtin](shell, pipes);
			}
			else
				execute_cmd(shell, pipes, prev_pipe);

			// Exit the child process
			exit(EXIT_SUCCESS);
		}

		// Parent process
		if (pipes->next)
		{
			close(pipes->fd[OUT]);
			prev_pipe[0] = pipes->fd[IN];
			prev_pipe[1] = pipes->fd[OUT];
		}

		pipes = pipes->next;
	}

	// Parent process waits for all child processes to finish
	while (wait(NULL) > 0)
		;
	if (prev_pipe[1] != -1)
		close(prev_pipe[1]);
}