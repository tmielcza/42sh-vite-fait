/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   predict_separator.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djean <djean@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/01 20:36:32 by djean             #+#    #+#             */
/*   Updated: 2017/04/01 20:36:33 by djean            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

bool	check_requirements_separator_op(const t_parser *parser)
{
	if (parser_check_current_token_type(parser, E_TOKEN_AND)
			|| parser_check_current_token_type(parser, E_TOKEN_SEMI))
		return (true);
	return (false);
}

/*
** separator_op : '&'
**              | ';'
*/

int		predict_separator_op(t_parser *parser)
{
	if (check_requirements_separator_op(parser))
	{
		parser_consume_token(parser);
		return (OK_);
	}
	return (ERROR_);
}

bool	check_requirements_separator(const t_parser *parser)
{
	if (check_requirements_separator_op(parser)
			|| check_requirements_newline_list(parser))
		return (true);
	return (false);
}

/*
** separator : separator_op linebreak
**           | newline_list
*/

int		predict_separator(t_parser *parser)
{
	if (check_requirements_newline_list(parser))
		predict_newline_list(parser);
	else
	{
		if (predict_separator_op(parser) != OK_)
			return (ERROR_);
		predict_linebreak(parser);
	}
	return (OK_);
}
