/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djean <djean@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/30 17:00:15 by djean             #+#    #+#             */
/*   Updated: 2017/03/30 17:22:47 by djean            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include <stdbool.h>
# include "array_42.h"
# include "errors.h"
# include "lexer.h"
# include "ast.h"

# define SUBSHELL_MAX_DEPTH (512)

typedef struct s_parser		t_parser;
struct			s_parser
{
	t_ast			ast;
	const t_string	*input;
	const t_array	*tokens;
	t_array			heredocs;
	const t_token	*current_token;
	size_t			index;
	size_t			subshell_depth;
};

/*
** Vérifie la correspondance entre le token courant et les paramètres attendus.
** Si ça match, on consomme le token courant, et on retourne OK_.
** Sinon on retourne l'erreur ERROR_.
*/
int				parser_consume_if_match(t_parser *parser, int type);
void			parser_consume_token(t_parser *parser);

/*
** Retourne `true` si le type du token courant correspond au type demandé
*/
bool			parser_check_current_token_type(const t_parser *parser,
		int type);

/*
** Retourne le token courant ou le token `n` suivant celui courant
*/
const t_token	*parser_get_current_token(const t_parser *parser);
const t_token	*parser_get_lookahead_token(const t_parser *parser, size_t n);

/*
** Reçoit une liste de tokens et retourne `OK_` si la phrase est correcte.
** `ERROR_` sinon.
*/
int				parser_parse(t_parser *parser);
t_parser		*parser_init(t_parser *parser);
void			parser_init_with_tokens(const t_string *input, t_parser *parser,
		const t_array *tokens);
void			parser_clear(t_parser *parser);
void			parser_shutdown(t_parser *parser);

/*
** Separator/Linebreak
*/
bool			check_requirements_separator_op(const t_parser *parser);
bool			check_requirements_separator(const t_parser *parser);
bool			check_requirements_newline_list(const t_parser *parser);
bool			check_requirements_linebreak(const t_parser *parser);
int				predict_separator_op(t_parser *parser);
int				predict_separator(t_parser *parser);
int				predict_linebreak(t_parser *parser);
int				predict_newline_list(t_parser *parser);

/*
** Redirection
*/
bool			check_requirements_io_here(const t_parser *parser);
bool			check_requirements_io_file(const t_parser *parser);
bool			check_requirements_io_redirect(const t_parser *parser);
bool			check_requirements_redirect_list(const t_parser *parser);
int				predict_io_here(t_parser *parser, struct s_redirection *redir);
int				predict_io_file(t_parser *parser);
int				predict_io_redirect(t_parser *parser,
		struct s_redirection *redir);
int				predict_redirect_list(t_parser *parser, t_array *redirections);
void			parser_heredoc_push(t_parser *parser,
		const struct s_redirection *redir);
int				parser_heredoc_execute(const t_string *input,
		t_array *heredocs);
void			parser_heredoc_shutdown(t_array *heredocs);

/*
** Simple Command
*/
bool			check_requirements_cmd_prefix(const t_parser *parser);
bool			check_requirements_cmd_suffix(const t_parser *parser);
bool			check_requirements_simple_command(const t_parser *parser);
int				predict_cmd_prefix(t_parser *parser, t_array *redirections);
int				predict_cmd_suffix(t_parser *parser, t_array *words,
		t_array *redirections);
int				predict_simple_command(t_parser *parser, t_ast_node *self);

/*
** Command
*/
bool			check_requirements_term(const t_parser *parser);
bool			check_requirements_compound_list(const t_parser *parser);
bool			check_requirements_subshell(const t_parser *parser);
bool			check_requirements_command(const t_parser *parser);
int				predict_term(t_parser *parser, t_ast_node **from_parent);
int				predict_compound_list(t_parser *parser,
		t_ast_node **from_parent);
int				predict_subshell(t_parser *parser, t_ast_node *self);
int				predict_command(t_parser *parser, t_ast_node **from_parent);

/*
** Complete commands
*/
bool			check_requirements_pipe_sequence(const t_parser *parser);
bool			check_requirements_and_or(const t_parser *parser);
bool			check_requirements_list(const t_parser *parser);
bool			check_requirements_complete_command(const t_parser *parser);
bool			check_requirements_complete_commands(const t_parser *parser);
int				predict_pipe_sequence(t_parser *parser,
		t_ast_node **from_parent);
int				predict_and_or(t_parser *parser, t_ast_node **from_parent);
int				predict_list(t_parser *parser, t_ast_node **from_parent);
int				predict_complete_command(t_parser *parser,
		t_ast_node **from_parent);
int				predict_complete_commands(t_parser *parser,
		t_ast_node **from_parent);
int				predict_program(t_parser *parser, t_ast_node **root);

#endif
