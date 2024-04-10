/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhogonca <jhogonca@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 16:56:22 by jhogonca          #+#    #+#             */
/*   Updated: 2024/04/08 19:25:22 by jhogonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	insert_redir(t_redir **head, char *data, int type)
{
	t_redir	*new;
	t_redir	*tmp;

	new = ft_calloc(1, sizeof(t_redir));
	if (new == NULL)
		return ;
	new->type = type;
	new->file = ft_strdup(data);
	new->next = NULL;
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

void	redirects(t_token **data, t_redir **r_in, t_redir **r_out)
{
	if ((*data)->type == REDIR_OUT || (*data)->type == APPEND)
	{
		insert_redir(r_out, (*data)->next->data, (*data)->type);
		*data = (*data)->next;
	}
	else if ((*data)->type == REDIR_IN)
	{
		insert_redir(r_in, (*data)->next->data, (*data)->type);
		*data = (*data)->next;
	}
	else if ((*data)->type == HEREDOC)
	{
		insert_redir(r_in, (*data)->next->data, HEREDOC);
		*data = (*data)->next;
	}
}

static void	*copy_tokens_to_pipeline(t_token **current)
{
	int		i;
	t_token	*tmp;
	t_pipes	*pipes;

	i = -1;
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
		if ((*current)->type == REDIR_OUT || (*current)->type == APPEND \
		|| (*current)->type == REDIR_IN || (*current)->type == HEREDOC)
			redirects(current, &pipes->redir_in, &pipes->redir_out);
		else
			pipes->cmds[i++] = ft_strdup((*current)->data);
		pipes->type = (*current)->type;
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
	aux[1] = count_pipes(shell->tokens) + 1;
	while (aux[1]-- > 0)
	{
		new_pipe = copy_tokens_to_pipeline(&current);
		if (new_pipe == NULL)
			return (print_error("minishell: malloc error", 1));
		new_pipe->next = NULL;
		new_pipe->redir_fd[IN] = -1;
		new_pipe->redir_fd[OUT] = -1;
		add_node_to_pipeline(&head, new_pipe);
		if (current != NULL && current->type == PIPELINE)
			current = current->next;
	}
	shell->pipes = head;
	return (true);
}
