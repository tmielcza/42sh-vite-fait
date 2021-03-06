/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_words.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djean <djean@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/01 20:19:58 by djean             #+#    #+#             */
/*   Updated: 2017/04/01 20:20:01 by djean            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "user_interface.h"
#include "ctype_42.h"

size_t		line_next_word_size(t_term_line *line)
{
	int		i;
	size_t	len;

	i = (int)line->string_index;
	len = 0;
	while (FT_ISBLANK(line->str->str[i]))
	{
		i += 1;
		len += 1;
	}
	while (line->str->str[i] && !FT_ISBLANK(line->str->str[i]))
	{
		i += 1;
		len += 1;
	}
	return (len);
}

size_t		line_prev_word_size(t_term_line *line)
{
	int		i;
	size_t	len;

	i = (int)line->string_index - 1;
	len = 0;
	while (i >= 0 && FT_ISBLANK(line->str->str[i]))
	{
		i -= 1;
		len += 1;
	}
	while (i >= 0 && !FT_ISBLANK(line->str->str[i]))
	{
		i -= 1;
		len += 1;
	}
	return (len);
}
