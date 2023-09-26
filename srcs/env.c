
#include "../includes/minishell.h"

char	*ft_strdup_equal_value(const char *src)
{
	char	*cpy;
	size_t	i;
	size_t	j;
	size_t	k;

	i = 0;
	j = 0;
	while (src[i] != '=')
		i++;
	k = ++i;
	while (src[i++] != '\0')
		j++;
	cpy = (char *)malloc(sizeof(char) * (j + 1));
	if (!cpy)
		return (NULL);
	j = 0;
	while (src[k] != '\0')
		cpy[j++] = src[k++];
	cpy[j] = '\0';
	return (cpy);
}

char	*ft_strdup_equal_key(const char *src)
{
	char	*cpy;
	size_t	i;

	i = 0;
	while (src[i] != '=')
		i++;
	cpy = (char *)malloc(sizeof(char) * i + 1);
	if (!cpy)
		return (NULL);
	i = 0;
	while (*src != '=')
		cpy[i++] = *src++;
	cpy[i] = '\0';
	return (cpy);
}
