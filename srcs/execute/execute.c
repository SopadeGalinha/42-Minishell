/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rboia-pe <rboia-pe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 17:41:43 by jhogonca          #+#    #+#             */
/*   Updated: 2024/04/20 18:28:09 by rboia-pe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	**get_envp_array(t_shell *shell)
{
	char	**array;
	int		i;
	t_env	*current;

	i = -1;
	current = shell->env;
	array = malloc(sizeof(char *) * (ft_list_envsize(shell->env) + 1));
	if (!array)
		return (NULL);
	while (current != NULL)
	{
		array[++i] = ft_strdup(current->line);
		if (!array[i])
			return (NULL);
		current = current->next;
	}
	array[++i] = NULL;
	return (array);
}

static void	ft_execve(t_shell *shell, t_pipes *pipes_lst)
{
	char	**envp;

	if (!pipes_lst->cmds[0] || !pipes_lst->cmds)
	{
		free_struct(shell, 1);
		exit (0);
	}
	envp = get_envp_array(shell);
	if (execve(pipes_lst->cmds[0], pipes_lst->cmds, envp) == -1)
	{
		ft_printf_fd(2, MS_ERR RESET"%s: %s\n", \
		pipes_lst->cmds[0], strerror(errno));
		ft_free_2d_array((void **)envp, 0);
		g_exit_status = 127;
		free_struct(shell, 1);
		exit(g_exit_status);
	}
	ft_free_2d_array((void **)envp, 0);
	g_exit_status = WEXITSTATUS(g_exit_status);
	exit (g_exit_status);
}

static void	hd1_signal_handler(int signum)
{
	if (signum == SIGINT)
		ft_printf_fd(0, "\n");
	else
		ft_printf_fd(0, "Quit: (core dumped)\n");
}

void	signals_wait(void)
{
	signal(SIGINT, hd1_signal_handler);
	signal(SIGQUIT, hd1_signal_handler);
}

static void	run(t_shell *shell, t_pipes *process, int index, \
const char **builtin)
{
	int	builtin_index;

	builtin_index = ft_is_builtin(builtin, process->cmds[0]);
	process->pid = fork();
	if (process->pid == 0)
	{
		if (process->type == PIPELINE || process->type == NONE)
		{
			get_redirections(index, shell->pipes_fd, process);
			if (!(builtin_index != -1 && count_pipes(shell->tokens) == 0))
			{
				close_redirections(process, count_pipes(shell->tokens), \
				shell->pipes_fd, index);
			}
		}
		if (builtin && builtin_index != -1)
		{
			shell->builtin[builtin_index](shell, process);
			free_struct(shell, 1);
			exit(g_exit_status);
		}
		else
			ft_execve(shell, process);
	}
	else if (process->pid == -1)
		ft_error("Error creating process", 1);
}

static void	waiting(int process_num, t_shell *shell, int wait_nbr)
{
	int	i;

	i = -1;
	close_pipes(shell->pipes_fd, process_num);
	while (++i < process_num - wait_nbr)
	{
		if (waitpid(-1, &g_exit_status, 0) == -1)
			ft_error("Error waiting for process", 1);
		if (WIFEXITED(g_exit_status))
			g_exit_status = WEXITSTATUS(g_exit_status);
		else if (WIFSIGNALED(g_exit_status))
			g_exit_status = WTERMSIG(g_exit_status) + 128;
	}
}

bool	has_parenthesis(t_pipes *process)
{
	bool	has_parenthesis;
	int		i;

	i = -1;
	has_parenthesis = false;
	while (process->cmds[0][++i])
		if (process->cmds[0][i] == '(')
			has_parenthesis = true;
	return (has_parenthesis);
}

int	execute(t_shell *shell)
{
	int			i_pipes[2];
	t_pipes		*process;
	const char	*builtin[7];

	i_pipes[0] = -1;
	i_pipes[1] = count_pipes(shell->tokens);
	process = shell->pipes;
	init_builtin(builtin);
	if (!create_pipes(shell) && i_pipes[1] > 0)
		return (ft_error("Error creating pipes", 1));
	while (process && ++i_pipes[0] > -1)
	{
		/*
		Se parenteses entao
			enviar para parser
			armazenar g_exit_status
			enviar para funcao de tratamento de operadores logicos && ||
		*/
		//process_logical_operators(shell);
		if (has_parenthesis(process))
		{
			int ***pipes_fd = &shell->pipes_fd;
			process->pid = fork();
			int	builtin_index = ft_is_builtin(builtin, process->cmds[0]);
			if (process->pid == 0)
			{
				int temp = dup(STDOUT_FILENO);
				free_tokens(&shell->tokens);
				free(shell->input);
				shell->input = process->cmds[0];
				if (parser(shell, 1))
				{
					printf("Commands: %s\n", shell->pipes->cmds[0]);
					if (shell->pipes->cmds[1])
						printf("Commands: %s\n", shell->pipes->cmds[1]);
					shell->pipes_fd = *pipes_fd;
					get_redirections(i_pipes[0], shell->pipes_fd, process);
					close_redirections(process, i_pipes[1] - 1,\
					 shell->pipes_fd, i_pipes[0]);
					execute(shell);
				}
				free_struct(shell, 1);
				exit(g_exit_status);
			}
			process = process->next;
			continue ;
		}
		if (ft_is_builtin(builtin, process->cmds[0]) != -1 && \
		!process->redir_out && i_pipes[1] == 0)
		{
			get_redirections(i_pipes[0], shell->pipes_fd, process);
			shell->builtin[ft_is_builtin(builtin, \
				process->cmds[0])](shell, process);
		}
		else
		{
			run(shell, process, i_pipes[0], builtin);
			//if (ft_is_builtin(builtin, process->cmds[0]) != -1 && i_pipes[1] == 0)
			//	waiting(i_pipes[1], shell, 0);
		}
		process = process->next;
		if (process && process->type == AND)
			i_pipes[0]--;
	}
	/* if (i_pipes[1] == 1)
		i_pipes[1] = 0; */
	if (i_pipes[1] == 0 && ft_is_builtin(builtin, shell->pipes->cmds[0]) == -1)
		waiting(i_pipes[1], shell, (-1));
	else if (i_pipes[1] == 0 && ft_is_builtin(builtin, shell->pipes->cmds[0]) != -1)
		waiting(i_pipes[1], shell, (1));
	else
		waiting(i_pipes[1], shell, -1);
	return (0);
}
