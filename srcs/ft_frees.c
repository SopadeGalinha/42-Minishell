/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_frees.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhogonca <jhogonca@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/08 04:53:48 by jhogonca          #+#    #+#             */
/*   Updated: 2023/10/08 04:53:48 by jhogonca         ###   ########.fr       */
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

bool	free_pipes(t_pipes *pipes)
{
	t_pipes	*current_pipeline;

	if (pipes == NULL)
		return (true);
	current_pipeline = pipes;
	while (current_pipeline != NULL)
	{
		ft_free_redirect(&current_pipeline->redir_in);
		ft_free_redirect(&current_pipeline->redir_out);
		ft_free_array(current_pipeline->cmds);
		close(current_pipeline->pipe[0]);
		close(current_pipeline->pipe[1]);
		current_pipeline = current_pipeline->next;
	}
	free(pipes);

	return (true);
}

static void	free_tokens(t_token **tokens)
{
	t_token	*current;
	t_token	*tmp;

	if (tokens == NULL)
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

static void free_env(t_env **env)
{
	t_env	*current;
	t_env	*tmp;

	if (env == NULL)
		return;
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
 	if (shell->input != NULL)
		free(shell->input);
/*
	if (shell->pipes != NULL)
		free_pipes(shell->pipes);
	if (shell->tokens != NULL) */
	free_tokens(&(shell->tokens));
	shell->error = NO_ERROR;
	if (running == 0)
		return ;
	free_env(&shell->env);
	free_env(&shell->exp);
	free(shell->oldpwd);
	close(shell->std_in);
	close(shell->std_out);
	ft_printf_fd(1, "Bye bye!\n");
	exit(g_exit_status);
}
