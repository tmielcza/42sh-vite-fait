/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strisnum.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djean <djean@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/30 16:51:26 by djean             #+#    #+#             */
/*   Updated: 2017/03/30 16:51:46 by djean            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ctype_42.h"
#include "str_42.h"

int		ft_strisnum(const char *s)
{
	if (s[0] == '+' || s[0] == '-')
		s += 1;
	if (!FT_ISDIGIT(s[0]))
		return (0);
	while (FT_ISDIGIT(s[0]))
		s += 1;
	return (s[0] == '\0');
}
