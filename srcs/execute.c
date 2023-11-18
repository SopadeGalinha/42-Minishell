
#include "../includes/minishell.h"

void	init_builtin(const char *builtin[7])
{
	builtin[0] = "pwd";
	builtin[1] = "cd";
	builtin[2] = "echo";
	builtin[3] = "export";
	builtin[4] = "exit";
	builtin[5] = "unset";
	builtin[6] = "env";
	builtin[7] = NULL;
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

void	execute_cmd(t_shell *shell, t_pipes *pipes)
{
	printf("not a builtin\n");
}

void	execute_pipeline(t_shell *shell)
{
	t_pipes		*pipes;
	const char	*builtin[7];
	int			is_builtin;

	pipes = shell->pipes;
	init_builtin(builtin);
	while (pipes)
	{
		if (pipes->fd[OUT] != -1)
			shell->std_out = pipes->fd[OUT];
		if (pipes->fd[IN] != -1)
			shell->std_in = pipes->fd[IN];
		is_builtin = ft_is_builtin(builtin, pipes->cmds[0]);
		if (is_builtin != -1)
			shell->builtin[is_builtin](shell, pipes);
		else
			execute_cmd(shell, pipes);
		pipes = pipes->next;
	}
}