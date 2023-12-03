/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhogonca <jhogonca@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 08:56:57 by jhogonca          #+#    #+#             */
/*   Updated: 2023/11/01 08:56:57 by jhogonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	str_isalpha_isequal_isunder(char *str, int flag)
{
	int	i;

	i = 0;
	if (flag)
		i = 1;
	while (str[i])
	{
		if (!isalnum(str[i]) || str[i] == '_' || str[i] == '=')
			return (0);
		i++;
	}
	return (1);
}

int	arg_checker(t_shell *shell, char *str)
{
	(void)shell;
	if (ft_isdigit(str[0]))
	{
		ft_printf_fd(STDERR_FILENO, MS_ERR"export: ");
		ft_printf_fd(STDERR_FILENO, "'%s': not a valid identifier\n", str);
		g_exit_status = 1;
		return (-1);
	}
	else if (str[0] == '_' && str[1])
	{
		if (!ft_isalnum(str[1]) && str_isalpha_isequal_isunder(str, 1) == 0)
		{
			ft_printf_fd(STDERR_FILENO, MS_ERR"export: ");
			ft_printf_fd(STDERR_FILENO, "'%s': not a valid identifier\n", str);
			g_exit_status = 1;
			return (-1);
		}
		else
			return (1);
	}
	else if (str[0] == '_' && (str[1] == '=' || str[1] == '\0'))
		return (-2);
	else if (str[0] == '=' || (str_isalpha_isequal_isunder(str, 0) == 0 && ((ft_strchr(str, '=') == NULL) || ft_strchr(str, '.'))))
	{
		ft_printf_fd(STDERR_FILENO, MS_ERR"export: ");
		ft_printf_fd(STDERR_FILENO, "'%s': not a valid identifier\n", str);
		g_exit_status = 1;
		return (-1);
	}
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
				{
					
					update_lists(shell, pipes->cmds[i], 1);
				}
				else if (!ft_strchr(pipes->cmds[i], '='))
					update_lists(shell, pipes->cmds[i], 0);
			}
			i++;
		}
	}
	else
		print_list(shell, 0, pipes);
}

void	ft_env(t_shell *shell, t_pipes *pipes)
{
	if (pipes->cmds[1])
	{
		ft_printf_fd(STDERR_FILENO, "env: ");
		ft_printf_fd(STDERR_FILENO, "‘%s’: No such file or directory\n",
			pipes->cmds[1]);
		g_exit_status = 127;
	}
	else
		print_list(shell, 1, pipes);
}
