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
void execute(t_shell *shell)
{
	int fd[2];
	int pid_1;
	int pid_2;
	char **envp;

	envp = get_envp_array(shell);
	if (pipe(fd) == -1)
		exit(ft_error("pipe", 1));
	pid_1 = fork();
	if (pid_1 == -1)
		exit(ft_error("fork", 1));
	if (pid_1 == 0)
	{
		if (dup2(fd[1], STDOUT_FILENO) == -1)
			exit(ft_error("dup2", 1));
		close(fd[0]);
		close(fd[1]);
		if (execve(shell->pipes->cmds[0], shell->pipes->cmds, envp) == -1)
			exit(ft_error("execve", 1));
	}

	pid_2 = fork();
	if (pid_2 == -1)
		exit(ft_error("fork", 1));
	if (pid_2 == 0)
	{
		if (dup2(fd[0], STDIN_FILENO) == -1)
			exit(ft_error("dup2", 1));
		close(fd[0]);
		close(fd[1]);
		if (execve(shell->pipes->next->cmds[0], shell->pipes->next->cmds, envp) == -1)
			exit(ft_error("execve", 1));
	}
	close(fd[0]);
	close(fd[1]);
	waitpid(pid_1, NULL, 0);
	waitpid(pid_2, NULL, 0);
}
