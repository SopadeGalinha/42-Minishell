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

static void	access_aux(char *cmd_path, char **cmd, char **path_array)
{
	char	*aux;
	int		i;

	i = -1;
	aux = NULL;
	while (path_array[++i])
	{
		free(aux);
		free(cmd_path);
		aux = ft_strjoin(path_array[i], "/");
		cmd_path = ft_strjoin(aux, cmd[0]);
		if (access(cmd_path, F_OK) == 0)
		{
			free(cmd[0]);
			cmd[0] = ft_strdup(cmd_path);
			break ;
		}
	}
	free(aux);
	free(cmd_path);
	ft_free_2d_array((void **)path_array, 0);
}

void	ft_access(char **cmd, t_shell *shell)
{
	char	**path_array;
	char	*full_path;
	char	*cmd_path;

	if (!cmd || !cmd[0])
		return ;	
	if (ft_strchr(cmd[0], '/'))
		return ;
	full_path = get_env_value(shell->env, "PATH");
	path_array = ft_split(full_path, ':');
	free(full_path);
	cmd_path = NULL;
	return (access_aux(cmd_path, cmd, path_array));
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

	if (!cmd)
		return (-1);
	i = -1;
	while (cmd && ++i < 7)
		if (ft_strncmp(builtin[i], cmd, ft_strlen(builtin[i])) == 0
			&& ft_strlen(builtin[i]) == ft_strlen(cmd))
			return (i);
	return (-1);
}
