/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isspace.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhogonca <jhogonca@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 00:00:01 by jhogonca          #+#    #+#             */
/*   Updated: 2023/09/14 00:00:01 by jhogonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

bool	ft_isspace(const char *str)
{
	int	i;

	i = -1;
	while (str[++i])
		if (!(str[i] == 32 || (str[i] >= 9 && str[i] <= 13)))
			return (false);
	return (true);
}
