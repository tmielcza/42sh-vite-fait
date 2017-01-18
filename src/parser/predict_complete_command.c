#include "parser.h"

bool	check_requirements_complete_command(const t_parser *parser)
{
	if (check_requirements_list(parser))
		return (true);
	return (false);
}

/*
** complete_command : list separator_op
**                  | list
*/

int		predict_complete_command(t_parser *parser, t_ast_node **from_parent)
{
	if (predict_list(parser, from_parent) != PARSER_NO_ERROR) // Evite la compression
		return (ERR_PARSING);
	if (check_requirements_separator_op(parser))
		return (predict_separator_op(parser));
	return (PARSER_NO_ERROR);
}