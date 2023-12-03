/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_array.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhogonca <jhogonca@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 01:23:16 by jhogonca          #+#    #+#             */
/*   Updated: 2023/09/14 01:23:16 by jhogonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_free_2d_array(void **array_ptr, int size)
{
	int	i;

	i = -1;
	if (!array_ptr)
		return ;
	while (++i < size)
		if (array_ptr[i])
			free(array_ptr[i]);
	while (size == 0 && array_ptr[i])
		free(array_ptr[i++]);
	free(array_ptr);
}
