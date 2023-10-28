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

bool	free_pipes(t_pipes *pipes)
{
	t_pipes	*current_pipeline;

	while (pipes != NULL)
	{
		current_pipeline = pipes;
		pipes = pipes->next;
		free(current_pipeline);
	}
	return (false);
}

static void	free_tokens(t_token **tokens)
{
	t_token	*current;
	t_token	*tmp;

	current = *tokens;
	while (current != NULL)
	{
		tmp = current->next;
		free(current->data);
		free(current);
		current = tmp;
	}
	*tokens = NULL;
}

static void	free_env(t_env *env)
{
	t_env	*current;
	t_env	*tmp;

	current = env;
	while (current != NULL)
	{
		tmp = current->next;
		free(current->key);
		free(current->value);
		free(current->line);
		free(current);
		current = tmp;
	}
	env = NULL;
}

void	free_struct(t_shell *shell, int running)
{
	free(shell->input);
	free(shell->pipes);
	free_tokens(&shell->tokens);
	shell->error = NO_ERROR;
	if (running == 0)
		return ;
	free_env(shell->env);
	free_env(shell->exp);
}
