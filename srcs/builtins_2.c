/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simple_builtins.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhogonca <jhogonca@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/12 10:47:58 by jhogonca          #+#    #+#             */
/*   Updated: 2023/11/12 10:47:58 by jhogonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	delete_node(t_env **lst, char *key)
{
	t_env	*current;
	t_env	*prev;

	prev = NULL;
	current = *lst;
	while (current != NULL)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			if (prev == NULL)
				*lst = current->next;
			else
				prev->next = current->next;
			free(current->key);
			free(current->value);
			free(current->line);
			free(current);
			return ;
		}
		prev = current;
		current = current->next;
	}
}

void	ft_unset(t_shell *shell, t_pipes *pipes)
{
	int	i;

	i = 0;
	while (pipes->cmds[++i])
	{
		delete_node(&shell->env, pipes->cmds[i]);
		delete_node(&shell->exp, pipes->cmds[i]);
	}
	g_exit_status = 0;
}

void	ft_pwd(t_shell *shell, t_pipes *pipes)
{
	char	*pwd;

	(void)shell;
	(void)pipes;
	if (pipes->cmds[1] && pipes->cmds[1][0] == '-')
	{
		if (pipes->cmds[1][1])
		{
			ft_printf_fd(STDOUT_FILENO, "pwd does not support options\n");
			g_exit_status = 1;
			return ;
		}
	}
	pwd = getcwd(NULL, 0);
	ft_printf_fd(STDOUT_FILENO, "%s\n", pwd);
	free(pwd);
	g_exit_status = 0;
}

void	ft_exit(t_shell *shell, t_pipes *pipes)
{
	if (count_pipes(shell->tokens) == 0)
		ft_printf_fd(STDOUT_FILENO, "Bye bye\n");
	if (ft_count_words(pipes->cmds) > 2)
	{
		print_error("minishell: exit: too many arguments", 1);
		return ;
	}
	ft_exit_aux(pipes->cmds);
	free_struct(shell, 1);
}

void	ft_echo(t_shell *shell, t_pipes *pipes)
{
	int		i;
	int		j;
	int		n_flag;

	i = 0;
	n_flag = 0;
	while (pipes->cmds[++i]
		&& ft_strncmp(pipes->cmds[i], "-n", 2) == 0)
	{
		j = 0;
		while (pipes->cmds[i][++j] == 'n')
			;
		if ((int)ft_strlen(pipes->cmds[i]) != (j))
			break ;
		n_flag = 1;
	}
	while (pipes->cmds[i])
	{
		ft_printf_fd(STDOUT_FILENO, "%s", pipes->cmds[i]);
		if (pipes->cmds[i++ + 1])
			ft_printf_fd(STDOUT_FILENO, " ");
	}
	if (!n_flag && shell)
		ft_printf_fd(STDOUT_FILENO, "\n");
	g_exit_status = 0;
}
