/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhogonca <jhogonca@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/28 17:28:01 by jhogonca          #+#    #+#             */
/*   Updated: 2023/11/18 14:53:44 by jhogonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	g_exit_status;

int	main(int ac, char **av, char **envp)
{
	t_shell	shell;

	if (ac != 1 || !av)
		return (ft_printf_fd(2, ERROR_ARGS));
	shell = (t_shell){0};
	init_shell(&shell, envp);
	while (true)
	{
		get_input(&shell);
		if (shell.input == NULL)
			continue ;
		if (shell.input[0])
		{
			if (shell.input && input_is_valid(shell.input))
				if (parse_input(&shell))
					execute_pipeline(&shell);
		}
		if (ft_strncmp(shell.input, "exit", 4) == 0)
			break ;
		free_struct(&shell, 0);
	}
	free_struct(&shell, 1);
	rl_clear_history();
	ft_printf_fd(1, "Bye!\n");
	return (0);
}
