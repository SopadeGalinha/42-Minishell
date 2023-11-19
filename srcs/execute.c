
#include "../includes/minishell.h"

/*------------------------------INICIO---------CRIA O ENVPP PARA USAR NO EXECVE------INICIO--------------------------------------*/

int ft_envsize(t_env *lst)
{
    int i = 0;
    while (lst)
    {
        lst = lst->next;
        i++;
    }
    return i;
}

// Função para criar um array a partir da informação de line em t_env
char **create_envpp(t_shell *shell)
{
    int envArraySize = ft_envsize(shell->env);
    char **envArray = (char **)malloc((envArraySize + 1) * sizeof(char *));
    int index = 0;

    t_env *currentEnv = shell->env;
    while (currentEnv != NULL)
    {
        if (index >= envArraySize)
        {
            fprintf(stderr, "Tamanho máximo do array de ambiente atingido.\n");
            break;
        }

        envArray[index] = strdup(currentEnv->line);
        currentEnv = currentEnv->next;
        index++;
    }

    envArray[index] = NULL;

    return (envArray);
}


/*----------------------------------FIM------CRIA O ENVPP PARA USAR NO EXECVE-------FIM------------------------------------------*/

void	init_builtin(const char *builtin[7])
{
	builtin[0] = "pwd";
	builtin[1] = "cd";
	builtin[2] = "echo";
	builtin[3] = "export";
	builtin[4] = "exit";
	builtin[5] = "unset";
	builtin[6] = "env";
	//builtin[7] = NULL;
}

int	ft_is_builtin(const char *builtin[7], char *cmd)
{
	int	i;

	i = -1;
	while (cmd && ++i < 7)
		if (ft_strncmp(builtin[i], cmd, ft_strlen(builtin[i])) == 0)
			return (i);
	return (-1);
}

void execute_cmd(t_shell *shell, t_pipes *pipes, int prev_pipe[2])
{
	pid_t	pid;
	int		status;
	int		out_fd;
	char	**envpp;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	if (pid == 0)
	{
		envpp = create_envpp(shell);
		if (prev_pipe[0] != -1)
		{
			if (dup2(prev_pipe[0], STDIN_FILENO) == -1)
			{
				perror("dup2");
				exit(EXIT_FAILURE);
			}
			close(prev_pipe[0]);
		}
		// handle command-specific input redirection
		if (pipes->redir_in != NULL)
		{
			int in_fd = pipes->in;
			if (in_fd == -1)
			{
				perror("open");
				exit(EXIT_FAILURE);
			}
			if (dup2(in_fd, STDIN_FILENO) == -1) {
				perror("dup2");
				exit(EXIT_FAILURE);
			}
			close(in_fd);
		}
		// handle pipe output redirection
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
		if (execve(pipes->cmds[0], pipes->cmds, envpp) == -1)
		{
			perror("execve");
			exit(EXIT_FAILURE);
		}
		ft_free_array(envpp);
	}
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
	t_pipes		*pipes;
	const char	*builtin[7];
	int			is_builtin;
	int			prev_pipe[2];

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
		if (is_builtin != -1)
			shell->builtin[is_builtin](shell, pipes);
		else
			execute_cmd(shell, pipes, prev_pipe);
		if (pipes->next)
		{
			close(pipes->fd[OUT]);
			prev_pipe[0] = pipes->fd[IN];
			prev_pipe[1] = pipes->fd[OUT];
		}
		pipes = pipes->next;
	}
	if (prev_pipe[1] != -1)
		close(prev_pipe[1]);
}
