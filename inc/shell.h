#ifndef SHELL_H
# define SHELL_H

#include "string_42.h"
#include "array_42.h"
#include "parser.h"

# define BIN_NAME "42sh"
# define SHELL_PS1 "42sh> "
# define SHELL_PS2 "> "

int	shell_loop(void);
int	shell_input(t_string *input, const char *prompt);
int	shell_lexer(t_string *input, t_array *tokens);
int	shell_parser(const t_string *input, t_parser *parser, const t_array *tokens);
int	shell_exec(const t_ast ast, const t_string *inpu);

void	shell_history_init(void);
void	shell_history_shutdown(void);

#endif
