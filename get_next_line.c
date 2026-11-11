/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamehri <mamehri@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/06 06:08:54 by mamehri           #+#    #+#             */
/*   Updated: 2026/11/11 10:06:09 by mamehri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static int	ft_check_new_line(char *s)
{
	int	i;

	i = 0;
	if (!s)
		return (0);
	while (s[i])
		if (s[i++] == '\n')
			return (1);
	return (0);
}

static char	*ft_extract(char **stash)
{
	char	*tmp;
	size_t	len;
	size_t	i;

	i = 0;
	len = 0;
	while (!ft_is_newline(*((*stash) + len)) && *((*stash) + len))
		len++;
	if (ft_is_newline(*((*stash) + len)))
		len++;
	tmp = malloc(ft_strlen(*stash) - len + 1);
	if (!tmp)
		return (ft_free(stash));
	while (*((*stash) + i + len))
	{
		tmp[i] = *((*stash) + i + len);
		i++;
	}
	tmp[i] = '\0';
	free(*stash);
	*stash = tmp;
	return (*stash);
}

static char	*ft_ret_extract(char **stash)
{
	char	*ret;
	size_t	size;
	size_t	index;

	size = 0;
	index = 0;
	while (!ft_is_newline(stash[0][size]) && stash[0][size])
		size++;
	if (ft_is_newline(stash[0][size]))
		size++;
	ret = malloc(size + 1);
	if (!ret)
		return (ft_free(stash));
	while (index < size)
	{
		ret[index] = *((*stash) + index);
		index++;
	}
	ret[index] = '\0';
	if (!ft_extract(stash))
		return (ft_free(&ret));
	return (ret);
}

static int	ft_loop(int fd, char **stash, char *buf)
{
	char	*tmp;
	ssize_t	bytes;

	while (!ft_check_new_line(*stash))
	{
		bytes = read(fd, buf, BUFFER_SIZE);
		if (bytes < 0)
		{
			ft_free(stash);
			return (0);
		}
		else if (bytes == 0)
			break ;
		buf[bytes] = '\0';
		tmp = *stash;
		*stash = ft_strjoin(tmp, buf);
		free(tmp);
		if (!*stash)
			return (0);
	}
	return (1);
}

char	*get_next_line(int fd)
{
	static char	*stash = NULL;
	char		*buf;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (ft_free(&stash));
	buf = malloc((size_t)BUFFER_SIZE + 1);
	if (!buf)
		return (ft_free(&stash));
	if (!ft_loop(fd, &stash, buf))
		return (ft_free(&buf));
	ft_free(&buf);
	if (!stash || !*stash)
		return (ft_free(&stash));
	return (ft_ret_extract(&stash));
}
