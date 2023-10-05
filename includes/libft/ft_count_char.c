/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_count_char.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhogonca <jhogonca@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/24 23:42:29 by jhogonca          #+#    #+#             */
/*   Updated: 2023/09/24 23:42:29 by jhogonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_count_char(char *str, char c)
{
	int	i;
	int	count;

	i = -1;
	count = 0;
	while (str[++i])
		if (str[i] == c)
			count++;
	return (count);
}
