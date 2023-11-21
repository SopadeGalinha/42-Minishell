#include "../includes/minishell.h"

void redirect_stdin(int source_fd) {
    if (dup2(source_fd, STDIN_FILENO) == -1) {
        perror("minishell");
        exit(EXIT_FAILURE);
    }
    close(source_fd);
}

void redirect_stdout(int dest_fd) {
    if (dup2(dest_fd, STDOUT_FILENO) == -1) {
        perror("minishell");
        exit(EXIT_FAILURE);
    }
    close(dest_fd);
}

void setup_redirection(t_pipes *pipes, int prev_pipe[2], t_shell *shell) {
    if (prev_pipe[0] != -1)
        redirect_stdin(prev_pipe[0]);

    if (pipes->redir_in != NULL) {
        if (pipes->fd[IN] == -1) {
            perror("minishell");
            exit(EXIT_FAILURE);
        }
        if (dup2(pipes->fd[IN], STDIN_FILENO) == -1) {
            perror("minishell");
            exit(EXIT_FAILURE);
        }
        close(pipes->fd[IN]);
    }

    if (pipes->fd[OUT] != shell->std_out)
        redirect_stdout(shell->std_out);

    if (pipes->redir_out != NULL) {
        if (pipes->fd[OUT] == -1) {
            perror("minishell");
            exit(EXIT_FAILURE);
        }
        if (dup2(pipes->fd[OUT], STDOUT_FILENO) == -1) {
            perror("minishell");
            exit(EXIT_FAILURE);
        }
        close(pipes->fd[OUT]);
    }
}

void execute_cmd(t_shell *shell, t_pipes *pipes, int prev_pipe[2]) {
    char **envpp;
    envpp = get_envp_array(shell);

    setup_redirection(pipes, prev_pipe, shell);
    if (pipes->cmds[0][0] == '/')
        if (execve(pipes->cmds[0], pipes->cmds, envpp) == -1) {
            perror("minishell");
            exit(EXIT_FAILURE);
        }
    ft_free_array(envpp);
}

void execute_pipeline(t_shell *shell) {
    t_pipes *pipes;
    const char *builtin[7];
    int is_builtin;
    int prev_pipe[2] = { -1, -1 };
    int fd[2];

    pipes = shell->pipes;
    init_builtin(builtin);
    fd[0] = -1;
    fd[1] = -1;

    while (pipes) {
        if (pipes->next) {
            if (pipe(fd) == -1) {
                perror("pipe");
                exit(EXIT_FAILURE);
            }
        }

        shell->std_in = fd[0];
        shell->std_out = fd[1];

        is_builtin = ft_is_builtin(builtin, pipes->cmds[0]);
        if (is_builtin != -1)
            shell->builtin[is_builtin](shell, pipes);
        else
            execute_cmd(shell, pipes, prev_pipe);

        if (pipes->next) {
            close(prev_pipe[0]);
            close(prev_pipe[1]);
            prev_pipe[0] = fd[0];
            prev_pipe[1] = fd[1];
        }

        pipes = pipes->next;
    }

    // Close the last set of file descriptors
    close(prev_pipe[0]);
    close(prev_pipe[1]);
}
