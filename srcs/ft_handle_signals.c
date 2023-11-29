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

static void	signal_hdl(int sig)
{
	if (sig == SIGINT)
	{
		ft_printf_fd(STDOUT_FILENO, "\n");
		return ;
	}
	else if (sig == SIGQUIT)
	{
		ft_printf_fd(STDOUT_FILENO, "Quit: 3\n");
		return ;
	}
}

void	ft_handle_signals(int flag)
{
	if (flag == PARENT)
	{
		signal(SIGQUIT, signal_hdl);
		signal(SIGINT, signal_hdl);
	}
	if (flag == MAIN)
	{
		signal(SIGQUIT, SIG_IGN);
		signal(SIGINT, sig_handler);
	}
	if (flag == CHILD)
	{
		signal(SIGQUIT, SIG_DFL);
		signal(SIGINT, SIG_DFL);
	}
}
