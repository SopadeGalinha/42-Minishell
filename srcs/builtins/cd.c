/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhogonca <jhogonca@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/06 15:25:33 by jhogonca          #+#    #+#             */
/*   Updated: 2024/04/06 15:45:54 by jhogonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*define_key(char *param)
{
	char	*key;

	key = NULL;
	if (param && param[0] == '-' && strlen(param) <= 2)
	{
		if (strlen(param) == 1)
			key = strdup("OLDPWD");
		else if (param[1] == '-')
			key = strdup("HOME");
	}
	return (key);
}

static bool	ft_chdir(t_shell *shell, char *param, char *oldpwd)
{
	char	*pwd;
	char	*join;

	if (!param)
		return (false);
	if (chdir(param) == -1)
		return (print_error(strerror(errno), 1));
	pwd = getcwd(NULL, 0);
	if (!pwd)
		return (print_error(strerror(errno), 1));
	join = ft_strjoin("OLDPWD=", oldpwd);
	u_update_lists(shell, join, 1);
	free(join);
	join = ft_strjoin("PWD=", pwd);
	u_update_lists(shell, join, 1);
	free(pwd);
	free(join);
	return (false);
}

static bool	cd_validation(t_shell *shell, t_pipes *pipes)
{
	char	*home;
	char	*oldpwd;

	if (ft_count_words(pipes->cmds) == 2)
	{
		if (ft_strcmp(pipes->cmds[1], "") == 0 && pipes->type != ENV)
			return (print_error("\n", 0));
		else
			return (true);
	}
	if (ft_count_words(pipes->cmds) > 2)
		return (print_error("minishell: cd: too many arguments", 1));
	home = get_env_value(shell->env, "HOME");
	if (!home || ft_strlen(home) == 0)
	{
		if (home)
			free(home);
		return (print_error("minishell: cd: HOME not set", 1));
	}
	oldpwd = getcwd(NULL, 0);
	ft_chdir(shell, home, oldpwd);
	free(home);
	free(oldpwd);
	return (false);
}

static char	*get_param(t_shell *shell, char *key)
{
	char	*param;

	param = get_env_value(shell->env, key);
	if (!param || ft_strlen(param) == 0)
	{
		ft_printf_fd(2, "minishell: cd: %s not set\n", key);
		g_exit_status = 1;
		if (param)
			free(param);
		param = NULL;
	}
	return (param);
}

void	ft_cd(t_shell *shell, t_pipes *pipes)
{
	char	*param;
	char	*oldpwd;
	char	*key;

	key = NULL;
	if (!cd_validation(shell, pipes))
		return ;
	key = define_key(pipes->cmds[1]);
	if (key)
		param = get_param(shell, key);
	else
		param = ft_strdup(pipes->cmds[1]);
	oldpwd = getcwd(NULL, 0);
	ft_chdir(shell, param, oldpwd);
	free(oldpwd);
	if (key && ft_strncmp(key, "OLDPWD", 6) == 0 && param)
		ft_printf_fd(STDOUT_FILENO, "%s\n", param);
	free(param);
	free(key);
	g_exit_status = 0;
}
