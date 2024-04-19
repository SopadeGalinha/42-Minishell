/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rboia-pe <rboia-pe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 17:41:43 by jhogonca          #+#    #+#             */
/*   Updated: 2024/04/19 19:16:30 by rboia-pe         ###   ########.fr       */
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
	printf("Executando execve\n");
	if (execve(pipes_lst->cmds[0], pipes_lst->cmds, envp) == -1)
	{
		ft_printf_fd(2, MS_ERR RESET"%s: %s\n", \
		pipes_lst->cmds[0], strerror(errno));
		ft_free_2d_array((void **)envp, 0);
		g_exit_status = 127;
		free_struct(shell, 1);
		exit(g_exit_status);
	}
	printf("Execve finalizado\n");
	ft_free_2d_array((void **)envp, 0);
	g_exit_status = WEXITSTATUS(g_exit_status);
	printf("Execve finalizado com status %d\n", g_exit_status);
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
		printf("Processo filho %d\n", index);
		signal(SIGINT, hd1_signal_handler);
		get_redirections(index, shell->pipes_fd, process, shell);
		close_redirections(process, builtin_index, \
		shell->pipes_fd, count_pipes(shell->tokens) + 1);
		if (builtin && builtin_index != -1)
		{
			printf("Executando builtin\n");
			signal(SIGQUIT, hd1_signal_handler);
			shell->builtin[builtin_index](shell, process);
			printf("Builtin finalizado\n");
			free_struct(shell, 1);
			ft_printf_fd(1 , "Saindo do execve\n");
			exit(g_exit_status);
			printf("Execve finalizado (NAO DEVERIA TER PRINTADO)\n");
		}
		else
		{
			signal(SIGQUIT, hd1_signal_handler);
			printf("A entrar no execve\n");
			ft_execve(shell, process);
			printf("Execve finalizado\n");
		}
	}
	else if (process->pid == -1)
		ft_error("Error creating process", 1);
	else
	{
		printf("Processo %d criado\n", index);
		//signals_wait();
	}
}

static void	waiting(int process_num, t_shell *shell)
{
	int	i;

	i = -1;
	close_pipes(shell->pipes_fd, process_num);
	while (++i < process_num)
	{
		printf("Esperando processo %d\n", i);
		if (waitpid(-1, &g_exit_status, 0) == -1)
			ft_error("Error waiting for process", 1);
		printf("Processo %d finalizado\n", i);
		if (WIFEXITED(g_exit_status))
		{
			g_exit_status = WEXITSTATUS(g_exit_status);
			printf("Processo %d finalizado com status %d\n", i, g_exit_status);
		}
		else if (WIFSIGNALED(g_exit_status))
		{
			g_exit_status = WTERMSIG(g_exit_status) + 128;
			printf("Processo %d finalizado com status %d\n", i, g_exit_status);
		}
	}
	/* if (waitpid(-1, &g_exit_status, 0) == -1)
			ft_error("Error waiting for process", 1);
	if (WIFEXITED(g_exit_status))
		g_exit_status = WEXITSTATUS(g_exit_status);
	else if (WIFSIGNALED(g_exit_status))
		g_exit_status = WTERMSIG(g_exit_status) + 128; */
	printf("Processo finalizado\n");
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
	i_pipes[1] = count_pipes(shell->tokens) + 1;
	process = shell->pipes;
	init_builtin(builtin);
	if (!create_pipes(shell) && i_pipes[1] > 0)
		return (ft_error("Error creating pipes", 1));
	//printf("Num pipes: %d\n", i_pipes[1]);
	while (process && ++i_pipes[0] > -1)
	{
		/*
		Se parenteses entao
			enviar para parser
			armazenar g_exit_status
			enviar para funcao de tratamento de operadores logicos && ||
		*/
		if (has_parenthesis(process))
		{
			printf("Tratamento de parenteses\n");
			process->pid = fork();
			int	builtin_index = ft_is_builtin(builtin, process->cmds[0]);
			if (process->pid == 0)
			{
				free_tokens(&shell->tokens);
				free(shell->input);
				shell->input = process->cmds[0];
				if (parser(shell))
				{
					get_redirections(i_pipes[0], shell->pipes_fd, process, shell);
					close_redirections(process, builtin_index, shell->pipes_fd, i_pipes[0]);
					execute(shell);
				}
				free_struct(shell, 1);
				exit(g_exit_status);
			}
			//else
			//{
			//	waiting(i_pipes[1], shell);
			//}
			process = process->next;
			continue ;
			//process_logical_operators(shell);
		}
		if (ft_is_builtin(builtin, process->cmds[0]) != -1 && i_pipes[1] == 0)
			shell->builtin[ft_is_builtin(builtin, \
				process->cmds[0])](shell, process);
		else
			run(shell, process, i_pipes[0], builtin);
		if (i_pipes[1] > 0 && process->next)
		{
			//sleep(2);
			//signals_wait();
		}
		process = process->next;
	}
	if ((ft_is_builtin(builtin, shell->pipes->cmds[0]) != -1  || \
	shell->pipes->type == PARENTHESIS) && i_pipes[1] == 0)
	{
		//printf("Primeiro wait\n");
		waiting(i_pipes[1], shell);
	}
	else
	{
		//printf("Segundo wait\n");
		waiting(i_pipes[1] + 1, shell);
	}
	return (0);
}
