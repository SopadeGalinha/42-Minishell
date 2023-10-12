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
	free(*tokens);
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

void	free_struct(t_shell *shell, int	running)
{
	free_tokens(&shell->tokens);
	shell->error = NO_ERROR;
	if (running == 0)
		return ;
	free(shell->path_env);
	free_env(shell->env);
	free_env(shell->exp);
}
