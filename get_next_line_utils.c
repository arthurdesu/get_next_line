/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamehri <mamehri@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/06 06:11:31 by mamehri           #+#    #+#             */
/*   Updated: 2026/11/11 09:44:51 by mamehri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int	ft_is_newline(char c)
{
	return (c == '\n');
}

char	*ft_free(char **s)
{
	free(*s);
	*s = NULL;
	return (NULL);
}

size_t	ft_strlen(const char *s)
{
	size_t	len;

	len = 0;
	while (s[len])
		len++;
	return (len);
}

char	*ft_strdup(char *s)
{
	char	*dup;
	int		i;

	i = 0;
	dup = malloc(ft_strlen(s) + 1);
	if (!dup)
		return (NULL);
	while (s[i])
	{
		dup[i] = s[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}

char	*ft_strjoin(char *stash, char *buf)
{
	int		i;
	int		j;
	char	*join;

	if (!stash && !buf)
		return (NULL);
	else if (!stash)
		return (ft_strdup(buf));
	else if (!buf)
		return (ft_strdup(stash));
	i = 0;
	j = 0;
	join = malloc(ft_strlen(stash) + ft_strlen(buf) + 1);
	if (!join)
		return (NULL);
	while (stash[i])
		join[j++] = stash[i++];
	i = 0;
	while (buf[i])
		join[j++] = buf[i++];
	join[j] = '\0';
	return (join);
}
