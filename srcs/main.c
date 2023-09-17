/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhogonca <jhogonca@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/17 04:29:40 by jhogonca          #+#    #+#             */
/*   Updated: 2023/09/17 16:18:35 by jhogonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int main(int ac, char **av, char **env)
{
	struct sigaction sa;
	char *input;
    
	if (ac < 1)
		return 1;

    // Set up the SIGINT handler
    sa.sa_handler = sigint_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);
	while (true)
	{
		rl_initialize();
		char *input = readline(M I N I S H E L L Z);
		
		// Handle EOF or error.
		if (input == NULL) {
			break;
		}
		
		ft_printf_fd(1, "Your input: %s\n", input);
		free(input);
		add_history(input);
	}
	return 0;
}
