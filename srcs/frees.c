/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   frees.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhogonca <jhogonca@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/06 15:41:01 by jhogonca          #+#    #+#             */
/*   Updated: 2024/04/06 15:41:06 by jhogonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	ft_free_redirect(t_redir **redir)
{
	t_redir	*current;
	t_redir	*tmp;

	if (!redir)
		return ;
	current = *redir;
	while (current != NULL)
	{
		tmp = current->next;
		free(current->file);
		free(current);
		current = tmp;
	}
	*redir = NULL;
}

static bool	free_pipes(t_pipes **pipes)
{
	t_pipes	*current_pipeline;
	t_pipes	*next_pipeline;

	current_pipeline = *pipes;
	if (pipes == NULL || *pipes == NULL)
		return (true);
	if (current_pipeline->heredoc)
		free(current_pipeline->heredoc);
	while (current_pipeline != NULL)
	{
		ft_free_redirect(&current_pipeline->redir_in);
		ft_free_redirect(&current_pipeline->redir_out);
		ft_free_2d_array((void **)current_pipeline->cmds, 0);
		next_pipeline = current_pipeline->next;
		free(current_pipeline);
		current_pipeline = next_pipeline;
	}
	*pipes = NULL;
	return (true);
}

static void	free_tokens(t_token **tokens)
{
	t_token	*current;
	t_token	*tmp;

	if (tokens == NULL || *tokens == NULL)
		return ;
	current = *tokens;
	while (current != NULL)
	{
		tmp = current->next;
		if (current->data)
			free(current->data);
		free(current);
		current = tmp;
	}
	*tokens = NULL;
}

static void	free_env(t_env **env)
{
	t_env	*current;
	t_env	*tmp;

	if (env == NULL)
		return ;
	current = *env;
	while (current != NULL)
	{
		tmp = current->next;
		free(current->key);
		free(current->value);
		free(current->line);
		free(current);
		current = tmp;
	}
	*env = NULL;
}

void	free_struct(t_shell *shell, int running)
{
	int	process;

	process = count_pipes(shell->tokens) + 1;
	if (process == 1)
		process = 0;
	if (shell->pipes_fd)
		ft_free_2d_array((void **)shell->pipes_fd, process);
	if (shell->input != NULL)
		free(shell->input);
	if (shell->pipes != NULL)
		free_pipes(&(shell->pipes));
	if (shell->tokens != NULL)
		free_tokens(&(shell->tokens));
	if (shell->heredoc)
		free(shell->heredoc);
	if (running == 0)
		return ;
	free_env(&shell->env);
	free_env(&shell->exp);
	rl_clear_history();
	exit(g_exit_status);
}
