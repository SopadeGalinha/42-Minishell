/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_tokens.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhogonca <jhogonca@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 16:46:10 by jhogonca          #+#    #+#             */
/*   Updated: 2024/04/07 16:52:18 by jhogonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*expand_tilde(t_shell *shell, t_token *current)
{
	int		ref;
	char	*home;
	char	*new_token;

	if (ft_strncmp(current->data, "~/", 2) == 0)
		ref = 2;
	else
		ref = 1;
	home = get_env_value(shell->env, "HOME");
	if (home == NULL)
		return (current->data);
	new_token = ft_strjoin(home, "/");
	free(home);
	home = ft_substr(current->data, ref, ft_strlen(current->data) - ref);
	free(current->data);
	current->data = ft_strjoin(new_token, home);
	free(home);
	free(new_token);
	return (current->data);
}

static int	define_token(const char *token)
{
	if (ft_strncmp(token, "|", ft_strlen("|")) == 0 && ft_strlen(token) == 1)
		return (PIPELINE);
	if (ft_strncmp(token, ">", ft_strlen(">")) == 0 && ft_strlen(token) == 1)
		return (REDIR_OUT);
	if (ft_strncmp(token, "<", ft_strlen("<")) == 0 && ft_strlen(token) == 1)
		return (REDIR_IN);
	if (ft_strncmp(token, ">>", ft_strlen(">>")) == 0 && ft_strlen(token) == 2)
		return (APPEND);
	if (ft_strncmp(token, "<<", ft_strlen("<<")) == 0 && ft_strlen(token) == 2)
		return (HEREDOC);
	if (ft_strncmp(token, "2>", ft_strlen("2>")) == 0 && ft_strlen(token) == 2)
		return (REDIR_ERR);
	if (ft_strncmp(token, "$?", ft_strlen("$?")) == 0 && ft_strlen(token) == 2)
		return (EXIT_STATUS);
	if (ft_strncmp(token, "&&", ft_strlen("&&")) == 0 && ft_strlen(token) == 2)
		return (AND);
	if (ft_strncmp(token, "||", ft_strlen("||")) == 0 && ft_strlen(token) == 2)
		return (OR);
	if (ft_strncmp(token, "(", ft_strlen("(")) == 0 && ft_strlen(token) == 2)
	{
		printf("Retornou Parenthesis open\n");
		return (PARENTHESIS_OPEN);
	}
	if (ft_strncmp(token, ")", ft_strlen(")")) == 0 && ft_strlen(token) == 2)
	{
		printf("Retornou Parenthesis close\n");
		return (PARENTHESIS_CLOSE);
	}
	if (ft_strncmp(token, ";", ft_strlen(";")) == 0)
		return (SEMICOLON);
	if (token[0] == '$')
		return (ENV);	
	return (WORD);
}

static bool	process_aux(t_shell *shell, t_token *current)
{
	if (current->quote != SINGLE)
	{
		if ((current->prev != NULL && current->prev->type == HEREDOC))
			;
		else if (current->type == EXIT_STATUS)
		{
			free(current->data);
			current->data = ft_itoa(g_exit_status);
			if (current->data == NULL)
				return (print_error("minishell: malloc error", 1));
		}
		else
			current->data = expand_env(shell->env, current->data);
		if (current->data == NULL)
			return (print_error("minishell: malloc error", 1));
		if (ft_strncmp(current->data, "~", 1) == 0)
			current->data = expand_tilde(shell, current);
	}
	return (true);
}

bool	process_tokens(t_shell *shell)
{
	t_token	*current;

	current = shell->tokens;
	while (current != NULL)
	{
		if (current->quote != NONE)
			current->type = WORD;
		else
			current->type = define_token(current->data);
		if (!process_aux(shell, current))
			return (false);
		if ((current->type == OR || current->type == SEMICOLON
				|| current->type == AND || current->type == PARENTHESIS_OPEN\
				 || current->type == PARENTHESIS_CLOSE) && \
				 current->quote == NONE)
			return (print_error(MS_ERR UNSUP_MCMDS, 1));
		current = current->next;
	}
	return (true);
}
