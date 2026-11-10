/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamehri <mamehri@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/09 14:49:27 by mamehri           #+#    #+#             */
/*   Updated: 2026/11/10 18:47:20 by mamehri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

static t_list	*ft_find(t_list **lst, int fd)
{
	t_list	*new;
	t_list	*current;

	current = *lst;
	while (current && current->next && current->fd != fd)
		current = current->next;
	if (current && current->fd == fd)
		return (current);
	new = malloc(sizeof(t_list));
	if (!new)
		return (NULL);
	new -> fd = fd;
	new -> content = NULL;
	new -> next = NULL;
	if (!*lst)
		*lst = new;
	else
		current->next = new;
	return (new);
}

static void	ft_extract(char **stash)
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
	{
		free(*stash);
		*stash = NULL;
		return ;
	}
	while (*((*stash) + i + len))
	{
		tmp[i] = *((*stash) + i + len);
		i++;
	}
	tmp[i] = '\0';
	free(*stash);
	*stash = tmp;
}

static char	*ft_ret_extract(t_list **l, int f)
{
	char	*ret;
	size_t	len;
	size_t	index;

	if (!ft_find(l, f))
		return (ft_lstfree(l, f));
	if (!ft_find(l, f)->content || !*ft_find(l, f)->content)
		return (ft_lstfree(l, f));
	len = 0;
	index = 0;
	while (ft_find(l, f)->content[len] != '\n' && ft_find(l, f)->content[len])
		len++;
	if (ft_find(l, f)->content[len] == '\n')
		len++;
	ret = malloc(len + 1);
	if (!ret)
		return (ft_lstfree(l, f));
	while (index < len)
	{
		ret[index] = ft_find(l, f)->content[index];
		index++;
	}
	ret[index] = '\0';
	ft_extract(&(ft_find(l, f)->content));
	return (ret);
}

static int	ft_loop(int fd, t_list **lst, char *buf)
{
	char	*tmp;
	t_list	*stash;
	ssize_t	bytes;

	stash = ft_find(lst, fd);
	while (!ft_check_new_line(stash->content))
	{
		bytes = read(fd, buf, BUFFER_SIZE);
		if (bytes < 0)
			return (0);
		else if (bytes == 0)
			break ;
		buf[bytes] = '\0';
		tmp = stash->content;
		stash->content = ft_strjoin(tmp, buf);
		free(tmp);
		tmp = NULL;
	}
	return (1);
}

char	*get_next_line(int fd)
{
	static t_list	*lst;
	char			*buf;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	buf = malloc(BUFFER_SIZE + 1);
	if (!buf || !ft_loop(fd, &lst, buf))
	{
		free(buf);
		buf = NULL;
		return (ft_lstfree(&lst, fd));
	}
	free(buf);
	buf = NULL;
	return (ft_ret_extract(&lst, fd));
}
