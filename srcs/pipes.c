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

int	count_cmds_in_pipeline(t_token *current)
{
	t_token	*temp;
	int		command_count;

	temp = current;
	command_count = 0;
	while (temp != NULL && temp->type != PIPELINE)
	{
		command_count++;
		temp = temp->next;
	}
	return (command_count);
}

t_pipes	*new_pipe_node(char **cmds)
{
	t_pipes	*new_pipeline;

	new_pipeline = (t_pipes *)malloc(sizeof(t_pipes));
	if (new_pipeline)
	{
		new_pipeline->cmds = cmds;
		new_pipeline->next = NULL;
	}
	return (new_pipeline);
}

void	insert_pipe_node(t_pipes **head, t_pipes *new_pipeline)
{
	t_pipes	*current;

	if (*head == NULL)
		*head = new_pipeline;
	else
	{
		current = *head;
		while (current->next != NULL)
			current = current->next;
		current->next = new_pipeline;
	}
}

void	copy_cmds_to_pipeline(char **cmds, t_token **current)
{
	int	i;

	i = 0;
	if (!cmds)
		return ;
	while (*current != NULL && (*current)->type != PIPELINE)
	{
		cmds[i] = ft_strdup((*current)->data);
		i++;
		*current = (*current)->next;
	}
	cmds[i] = NULL;
}

bool	handle_pipes(t_shell *shell)
{
	char	**cmds;
	int		c_pipes;
	t_token	*current;
	t_pipes	*new_pipeline;

	shell->pipes = NULL;
	current = shell->tokens;
	c_pipes = pipes_counter(shell->tokens) + 1;
	while (c_pipes-- > 0)
	{
		cmds = malloc((count_cmds_in_pipeline(current) + 1) * sizeof(char *));
		if (cmds == NULL)
			return (free_pipes(shell->pipes));
		new_pipeline = new_pipe_node(cmds);
		if (!new_pipe_node)
			return (free_pipes(shell->pipes));
		copy_cmds_to_pipeline(new_pipeline->cmds, &current);
		insert_pipe_node(&shell->pipes, new_pipeline);
		if (current != NULL && current->type == PIPELINE)
			current = current->next;
	}
	return (true);
}
