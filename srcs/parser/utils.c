/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhogonca <jhogonca@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 16:16:15 by jhogonca          #+#    #+#             */
/*   Updated: 2024/04/07 17:12:18 by jhogonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool	is_special_char(char c)
{
	if (c == '>' || c == '<' || c == '|' || c == '$' || c == ';' || \
	c == '(' || c == ')')
		return (true);
	return (false);
}

static t_token	*create_token(char *data, int *quo_err)
{
	t_token	*new;

	new = malloc(sizeof(t_token));
	if (!new)
		return (NULL);
	if (quo_err[QUOTE] != SINGLE && quo_err[QUOTE] != DOUBLE)
		quo_err[QUOTE] = NONE;
	new->data = ft_strdup(data);
	if (!new->data)
		return (NULL);
	free(data);
	new->quote = quo_err[QUOTE];
	new->error = quo_err[ERROR];
	new->next = NULL;
	new->type = NONE;
	new->error = NONE;
	return (new);
}

void	addtoken(t_token **tokens, char *data, int *quo_err)
{
	t_token	*new;
	t_token	*last;

	if (data == NULL)
		return ;
	new = create_token(data, quo_err);
	if (new == NULL)
		return ;
	if (*tokens == NULL)
	{
		*tokens = new;
		new->prev = NULL;
		return ;
	}
	last = *tokens;
	while (last->next != NULL)
		last = last->next;
	last->next = new;
	new->prev = last;
}

void	init_builtin(const char *builtin[7])
{
	builtin[0] = "pwd";
	builtin[1] = "cd";
	builtin[2] = "echo";
	builtin[3] = "export";
	builtin[4] = "exit";
	builtin[5] = "unset";
	builtin[6] = "env";
}

int	ft_is_builtin(const char *builtin[7], char *cmd)
{
	int	i;

	if (!cmd)
		return (-1);
	i = -1;
	while (cmd && ++i < 7)
		if (ft_strncmp(builtin[i], cmd, ft_strlen(builtin[i])) == 0
			&& ft_strlen(builtin[i]) == ft_strlen(cmd))
			return (i);
	return (-1);
}
