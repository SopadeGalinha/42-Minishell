/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils_2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhogonca <jhogonca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 22:38:18 by heolivei          #+#    #+#             */
/*   Updated: 2023/12/05 01:05:15 by jhogonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

bool	heredoc_validate(char *line, char *target)
{
	if (!line)
	{
		ft_printf_fd(STDERR_FILENO, MS_ERR RESET"warning: here-document");
		ft_printf_fd(STDERR_FILENO, \
		" delimited by end-of-file (wanted `%s')\n", target);
		return (false);
	}
	if (ft_strcmp(line, target) == 0)
	{
		free(line);
		return (false);
	}
	return (true);
}

char	**get_envp_array(t_shell *shell)
{
	char	**array;
	int		i;
	t_env	*current;

	i = -1;
	current = shell->env;
	array = malloc(sizeof(char *) * (ft_list_envsize(shell->env) + 1));
	if (!array)
		return (NULL);
	while (current != NULL)
	{
		array[++i] = ft_strdup(current->line);
		if (!array[i])
			return (NULL);
		current = current->next;
	}
	array[++i] = NULL;
	return (array);
}

int	ft_is_builtin(const char *builtin[7], char *cmd)
{
	int	i;

	if (!cmd)
		return (-1);
	i = -1;
	while (cmd && ++i < 7)
		if (ft_strncmp(builtin[i], cmd, ft_strlen(builtin[i])) == 0
			&& ft_strlen(builtin[i]) == ft_strlen(cmd))
			return (i);
	return (-1);
}

void	close_pipes(int **pipes, int process_num)
{
	int	pos;

	pos = -1;
	while (++pos < process_num - 1)
	{
		close(pipes[pos][READ_END]);
		close(pipes[pos][WRITE_END]);
	}
}

void	waiting(int process_num, t_shell *shell)
{
	close_pipes(shell->pipes_fd, process_num);
	
	if (waitpid(-1, &g_exit_status, 0) == -1)
		ft_error("Error waiting for process", 1);
	if (WIFEXITED(g_exit_status))
		g_exit_status = WEXITSTATUS(g_exit_status);
	else if (WIFSIGNALED(g_exit_status))
		g_exit_status = WTERMSIG(g_exit_status) + 128;
	ft_printf_fd(0, "\n");
}
