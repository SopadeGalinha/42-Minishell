/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhogonca <jhogonca@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 15:23:53 by jhogonca          #+#    #+#             */
/*   Updated: 2023/10/05 15:23:53 by jhogonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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
		ft_printf_fd(1, "path: %s\n", path);
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

bool	parse_input(t_shell *shell)
{
	if (!lexical_analyzer(shell->input, &shell->tokens))
		return (false);
	if (!process_tokens(shell))
		return (false);
	if (!create_pipeline_node(shell))
		return (false);
	if (shell->error == true)
		printf("Error só n sei qual\n");
	if (ft_access(shell) == false)
		return (false);
	print_pipes(shell->pipes);
	return (true);
}
