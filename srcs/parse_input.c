/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhogonca <jhogonca@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 15:23:53 by jhogonca          #+#    #+#             */
/*   Updated: 2023/10/05 15:23:53 by jhogonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
void	lexical(char *input, t_token **tokens);

void	get_cmd(t_token **tokens, char *path_env)
{
	t_token	*current;
	int		i;
	char	*path;
	char	**split_path;
	current = *tokens;

	i = -1;
	split_path = ft_split(path_env, ':');
	while (current != NULL)
	{
		i = -1;
		while (split_path[++i])
		{
			path = ft_strjoin(ft_strjoin(split_path[i], "/"), current->data);
			if (access(path, F_OK) == 0)
				if (access(path, X_OK) == 0)
				{
					current->type = CMD;
					free(path);
					break ;
				}
			free(path);
		}
		current = current->next;
	}
}
char	*ft_getenv(char **env, char *var_name)
{
	int		i;
	char	*tmp;

	i = -1;
	while (env[++i] != NULL)
		if (ft_strncmp(env[i], var_name, ft_strlen(var_name)) == 0)
			return (ft_substr(env[i], ft_strlen(var_name) + 1, ft_strlen(env[i])));
	return (NULL);
}

bool	parse_input(char *input, char *path_env, t_token **tokens)
{
	bool error;
	t_token	*current;

	lexical(input, tokens);
	current = *tokens;
	while (current)
	{
		current->type = define_token(current->data);
		current = current->next;
	}
	get_cmd(tokens, path_env);
	print_tokens(*tokens);
	return (true);
}

static void addtoken(t_token **tokens, char *data, int *quo_err)
{
    t_token *new;
    t_token *last;

    new = malloc(sizeof(t_token));
    new->data = data;
    new->quote = quo_err[QUOTE];
	new->error = quo_err[ERROR];
    new->next = NULL;
    if (*tokens == NULL)
    {
        *tokens = new;
        new->prev = NULL;
        return;
    }
    last = *tokens;
    while (last->next != NULL)
        last = last->next;
    last->next = new;
    new->prev = last;
}

static int	cmds_data(char *input, int i, int start, t_token **tokens)
{
	char	*data;

	if (input[i] == '>' || input[i] == '<' || input[i] == '|')
	{
		if (input[i] == input[i + 1])
		{
			data = ft_substr(input, start, 2);
			i += 1;
		}
		else
			data = ft_substr(input, start, 1);
	}
	else
	{
		while (input[++i] != '\0' && input[i] != ' ' && input[i] \
			!= '>' && input[i] != '<' && input[i] != '|' && input[i] != '$')
			;
		data = ft_substr(input, start, i - start);
	}
	addtoken(tokens, data, (int []){NONE, NO_ERROR});
	return (i);
}

static int	quote_data(char *input, int i, int start, t_token **tokens)
{
	char	*data;
	int		quote;
	int		ref;
	int		quo_err[2];

	ref = i - 1;
	quo_err[QUOTE] = SINGLE;
	quo_err[ERROR] = NO_ERROR;
	while (input[i] != '\0' && input[i] != input[ref])
		i++;
	data = ft_substr(input, start + 1, i - start - 1);
	if (input[ref] == '"')
		quo_err[QUOTE] = DOUBLE;
	if (input[i] == '\0')
		quo_err[ERROR] = UNCLOSED_QUOTE;
	if (ft_strlen(data) > 0)
		addtoken(tokens, data, quo_err);
	return (i);
}

static int	general_data(char *input, int i, int start, t_token **tokens)
{
	char	*data;

	while (input[i] && input[i] != ' ' && input[i] \
		!= '>' && input[i] != '<' && input[i] != '|' && input[i] != '$')
		i++;
	data = ft_substr(input, start, i - start);
	addtoken(tokens, data, (int []){NONE, NO_ERROR});
	return (i);
}

void	lexical(char *input, t_token **tokens)
{
	int		i;
	int		start;

	i = -1;
	while (++i <= (int)strlen(input) - 1)
	{
		start = i;
		if ((input[i] == '"' || input[i] == '\''))
			i = quote_data(input, ++i, start, tokens);
		else if (input[i] == '>' || input[i] == '<' || input[i] == '|'
			|| input[i] == '$')
			i = cmds_data(input, i, start, tokens);
		else if (input[i] == '2' && input[i + 1] == '>')
		{
			i++;
			addtoken(tokens, ft_substr(input, start, 2), (int []){NONE, NO_ERROR});
		}
		else if (input[i] == ' ' || input[i] == '\t' || input[i] == '\n')
			continue ;
		else
			i = general_data(input, i, start, tokens);
	}
}
