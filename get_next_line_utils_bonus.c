/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamehri <mamehri@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/09 14:49:36 by mamehri           #+#    #+#             */
/*   Updated: 2026/11/10 18:47:28 by mamehri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	if (!s)
		return (i);
	while (s[i])
		i++;
	return (i);
}

int	ft_check_new_line(char *s)
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

static char	*ft_strdup(char *s)
{
	size_t	i;
	size_t	s_len;
	char	*dup;

	i = 0;
	s_len = ft_strlen(s);
	dup = malloc(s_len + 1);
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

char	*ft_strjoin(char *s1, char *s2)
{
	size_t	i;
	size_t	j;
	char	*join;

	if (!s1 && !s2)
		return (NULL);
	else if (!s1)
		return (ft_strdup(s2));
	else if (!s2)
		return (ft_strdup(s1));
	i = 0;
	j = 0;
	join = malloc(ft_strlen(s1) + ft_strlen(s2) + 1);
	if (!join)
		return (NULL);
	while (s1[i])
		join[j++] = s1[i++];
	i = 0;
	while (s2[i])
		join[j++] = s2[i++];
	join[j] = '\0';
	return (join);
}

void	*ft_lstfree(t_list **lst, int fd)
{
	t_list	*current;
	t_list	*prev;

	current = *lst;
	if (!current)
		return (NULL);
	if (current->fd == fd)
	{
		prev = current->next;
		free(current -> content);
		free(current);
		*lst = prev;
		return (NULL);
	}
	while (current->fd != fd && current->next != NULL)
	{
		prev = current;
		current = current -> next;
	}
	if (current->fd != fd && current->next == NULL)
		return (NULL);
	prev -> next = current -> next;
	free(current -> content);
	free(current);
	return (NULL);
}
