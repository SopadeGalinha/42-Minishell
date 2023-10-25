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

void	insert_redir(t_pipes *pipes, char *data, int type, int io)
{
	t_redir	*redir;
	t_redir	*tmp;

	if (data == NULL)
		return ;
	redir = ft_calloc(1, sizeof(t_redir));
	if (redir == NULL)
		return (NULL);
	redir->type = type;
	redir->file = ft_strdup(data);
	
	// criar o no do redir, e adicionar ao fim da lista
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
	pipes.cmds = ft_calloc(i + 1, sizeof(char *));
	pipes.cmds[i] = NULL;
	i = 0;
	while (*current != NULL && (*current)->type != PIPELINE)
	{
		if ((*current)->type == REDIR_IN || (*current)->type == HEREDOC)
		{
			(*current) = (*current)->next;
			if (!(*current)->type)
				return (NULL);
			insert_redir(&pipes, (*current)->data, (*current)->type, IN);
			(*current) = (*current)->next;
			continue ;
		}
		else if ((*current)->type == REDIR_OUT || (*current)->type == D_REDIR_OUT)
		{
			(*current) = (*current)->next;
			if (!(*current)->type)
				return (NULL);
			insert_redir(&pipes, (*current)->data, (*current)->type, OUT);
			(*current) = (*current)->next;
			continue ;
		}
		else 
			pipes.cmds[i++] = ft_strdup((*current)->data);
		*current = (*current)->next;
	}
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
