/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_aux.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhogonca <jhogonca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 22:04:37 by jhogonca          #+#    #+#             */
/*   Updated: 2023/11/07 22:23:38 by jhogonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_pwd(t_shell *shell, t_pipes *pipes)
{
	char	*pwd;

	(void)shell;
	(void)pipes;
	pwd = getcwd(NULL, 0);
	ft_printf_fd(shell->std_out, "%s\n", pwd);
	free(pwd);
}

static char *define_key(char *param)
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

static char	*get_param(t_shell *shell, char *key)
{
	char	*param;

	param = get_env_value(shell->env, key);
	if (param && param[0] == '\0' && strlen(param) == strlen(key))
	{
		fprintf(stderr, "minishell: cd: %s not set\n", key);
		g_exit_status = 1;
        free(param);
		return (NULL);
	}
	else
		ft_printf_fd(1, "%s\n", param);
	return (param);
}

static void	cd_aux(t_shell *shell, char *param, char *oldpwd)
{
	char	*tmp;
	char	*key;
	int		status;

	status = chdir(param);
	if (status == -1)
	{
		free(oldpwd);
		ft_printf_fd(2, "minishell: cd: %s: %s\n", param, strerror(errno));
		return ;
	}
    free(param);
	key = getcwd(NULL, 0);
	tmp = ft_strjoin("OLDPWD=", oldpwd);
	update_lists(shell, tmp, 1);
	free(tmp);
	tmp = ft_strjoin("PWD=", key);
    free(key);
	update_lists(shell, tmp, 1);
	free(tmp);
	free(oldpwd);
}

void	ft_cd(t_shell *shell, t_pipes *pipes)
{
	char	*param;
	char	*oldpwd;
	char	*key;

	key = NULL;
	if (ft_count_words(pipes->cmds) > 2)
	{
		print_error("minishell: cd: too many arguments", 1);
		return ;
	}
	oldpwd = getcwd(NULL, 0);
	key = define_key(pipes->cmds[1]);
	if (key)
		param = get_param(shell, key);
	else
		param = ft_strdup(pipes->cmds[1]);
	free(key);
	if (!param)
	{
		free(oldpwd);
		return ;
	}
	cd_aux(shell, param, oldpwd);
}
