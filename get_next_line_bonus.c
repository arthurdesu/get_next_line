/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamehri <mamehri@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/09 14:49:27 by mamehri           #+#    #+#             */
/*   Updated: 2026/11/11 11:21:03 by mamehri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

static t_list	*ft_find(t_list **lst, int fd)
{
	t_list	*node;
	t_list	*current;

	current = *lst;
	while (current && current->next && current->fd != fd)
		current = current->next;
	if (current && current->fd == fd)
		return (current);
	node = malloc(sizeof(t_list));
	if (!node)
		return (NULL);
	node -> fd = fd;
	node -> stash = NULL;
	node -> next = NULL;
	if (!*lst)
		*lst = node;
	else
		current->next = node;
	return (node);
}

static char	*ft_extract(char **stash)
{
	char	*tmp;
	size_t	len;
	size_t	i;

	i = 0;
	len = 0;
	while (*((*stash) + len) != '\n' && *((*stash) + len))
		len++;
	if (*((*stash) + len) == '\n')
		len++;
	tmp = malloc(ft_strlen(*stash) - len + 1);
	if (!tmp)
		return (NULL);
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

static char	*ft_ret_extract(t_list **lst, int fd)
{
	t_list	*node;
	char	*ret;
	size_t	len;
	size_t	index;

	len = 0;
	index = 0;
	node = ft_find(lst, fd);
	while (node->stash[len] != '\n'
		&& node->stash[len])
		len++;
	if (node->stash[len] == '\n')
		len++;
	ret = malloc(len + 1);
	if (!ret)
		return (ft_lstfree(lst, fd));
	while (index < len)
	{
		ret[index] = node->stash[index];
		index++;
	}
	ret[index] = '\0';
	if (!ft_extract(&(node->stash)))
		return (ft_lstfree(lst, fd));
	return (ret);
}

static int	ft_loop(int fd, t_list **lst, char *buf)
{
	char	*tmp;
	t_list	*node;
	ssize_t	bytes;

	node = ft_find(lst, fd);
	if (!node)
		return (0);
	while (!ft_check_new_line(node->stash))
	{
		bytes = read(fd, buf, BUFFER_SIZE);
		if (bytes < 0)
			return (0);
		else if (bytes == 0)
			return (1);
		buf[bytes] = '\0';
		tmp = node->stash;
		node->stash = ft_strjoin(tmp, buf);
		free(tmp);
		if (!node->stash)
			return (0);
	}
	return (1);
}

char	*get_next_line(int fd)
{
	static t_list	*lst = NULL;
	t_list			*node;
	char			*buf;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	buf = malloc((size_t)BUFFER_SIZE + 1);
	if (!buf || !ft_loop(fd, &lst, buf))
	{
		free(buf);
		return (ft_lstfree(&lst, fd));
	}
	free(buf);
	node = ft_find(&lst, fd);
	if (!node->stash || !*node->stash)
		return (ft_lstfree(&lst, fd));
	return (ft_ret_extract(&lst, fd));
}
