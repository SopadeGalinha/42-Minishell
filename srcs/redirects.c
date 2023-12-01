/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhogonca <jhogonca@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/28 09:59:48 by jhogonca          #+#    #+#             */
/*   Updated: 2023/10/28 09:59:48 by jhogonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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
