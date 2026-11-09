/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamehri <mamehri@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/09 14:49:27 by mamehri           #+#    #+#             */
/*   Updated: 2026/11/09 18:48:52 by mamehri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

static int	ft_check_new_line(char *s)
{
	size_t	i;

	i = 0;
	if (!s)	
		return (0);
	while (s[i])
	{
		if (s[i] == '\n')
			return (1);
		i++;
	}
	return (0);
}

static char	*ft_extract(char **stash)
{
	char	*tmp;
	size_t	len;
	size_t	i;

	i = 0;
	len = 0;
	while (!ft_new_line(stash[0][len]) && stash[0][len])
		len++;
	if (ft_new_line(stash[0][len]))
		len++;
	tmp = malloc(ft_strlen(*stash) - len + 1);
	if (!tmp)
		return (ft_free(stash));
	while (stash[0][len + i])
	{
		tmp[i] = stash[0][len + i];
		i++;
	}
	tmp[i] = '\0';
	free(*stash);
	*stash = tmp;
	return (*stash);
}

static char	*ft_ret_extract(char **stash)
{
	size_t	i;
	size_t	len;
	char	*ret;

	if (!*stash || !**stash)
		return (NULL);
	i = 0;
	len = 0;
	while (!ft_new_line(stash[0][len]) && stash[0][len])
		len++;
	if (ft_new_line(stash[0][len]))
		len++;
	ret = malloc(len + 1);
	if (!ret)
		return (ft_free(stash));
	while (i < len)
	{
		ret[i] = stash[0][i];
		i++;
	}
	ret[i] = '\0';
	ft_extract(stash);
	return (ret);
}

static int	ft_loop(char **stash, char *buf, int fd)
{
	char	*tmp;
	ssize_t	bytes;

	while (!ft_check_new_line(*stash))
	{
		bytes = read(fd, buf, BUFFER_SIZE);
		if (bytes < 0)
			return (0);
		else if (bytes == 0)
			return (1);
		buf[bytes] = '\0';
		tmp = *stash;
		*stash = ft_strjoin(tmp, buf);
		ft_free(&tmp);
	}
	return (1);
}

char	*get_next_line(int fd)
{
	static char	*stash[1024];
	char		*buf;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	buf = malloc(BUFFER_SIZE + 1);
	if (!buf)
		return (ft_free(&stash[fd]));
	if (!ft_loop(&stash[fd], buf, fd))
		return (ft_free(&buf));
	ft_free(&buf);
	return (ft_ret_extract(&stash[fd]));
}
