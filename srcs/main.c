/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhogonca <jhogonca@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/17 04:29:40 by jhogonca          #+#    #+#             */
/*   Updated: 2023/09/17 05:28:33 by jhogonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int main(int ac, char **av, char **env)
{
	if (ac < 1)
		return 1;
		char input_buffer[1024]; // You can adjust the buffer size as needed.
	while (1)
	{
		ft_printf_fd(1, M I N I S H E L L Z);
		if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
			// Handle EOF or error.
			break; // Exit the loop.
		}
		// Remove the newline character from the input (if present).
		input_buffer[strcspn(input_buffer, "\n")] = '\0';
		
		// Now you have the user's input in `input_buffer`. You can process it here.
		// For a basic minishell, you'd parse the input and execute the corresponding command.
		// You'll need to implement command parsing and execution logic.

		// For this example, we'll just echo the input back to the user.
		ft_printf_fd(1, "You entered: %s\n", input_buffer);
	}
		
	return 0;
}
