/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhogonca <jhogonca@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 17:07:11 by jhogonca          #+#    #+#             */
/*   Updated: 2024/04/07 17:08:05 by jhogonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static bool	heredoc_validate(char *line, char *target)
{
	if (!line)
	{
		ft_printf_fd(STDERR_FILENO, MS_ERR RESET"warning: here-document");
		ft_printf_fd(STDERR_FILENO, \
		" delimited by end-of-file (wanted `%s')\n", target);
		return (false);
	}
	if (ft_strcmp(line, target) == 0)
	{
		free(line);
		return (false);
	}
	return (true);
}

void	heredoc(char *target, t_pipes *current, t_shell *shell)
{
	char	*line;
	char	*aux;

	line = NULL;
	aux = NULL;
	current->do_heredoc = true;
	if (current->heredoc)
		free(current->heredoc);
	current->heredoc = ft_strdup("");
	while (true)
	{
		line = readline(BOLD_GREEN"heredoc> "RESET);
		if (heredoc_validate(line, target) == false)
			break ;
		line = expand_env(shell->env, line);
		aux = ft_strjoin(line, "\n");
		free(line);
		line = ft_strjoin(current->heredoc, aux);
		free(aux);
		free(current->heredoc);
		current->heredoc = line;
	}
}
