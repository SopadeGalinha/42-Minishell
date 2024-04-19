/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   frees.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rboia-pe <rboia-pe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/06 15:41:01 by jhogonca          #+#    #+#             */
/*   Updated: 2024/04/19 19:13:08 by rboia-pe         ###   ########.fr       */
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

void	free_tokens(t_token **tokens)
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

	//printf("> Freeing struct\n");
	process = count_pipes(shell->tokens) + 1;
	//printf("> Process: %d\n", process);
	if (process == 1)
		process = 0;
	if (shell->pipes_fd)
		ft_free_2d_array((void **)shell->pipes_fd, process);
	//printf("> Freed pipes_fd\n");
	if (shell->input != NULL)
		free(shell->input);
	//printf("> Freed input\n");
	if (shell->pipes != NULL)
		free_pipes(&(shell->pipes));
	//printf("> Freed pipes\n");
	if (shell->tokens != NULL)
		free_tokens(&(shell->tokens));
	//printf("> Freed tokens\n");
	if (shell->heredoc)
		free(shell->heredoc);
	//printf("> Freed heredoc\n");
	if (running == 0)
		return ;
	free_env(&shell->env);
	//printf("> Freed env\n");
	free_env(&shell->exp);
	//printf("> Freed exp\n");
	rl_clear_history();
	//printf("> Cleared history\n");
	exit(g_exit_status);
}
