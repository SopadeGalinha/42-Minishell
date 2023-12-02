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
	{
		ft_printf_fd(0, "\n");
		g_exit_status = 130;
		exit(130);
	}
	else
	{
		ft_printf_fd(0, "Quit: (core dumped)\n");
		g_exit_status = 131;
		exit(131);
	}
}

void	signals_child(void)
{
	struct sigaction	sa;
	sa.sa_handler = hdl_signals_child;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
}

static void	hdl_signals_heredoc(int signum)
{
	(void)signum;
	ft_printf_fd(0, "\n");
	exit(130);
}

void	signals_heredoc(void)
{
	signal(SIGINT, hdl_signals_heredoc);
	signal(SIGQUIT, SIG_IGN);
}

void	signals_wait(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}
