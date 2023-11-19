
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

/*-----------refatora-------------*/

int handle_error(char *str)
{
	perror(str);
	return (EXIT_FAILURE);
}

void redirect_stdin(int source_fd)
{
	if (dup2(source_fd, STDIN_FILENO) == -1)
		exit (handle_error("dup2"));
	close(source_fd);
}

void redirect_stdout(int dest_fd)
{
	if (dup2(dest_fd, STDOUT_FILENO) == -1)
		exit (handle_error("dup2"));
	close(dest_fd);
}


void setup_redirection(t_pipes *pipes, int prev_pipe[2], t_shell *shell)
{
	int	out_fd;
	if (prev_pipe[0] != -1)
		redirect_stdin(prev_pipe[0]);
	if (pipes->redir_in != NULL)
	{
		int in_fd = pipes->in;
		if (in_fd == -1)
			exit (handle_error("open"));
		if (dup2(in_fd, STDIN_FILENO) == -1)
			exit (handle_error("dup2"));
		close(in_fd);
	}
	if (pipes->fd[OUT] != shell->std_out)
		redirect_stdout(pipes->fd[OUT]);
	if (pipes->redir_out != NULL)
	{
		out_fd = pipes->out;
		if (out_fd == -1)
			exit (handle_error("open"));
		if (dup2(out_fd, STDOUT_FILENO) == -1)
			exit (handle_error("dup2"));
		close(out_fd);
	}
}

void ft_access(char **cmd, t_shell *shell)
{
	int		i;
	char	*path;
	char	**path_array;
	char	*full_path;

	if (ft_strchr(cmd[0], '/'))
		return ;
	path = get_env_value(shell->env, "PATH");
	path_array = ft_split(path, ':');
	i = -1;
	full_path = NULL;
	while (path_array[++i])
	{
		free(full_path);
		full_path = ft_strjoin(path_array[i], "/");
		full_path = ft_strjoin(full_path, cmd[0]);
		if (access(full_path, F_OK) == 0)
		{
			free(cmd[0]);
			cmd[0] = full_path;
			break ;
		}
	}
	ft_free_array(path_array);
	free(path);
}

void child_process(t_shell *shell, t_pipes *pipes, int prev_pipe[2])
{
	char **envpp;
	envpp = create_envpp(shell);

	setup_redirection(pipes, prev_pipe, shell);
	ft_access(&pipes->cmds[0], shell);
	if (pipes->cmds[0][0] == '/')
		if (execve(pipes->cmds[0], pipes->cmds, envpp) == -1)
			exit (handle_error("execve"));
	ft_free_array(envpp);
}

void execute_cmd(t_shell *shell, t_pipes *pipes, int prev_pipe[2])
{
	pid_t	pid;
	int		status;
	char	**envpp;

	pid = fork();
	if (pid == -1)
		exit (handle_error("fork"));
	if (pid == 0)
		child_process(shell, pipes, prev_pipe);
	else
	{
		if (waitpid(pid, &status, 0) == -1)
			exit (handle_error("waitpid"));
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
				exit (handle_error("pipe"));
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
