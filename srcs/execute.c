/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heolivei <heolivei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/09 18:18:41 by jhogonca          #+#    #+#             */
/*   Updated: 2023/11/01 16:22:15 by heolivei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	token_checker(char *str)
{
	int	i = 0;

	if (ft_isdigit(str[0]))
	{
		printf("nao pode colocar numero no inicio na key vacilao\n");
		return (-1);
	}
	else if (str[0] == '_' && str[1])
	{
		if(!ft_isalnum(str[1]) && str[1] != '_')
		{
			printf("Depois do UNDERLINE so pode numero, letra ou outro UNDELINE\n");
			return (-1);
		}
		else
			return (1);
	}
	else if (str[0] == '_' && (str[1] == '=' || str[1] == '\0'))
	{
		printf("Apenas UNDERLINE o codigo nao faz nada\n");
		return (-2);
	}
	return (1);
}

void	ft_export(t_shell *shell)
{
	int	i = 1;

	if (shell->pipes->cmds[i])
	{
		while (shell->pipes->cmds[i])
		{
			if(token_checker(shell->pipes->cmds[i]) > 0)
			{
				if (ft_strchr(shell->pipes->cmds[i], '='))
					update_lists(shell, shell->pipes->cmds[i], 1);
				else if (!ft_strchr(shell->pipes->cmds[i], '='))
					update_lists(shell, shell->pipes->cmds[i], 0);
			}
			i++;
		}
	}
    else
        print_list(shell, 0);
}

void	execute(t_shell *shell)
{
	t_pipes		*pipes;
	int			function;
	const char	*builtin[3];

	builtin[0] = "pwd";
	builtin[1] = "cd";
	builtin[2] = "echo";
	function = -1;
	pipes = shell->pipes;

}

