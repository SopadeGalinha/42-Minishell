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

static void	read_heredoc_lines(int fd, char *file, t_shell *shell)
{
	char	*line;

	while (true)
	{
		line = readline("> ");
		if (line == NULL)
		{
			ft_printf_fd(2, "minishell: warning: here-document delimited");
			ft_printf_fd(2, " by end-of-file (wanted `%s')\n", file);
			break ;
		}
		if (ft_strncmp(line, file, ft_strlen(file)) == 0
			&& ft_strlen(line) == ft_strlen(file))
		{
			free(line);
			break ;
		}
		line = expand_env(shell->env, line);
		ft_putendl_fd(line, fd);
		free(line);
	}
}

static void	handle_heredoc(char **current, t_shell *shell)
{
	int	fd;

	if (access(".heredoc", F_OK) != -1)
		unlink(".heredoc");
	fd = open(".heredoc", O_CREAT | O_RDWR | O_TRUNC, PERMISSIONS);
	if (fd == -1)
	{
		perror("minishell");
		return ;
	}
	read_heredoc_lines(fd, *current, shell);
	free(*current);
	*current = ft_strdup(".heredoc");
	close(fd);
}

void	redirects(t_token **data, t_redir **r_in, t_redir **r_out, t_shell *sh)
{
	if ((*data)->type == REDIR_OUT || (*data)->type == D_REDIR_OUT)
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
		handle_heredoc(&(*data)->next->data, sh);
		insert_redir(r_in, (*data)->next->data, (*data)->type);
		*data = (*data)->next;
	}
}
