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

static void	hdl_signals_main(int signum)
{
	(void)signum;
	rl_replace_line("", 0);
	rl_on_new_line();
	ft_printf_fd(0, "\n");
	rl_redisplay();
	g_exit_status = 130;
}

void	signals_main(void)
{
	signal(SIGINT, hdl_signals_main);
	signal(SIGQUIT, SIG_IGN);
}

static void	hdl_signals_child(int sig)
{
	if (sig == SIGINT)
		ft_printf_fd(0, "\n");
	else
		ft_printf_fd(0, "Quit (core dumped)\n");
}

void	signals_child(void)
{
	signal(SIGINT, hdl_signals_child);
	signal(SIGQUIT, hdl_signals_child);
}

void	signals_wait(void)
{
	signal(SIGINT, hdl_signals_child);
	signal(SIGQUIT, hdl_signals_child);
}
