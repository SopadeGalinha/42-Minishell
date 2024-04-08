/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhogonca <jhogonca@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/06 13:39:55 by jhogonca          #+#    #+#             */
/*   Updated: 2024/04/08 19:04:59 by jhogonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	g_exit_status = 0;

int	main(int argc, char **argv, char **env)
{
	t_shell	shell;

	(void)argc;
	(void)argv;
	shell = (t_shell){0};
	init_shell(&shell, env);
	while (true)
	{
		if (get_input(&shell))
			if (parser(&shell))
				execute(&shell);
		free_struct(&shell, 0);
	}
	return (0);
}
