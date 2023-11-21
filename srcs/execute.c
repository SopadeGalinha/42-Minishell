#include "../includes/minishell.h"

void redirect_stdin(int source_fd)
{
	if (dup2(source_fd, STDIN_FILENO) == -1)
	{
		perror("minishell");
		exit(EXIT_FAILURE);
	}
	close(source_fd);
}

void redirect_stdout(int dest_fd)
{
	if (dup2(dest_fd, STDOUT_FILENO) == -1)
	{
		perror("minishell");
		exit(EXIT_FAILURE);
	}
	close(dest_fd);
}

void setup_redirection(t_pipes *pipes, int prev_pipe[2], t_shell *shell)
{
    if (prev_pipe[0] != -1)
        redirect_stdin(prev_pipe[0]);

    if (pipes->redir_in != NULL)
    {
        if (pipes->fd[IN] == -1)
        {
            perror("minishell");
            exit(EXIT_FAILURE);
        }
        if (dup2(pipes->fd[IN], STDIN_FILENO) == -1)
        {
            perror("minishell");
            exit(EXIT_FAILURE);
        }
        close(pipes->fd[IN]);
    }

    if (pipes->fd[OUT] != shell->std_out)
        redirect_stdout(shell->std_out);  // Use shell->std_out instead of pipes->fd[OUT]

    if (pipes->redir_out != NULL)
    {
        if (pipes->fd[OUT] == -1)
        {
            perror("minishell");
            exit(EXIT_FAILURE);
        }
        if (dup2(pipes->fd[OUT], STDOUT_FILENO) == -1)
        {
            perror("minishell");
            exit(EXIT_FAILURE);
        }
        close(pipes->fd[OUT]);
    }
}

void execute_cmd(t_shell *shell, t_pipes *pipes, int prev_pipe[2])
{
    char **envpp;
    envpp = get_envp_array(shell);

    setup_redirection(pipes, prev_pipe, shell);
    if (pipes->cmds[0][0] == '/')
        if (execve(pipes->cmds[0], pipes->cmds, envpp) == -1)
        {
            perror("minishell");
            exit(EXIT_FAILURE);
        }
    ft_free_array(envpp);
}

void execute_pipeline(t_shell *shell)
{
    t_pipes *pipes;
    const char *builtin[7];
    int is_builtin;
    int prev_pipe[2] = { -1, -1 };
	int	fd[2];

    pipes = shell->pipes;
    init_builtin(builtin);
    while (pipes)
    {
        if (pipes->next)
        {
            if (pipe(fd) == -1)  // Use shell->std_in for the pipe
            {
                perror("pipe");
                exit(EXIT_FAILURE);
            }
        }
		shell->std_in = fd[1];
		shell->std_in = fd[0];

        is_builtin = ft_is_builtin(builtin, pipes->cmds[0]);
        if (is_builtin != -1)
            shell->builtin[is_builtin](shell, pipes);
        else
            execute_cmd(shell, pipes, prev_pipe);

        if (pipes->next)
        {
            // Close the previous pipe after the command has been executed
            close(prev_pipe[0]);
            close(prev_pipe[1]);

            // Set the current pipe as the previous pipe for the next iteration
            prev_pipe[0] = shell->std_in;
            prev_pipe[1] = shell->std_out;
        }

        pipes = pipes->next;
    }
}
