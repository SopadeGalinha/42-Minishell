/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhogonca <jhogonca@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/06 13:37:13 by jhogonca          #+#    #+#             */
/*   Updated: 2024/04/06 15:45:25 by jhogonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	str_isalpha_isequal_isunder(char *str, int flag)
{
	int	i;

	i = 0;
	if (flag)
	{
		i = 1;
		while (str[i])
		{
			if (!isalnum(str[i]) || str[i] == '_' || str[i] == '=')
				return (0);
			i++;
		}
	}
	else
	{
		while (str[i])
		{
			if ((!isalnum(str[i]) && str[i] != '_') || str[i] == '=')
				return (0);
			i++;
		}
	}
	return (1);
}

static int	arg_checker(t_shell *shell, char *str)
{
	(void)shell;
	if (ft_isdigit(str[0]))
		return (u_error_arg(str));
	else if (str[0] == '_' && str[1])
	{
		if (!ft_isalnum(str[1]) && str_isalpha_isequal_isunder(str, 1) == 0)
			return (u_error_arg(str));
		else
			return (1);
	}
	else if (str[0] == '_' && (str[1] == '=' || str[1] == '\0'))
		return (-2);
	else if (str[0] == '=' || (str_isalpha_isequal_isunder(str, 0) == 0
			&& ((ft_strchr(str, '=') == NULL) || ft_strchr(str, '.'))))
		return (u_error_arg(str));
	return (1);
}

void	ft_export(t_shell *shell, t_pipes *pipes)
{
	int	i;

	i = 1;
	if (pipes->cmds[i])
	{
		while (pipes->cmds[i])
		{
			if (arg_checker(shell, pipes->cmds[i]) > 0)
			{
				if (ft_strchr(pipes->cmds[i], '='))
					u_update_lists(shell, pipes->cmds[i], 1);
				else if (!ft_strchr(pipes->cmds[i], '='))
					u_update_lists(shell, pipes->cmds[i], 0);
			}
			i++;
		}
	}
	else
		u_print_list(shell, 0, pipes);
}
