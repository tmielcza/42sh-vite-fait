/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   predict_cmd_suffix.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djean <djean@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/01 20:36:32 by djean             #+#    #+#             */
/*   Updated: 2017/04/01 20:36:33 by djean            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

bool	check_requirements_cmd_suffix(const t_parser *parser)
{
	if (check_requirements_io_redirect(parser)
			|| parser_check_current_token_type(parser, E_TOKEN_WORD))
		return (true);
	return (false);
}

/*
** cmd_suffix :            io_redirect
**            | cmd_suffix io_redirect
**			  |            WORD
**			  | cmd_suffix WORD
*/

int		predict_cmd_suffix(t_parser *parser, t_array *words,
		t_array *redirections)
{
	struct s_redirection	*redir;
	const t_token			*token;
	int						ret;

	ret = OK_;
	while (ret == OK_)
	{
		if (check_requirements_io_redirect(parser))
		{
			redir = fatal_malloc(array_get_available(redirections));
			ret = predict_io_redirect(parser, redir);
		}
		else if (parser_check_current_token_type(parser, E_TOKEN_WORD))
		{
			token = parser_get_current_token(parser);
			fatal_malloc(array_push(words, &token));
			parser_consume_token(parser);
		}
		else
			break ;
	}
	return (ret);
}
