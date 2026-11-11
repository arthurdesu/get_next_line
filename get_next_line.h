/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamehri <mamehri@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/06 06:09:09 by mamehri           #+#    #+#             */
/*   Updated: 2026/11/11 09:56:39 by mamehri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif

# include <unistd.h>
# include <stdlib.h>

char	*get_next_line(int fd);

char	*ft_strjoin(char *stash, char *buf);

char	*ft_strdup(char *s);

size_t	ft_strlen(const char *s);

char	*ft_free(char **s);

int		ft_is_newline(char c);

#endif