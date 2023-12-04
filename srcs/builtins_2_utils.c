/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_2_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heolivei <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 19:49:42 by heolivei          #+#    #+#             */
/*   Updated: 2023/12/04 19:49:56 by heolivei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_exit_aux(char **cmds)
{
	int	i;
	int	flag;

	i = 0;
	flag = 0;
	if (ft_count_words(cmds) == 2)
	{
		if (cmds[1][0] == '-' || cmds[1][0] == '+')
			i++;
		while (cmds[1][i])
		{
			if (!ft_isdigit(cmds[1][i]))
			{
				print_error("minishell: exit: numeric argument required", 2);
				flag = 1;
				break ;
			}
			i++;
		}
		if (!flag) 
			g_exit_status = ft_atoi(cmds[1]);
	}
}
