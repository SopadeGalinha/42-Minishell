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

void	ft_unset(t_shell *shell)
{
	int	i;

	i = 0;
	while (shell->pipes->cmds[++i])
	{
		delete_node(&shell->env, shell->pipes->cmds[i]);
		delete_node(&shell->exp, shell->pipes->cmds[i]);
	}
}

void	ft_pwd(t_shell *shell)
{
	char	*pwd;

	(void)shell;
	pwd = getcwd(NULL, 0);
	ft_printf_fd(shell->std_out, "%s\n", pwd);
	free(pwd);
}

int	ft_count_words(char **str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

void	ft_cd(t_shell *shell)
{
	char	*param;
	char	*pwd;
	int		status;
	char	*oldpwd;
	char	*tmp;
	char	*key;

	g_exit_status = 0;
	if (ft_count_words(shell->pipes->cmds) > 2)
	{
		print_error("minishell: cd: too many arguments", 1);
		return ;
	}
	oldpwd = getcwd(NULL, 0);
	if (ft_strncmp(shell->pipes->cmds[1], "-", 1) == 0
		&& ft_strlen(shell->pipes->cmds[1]) <= 2)
	{
		if (strlen(shell->pipes->cmds[1]) == 1)
			key = ft_strdup("OLDPWD");
		else if (shell->pipes->cmds[1][1] == '-')
			key = ft_strdup("HOME");
	}
	if (ft_strlen(key) > 1)
	{
		param = get_env_value(shell->env, key);
		if (ft_strncmp(param, "", 1) == 0)
		{
			free(oldpwd);
			ft_printf_fd(2, "minishell: cd: %s not set\n", key);
			g_exit_status = 1;
			return ;
		}
		else
			ft_printf_fd(1, "%s\n", param);
	}
	else
		param = shell->pipes->cmds[1];
	status = chdir(param);
	if (status == -1)
	{
		free(oldpwd);
		ft_printf_fd(2, "minishell: cd: %s: %s\n", param, strerror(errno));
		return ;
	}
	pwd = getcwd(NULL, 0);
	tmp = ft_strjoin("OLDPWD=", oldpwd);
	update_lists(shell, tmp, 1);
	free(tmp);
	tmp = ft_strjoin("PWD=", pwd);
	update_lists(shell, tmp, 1);
	free(tmp);
	free(pwd);
}

void	ft_export(t_shell *shell)
{
		int	i = 1;

	if (shell->pipes->cmds[i])
	{
		while (shell->pipes->cmds[i])
		{
			if(arg_checker(shell, shell->pipes->cmds[i]) > 0)
			{
				if (ft_strchr(shell->pipes->cmds[i], '='))
					update_lists(shell, shell->pipes->cmds[i], 1);
				else if (!ft_strchr(shell->pipes->cmds[i], '='))
					update_lists(shell, shell->pipes->cmds[i], 0);
			}
			i++;
		}
	}
    else
        print_list(shell, 0);
}

void	ft_env(t_shell *shell)
{
	print_list(shell, 1);
}

void	ft_exit(t_shell *shell)
{
	(void)shell;
	ft_printf_fd(1, "exit\n");
	free_struct(shell , 1);
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
