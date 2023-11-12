/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhogonca <jhogonca@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 12:13:54 by jhogonca          #+#    #+#             */
/*   Updated: 2023/10/22 12:13:54 by jhogonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	pipes_counter(t_token *tokens)
{
	int		i;
	t_token	*current;

	i = 0;
	current = tokens;
	while (current != NULL)
	{
		if (current->type == PIPELINE)
			i++;
		current = current->next;
	}
	return (i);
}

static int	is_redirect(t_token *current)
{
	return (current->type == REDIR_OUT || current->type == D_REDIR_OUT
		|| current->type == REDIR_IN || current->type == HEREDOC);
}

static void	*copy_tokens_to_pipeline(t_token **current)
{
	int		i;
	t_token	*tmp;
	t_pipes	*pipes;
	int		index;

	i = -1;
	index = 0;
	tmp = *current;
	pipes = ft_calloc(1, sizeof(t_pipes));
	while (tmp != NULL && tmp->type != PIPELINE && ++i != -2)
		tmp = tmp->next;
	pipes->cmds = ft_calloc(i + 2, sizeof(char *));
	if (pipes->cmds == NULL)
		return (NULL);
	i = 0;
	while (*current != NULL && (*current)->type != PIPELINE)
	{
		index++;
		if (is_redirect(*current))
			handle_redirects(current, &pipes->redir_in, &pipes->redir_out);
		else
			pipes->cmds[i++] = ft_strdup((*current)->data);
		*current = (*current)->next;
	}
	return (pipes);
}

static void	add_node_to_pipeline(t_pipes **head, t_pipes *new)
{
	t_pipes	*tmp;

	if (*head == NULL)
		*head = new;
	else
	{
		tmp = *head;
		while (tmp->next != NULL)
			tmp = tmp->next;
		tmp->next = new;
	}
}

bool	create_pipeline_node(t_shell *shell)
{
	int		aux[2];
	t_token	*current;
	t_pipes	*head;
	t_pipes	*new_pipe;

	aux[0] = 0;
	head = NULL;
	new_pipe = NULL;
	current = shell->tokens;
	aux[1] = pipes_counter(shell->tokens) + 1;
	while (aux[1]-- > 0)
	{
		new_pipe = copy_tokens_to_pipeline(&current);
		if (new_pipe == NULL)
			return (false);
		new_pipe->next = NULL;
		new_pipe->id = ++aux[0];
		add_node_to_pipeline(&head, new_pipe);
		if (current != NULL && current->type == PIPELINE)
			current = current->next;
	}
	shell->pipes = head;
	return (true);
}