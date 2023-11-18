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

static void	delete_node(t_env **lst, char *key)
{
	t_env	*current;
	t_env	*prev;

	prev = NULL;
	current = *lst;
	while (current != NULL)
	{
		if (ft_strncmp(current->key, key, strlen(current->key)) == 0)
		{
			if (prev == NULL)
				*lst = current->next;
			else
				prev->next = current->next;
			free(current->key);
			free(current->value);
			free(current->line);
			free(current);
			return ;
		}
		prev = current;
		current = current->next;
	}
}

void	ft_unset(t_shell *shell, t_pipes *pipes)
{
	int	i;

	i = 0;
	while (pipes->cmds[++i])
	{
		delete_node(&shell->env, pipes->cmds[i]);
		delete_node(&shell->exp, pipes->cmds[i]);
	}
}

static int	str_isalpha_isequal_isunder(char *str, int flag)
{
	int	i;

	i = 0;
	if (flag)
		i = 1;
	while (str[i])
	{
		if (!isalpha(str[i]) || str[i] == '_' || str[i] == '=')
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
		ft_printf_fd(shell->std_out, "minishell: export: ");
		ft_printf_fd(shell->std_out, "'%s': not a valid identifier\n", str);
		g_exit_status = 1;
		return (-1);
	}
	else if (str[0] == '_' && str[1])
	{
		if (!ft_isalnum(str[1]) && str_isalpha_isequal_isunder(str, 1) == 0)
		{
			ft_printf_fd(shell->std_out, "minishell: export: ");
			ft_printf_fd(shell->std_out, "'%s': not a valid identifier\n", str);
			g_exit_status = 1;
			return (-1);
		}
		else
			return (1);
	}
	else if (str[0] == '_' && (str[1] == '=' || str[1] == '\0'))
	{
		return (-2);
	}
	else if (str[0] == '=' || (str_isalpha_isequal_isunder(str, 0) == 0 && ft_strchr(str, '=') == NULL))
	{
		ft_printf_fd(shell->std_out, "minishell: export: ");
		ft_printf_fd(shell->std_out, "'%s': not a valid identifier\n", str);
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
					update_lists(shell, pipes->cmds[i], 1);
				else if (!ft_strchr(pipes->cmds[i], '='))
					update_lists(shell, pipes->cmds[i], 0);
			}
			i++;
		}
	}
	else
		print_list(shell, 0);
}



void	ft_env(t_shell *shell, t_pipes *pipes)
{
	if (pipes->cmds[1])
	{
		ft_printf_fd(shell->std_out, "env: ");
		ft_printf_fd(shell->std_out, "‘%s’: No such file or directory\n", pipes->cmds[1]);
		g_exit_status = 127;
	}
	else
		print_list(shell, 1);
}
