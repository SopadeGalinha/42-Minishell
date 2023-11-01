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
		if (strncmp(current->key, key, strlen(current->key)) == 0)
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

void	ft_unset(t_shell *shell, char *key)
{
	delete_node(&shell->env, key);
	delete_node(&shell->exp, key);
}

void	ft_pwd(t_shell *shell)
{
	char	*pwd;

	(void)shell;
	pwd = getcwd(NULL, 0);
	ft_printf_fd(shell->std_out, "%s\n", pwd);
	free(pwd);
}

void	ft_cd(t_shell *shell)
{
	char	*param;
	char	*pwd;

	param = shell->pipes->cmds[1];
	if (param == NULL)
	{
		pwd = getcwd(NULL, 0);
		ft_printf_fd(shell->std_out, "%s\n", pwd);
		free(pwd);
	}
	else if (chdir(param) == -1)
	{
		ft_printf_fd(2, "cd: %s: %s\n", param, strerror(errno));
		g_exit_status = 1;
	}
}

void	ft_echo(t_shell *shell)
{
	int		i;
	int		n_flag;

	i = 0;
	n_flag = 0;
	while (shell->pipes->cmds[++i]
		&& ft_strncmp(shell->pipes->cmds[i], "-n", 2) == 0)
		n_flag = 1;
	while (shell->pipes->cmds[i])
	{
		ft_printf_fd(1, "%s", shell->pipes->cmds[i]);
		if (shell->pipes->cmds[i + 1])
			ft_printf_fd(shell->std_out, " ");
		i++;
	}
	if (!n_flag)
		ft_printf_fd(shell->std_out, "\n");
}
