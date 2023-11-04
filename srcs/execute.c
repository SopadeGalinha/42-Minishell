/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhogonca <jhogonca@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/09 18:18:41 by jhogonca          #+#    #+#             */
/*   Updated: 2023/11/04 18:50:13 by jhogonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	str_isalpha(char *str, int flag)
{
	int	i;

	i = 0;
	if (flag)
		i = 1;
	while (str[i])
	{
		if (!isalpha(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int	arg_checker(t_shell *shell, char *str)
{
	int	i = 0;

	if (ft_isdigit(str[0]))
	{
		ft_printf_fd(2, "minishell: export: `%s': not a valid identifier\n", str);
		return (-1);
	}
	else if (str[0] == '_' && str[1])
	{
		if ((!ft_isalnum(str[1]) && str[1] != '_') || str_isalpha(str, 1) == 0)
		{
			ft_printf_fd(2, "minishell: export: `%s': not a valid identifier\n", str);;
			return (-1);
		}
		else
			return (1);
	}
	else if (str[0] == '_' && (str[1] == '=' || str[1] == '\0'))
	{
		return (-2);
	}
	else if (str_isalpha(str, 0) == 0)
	{
		ft_printf_fd(2, "minishell: export: `%s': not a valid identifier\n", str);
		return (-1);
	}
	return (1);
}

bool	ft_access(t_shell *shell)
{
	t_pipes	*tmp;
	char	**array;
	int		i;
	char	*path;

	i = -1;
	tmp = shell->pipes;
	path = get_env_value(shell->env, "PATH");
	array = ft_split(path, ':');
	while (array[++i])
	{
		free(path);
		path = ft_strjoin(array[i], "/");
		path = ft_strjoin(path, tmp->cmds[0]);
		 if (access(path, F_OK) == 0)
		{
			tmp->path = ft_strdup(path);
			free(path);
			break ;
		}
	}
	execve(tmp->path, tmp->cmds, NULL);
	return (true);
}

void	execute(t_shell *shell)
{
	t_pipes		*pipes;
	int			function;
	const char	*builtin[7];

	builtin[0] = "pwd";
	builtin[1] = "cd";
	builtin[2] = "echo";
	builtin[3] = "export";
	builtin[4] = "exit";
	builtin[5] = "unset";
	builtin[6] = "env";

	pipes = shell->pipes;
	while (pipes)
	{
		function = -1;
		while (++function < 7)
		{
			if (ft_strncmp(pipes->cmds[0], builtin[function], \
			ft_strlen(builtin[function])) == 0
				&& ft_strlen(pipes->cmds[0]) == ft_strlen(builtin[function]))
				break ;
		}
		if (function < 7)
			shell->builtin[function](shell, pipes);
		else
			ft_printf_fd(2, "minishell: %s: command not found\n", pipes->cmds[0]);
		pipes = pipes->next;
	}
}
