/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhogonca <jhogonca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/28 17:28:01 by jhogonca          #+#    #+#             */
/*   Updated: 2023/12/04 17:42:55 by jhogonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	g_exit_status;

int	main(int ac, char **av, char **envp)
{
	t_shell	shell;

	if (ac != 1 || !av)
		return (ft_printf_fd(2, MS_ERR ERROR_ARGS TRY RESET));
	shell = (t_shell){0};
	init_shell(&shell, envp);
	while (true)
	{
		signals_main();
		get_input(&shell);
		if (shell.input == NULL)
		{
			ft_printf_fd(0, "exit\n");
			break ;
		}
		if (shell.input[0])
		{
			if (shell.input && input_is_valid(shell.input))
				if (parse_input(&shell))
					execute(&shell);
		}
		free_struct(&shell, 0);
	}
	free_struct(&shell, 1);
	return (0);
}
