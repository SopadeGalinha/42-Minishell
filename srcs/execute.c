/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heolivei <heolivei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/09 18:18:41 by jhogonca          #+#    #+#             */
/*   Updated: 2023/11/01 16:34:08 by heolivei         ###   ########.fr       */
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

void	execute(t_shell *shell)
{
	t_pipes		*pipes;
	int			function;
	const char	*builtin[7];

	builtin[0] = "pwd";
	builtin[1] = "cd";
	builtin[2] = "echo";
	builtin[3] = "export";
	builtin[4] = "exit";
	builtin[5] = "unset";
	builtin[6] = "env";

	function = -1;
	pipes = shell->pipes;
	while (++function < 7)
	{
		if (ft_strncmp(pipes->cmds[0], builtin[function], \
		ft_strlen(builtin[function])) == 0
			&& ft_strlen(pipes->cmds[0]) == ft_strlen(builtin[function]))
			break ;
	}
	if (function < 7)
		shell->builtin[function](shell);
	else
		ft_printf_fd(shell->std_out, "Not a builtin\n");
}
