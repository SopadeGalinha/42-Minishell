
#include "../includes/minishell.h"


static void addtoken(t_token **tokens, char *data, int *quo_err)
{
	t_token *new;
	t_token *last;

	if (data == NULL)
		return ;
	new = malloc(sizeof(t_token));
	if (!new)
		return ;
	if (quo_err[QUOTE] != SINGLE && quo_err[QUOTE] != DOUBLE)
		quo_err[QUOTE] = NONE;
	new->data = ft_strdup(data);
	free(data);
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

bool	is_special_char(char c)
{
	if (c == '>' || c == '<' || c == '|' || c == '$' || c == ';')
		return (true);
	return (false);
}

static int	cmds_data(char *input, int i, int start, t_token **tokens)
{
	char	*data;

	if ((input[i] == '>' || input[i] == '<')
		|| (input[i] == '|' || input[i] == '&') || input[i] == ';')
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
		while (input[++i] != '\0' && !is_special_char(input[i])
			&& input[i] != '"' && input[i] != '\''
			&& input[i] != ' ' && input[i])
				;
		data = ft_substr(input, start, (i-- - start));
	}
	if ((ft_strncmp(data, "&", 1)) == 0 && ft_strlen(data) == 1)
		addtoken(tokens, data, (int []){NONE, BACKGROUND_NOT_SUPPORTED});
	else if (ft_strncmp(data, "||", 2) == 0 && ft_strlen(data) == 2)
		addtoken(tokens, data, (int []){NONE, D_PIPELINE_NOT_SUPPORTED});
	else if (ft_strncmp(data, ";", 1) == 0 && ft_strlen(data) == 1)
		addtoken(tokens, data, (int []){NONE, SEMICOLON_NOT_SUPPORTED});
	else
		addtoken(tokens, data, (int []){NONE, NO_ERROR});
	return (i);
}

char	*remove_quotes(char *data)
{
	int		i;
	int		start;
	int		quote_type;
	char	*aux;
	char	*tmp;
	char	*result;

	i = -1;
	start = 0;
	quote_type = 0;
	result = ft_strdup("");
	while (data[++i])
	{
		if (!quote_type && (data[i] == '"' || data[i] == '\'') || (quote_type == data[i]))
		{

			if (quote_type == data[i])
				quote_type = 0;
			else
				quote_type = data[i];
			aux = ft_substr(data, start, i - start);
			tmp = ft_strjoin(result, aux);
			free(result);
			free(aux);
			result = tmp;
			start = i + 1;
		}
	}
	aux = ft_substr(data, start, i - start);
	tmp = ft_strjoin(result, aux);
	free(result);
	free(aux);
	result = tmp;
	free(data);
	return (result);
}

bool	lexical(char *input, t_token **tokens)
{
	int		i;
	int		start;
	int		error;
	char	*data;
	char	quote;
	int		quote_type;

	i = -1;
	error = NO_ERROR;
	while (++i < (int)strlen(input))
	{
		quote = 0;
		start = i;
		quote_type = 0;
		if (is_special_char(input[i]))
		{
			i = cmds_data(input, i, start, tokens);
			continue ;
		}
		else
		{
			while (ft_isspace(input[i]))
			{
				i++;
				start++;
			}
			while (input[i] && (!ft_isspace(input[i]) || quote != 0 ))
			{
				if (input[i] == '"' || input[i] == '\'')
				{
					quote_type = input[i];
					if (quote == 0)
						quote = input[i];
					else if (quote == input[i])
						quote = 0;
				}
				i++;
			}
			if (quote != 0)
				error = UNCLOSED_QUOTE;
			data = ft_substr(input, start, i - start);
			data = remove_quotes(data);
			if (quote != 0)
				error = UNCLOSED_QUOTE;
			addtoken(tokens, data, (int []){quote_type, error});
		}
	}
	return (error);
}
