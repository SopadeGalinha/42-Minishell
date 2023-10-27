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

int	pipes_counter(t_token *tokens)
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

void	insert_redir(t_redir *head, char *data, int type)
{
	t_redir	*new;
	t_redir	*tmp;

	new = malloc(sizeof(t_redir));
	if (new == NULL)
		return ;
	new->type = type;
	new->file = ft_strdup(data);
	if (head == NULL)
		head = new;
	else
	{
		tmp = head;
		while (tmp->next != NULL)
			tmp = tmp->next;
		tmp->next = new;
	}
}

void	printfredir(t_redir *redir)
{
	while (redir != NULL)
	{
		printf("type: %d\n", redir->type);
		printf("file: %s\n", redir->file);
		redir = redir->next;
	}
}

t_pipes	*copy_tokens_to_pipeline(t_token **current)
{
	t_pipes	pipes;
	t_token	*tmp;
	int		i;

	i = -1;
	tmp = *current;
	while (tmp != NULL && tmp->type != PIPELINE && ++i != -2)
		tmp = tmp->next;
	pipes.cmds = malloc(sizeof(char *) * (i + 1));
	pipes.cmds[i] = NULL;
	i = 0;
	while (*current != NULL && (*current)->type != PIPELINE)
	{
		if ((*current)->type == REDIR_OUT || (*current)->type == D_REDIR_OUT)
		{
			(*current) = (*current)->next;
			if ((*current) == NULL)
				return (NULL);
			insert_redir(pipes.redir_out, (*current)->data, (*current)->prev->type);
		}
		else 
			pipes.cmds[i++] = ft_strdup((*current)->data);
		*current = (*current)->next;
	}
	printfredir(pipes.redir_out);
	exit(0);
}

bool	create_pipeline_node(t_shell *shell)
{
	t_token	*current;
	int		c_pipes;

	current = shell->tokens;
	c_pipes = pipes_counter(shell->tokens) + 1;
	while (c_pipes-- > 0)
	{
		shell->pipes = copy_tokens_to_pipeline(&current);
		exit(0);
		if (current != NULL && current->type == PIPELINE)
			current = current->next;
	}
	return (true);
}
