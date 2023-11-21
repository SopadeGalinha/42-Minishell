/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhogonca <jhogonca@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 21:56:43 by jhogonca          #+#    #+#             */
/*   Updated: 2023/11/20 21:56:43 by jhogonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

static int	ft_list_envsize(t_env *lst)
{
	int	i;
	
	i = 0;
	while (lst)
	{
		lst = lst->next;
		i++;
	}
	return (i);
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

	i = -1;
	while (cmd && ++i < 7)
		if (ft_strncmp(builtin[i], cmd, ft_strlen(builtin[i])) == 0)
			return (i);
	return (-1);
}