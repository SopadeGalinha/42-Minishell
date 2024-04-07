/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhogonca <jhogonca@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/06 13:25:47 by jhogonca          #+#    #+#             */
/*   Updated: 2024/04/06 15:42:23 by jhogonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	init_shell(t_shell *shell, char **env)
{
	shell->env = init_env(env);
	shell->exp = init_export(shell->env);
	shell->builtin[0] = ft_pwd;
	shell->builtin[1] = ft_cd;
	shell->builtin[2] = ft_echo;
	shell->builtin[3] = ft_export;
	shell->builtin[4] = ft_exit;
	shell->builtin[5] = ft_unset;
	shell->builtin[6] = ft_env;
}
