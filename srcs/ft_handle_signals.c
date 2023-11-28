/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_handle_signals.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhogonca <jhogonca@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 18:47:29 by jhogonca          #+#    #+#             */
/*   Updated: 2023/09/20 18:47:29 by jhogonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	sig_handler(int signum)
{
	(void)signum;
	rl_replace_line("", 0);
	rl_on_new_line();
	ft_printf_fd(0, "\n");
	rl_redisplay();
	g_exit_status = 130;
}

void	ft_handle_signals(void)
{
	struct sigaction	sig_actions;

	signal(SIGQUIT, SIG_IGN);
	sig_actions.sa_handler = sig_handler;
	sigemptyset(&sig_actions.sa_mask);
	sig_actions.sa_flags = 0;
	sigaction(SIGINT, &sig_actions, NULL);
}
