#ifndef VAR_H
# define VAR_H

# include "array_42.h"
# include "errors.h"

# define VAR_ATTR_EXPORT 1U << 0
# define VAR_ATTR_RDONLY 1U << 1

typedef t_array			t_var_priv;
typedef struct s_var	t_var;
struct		s_var {
	char		*name;
	char		*value;
	unsigned	attrs;
};

enum	e_variable_errors
{
	VAR_NO_ERROR = NO_ERROR,
	ERR_VAR_NOT_FOUND,
	ERR_VAR_RDONLY,
	ERR_VAR_BAD_NAME,
};

/*
** extern char	**g_environ;
*/

void		var_init(void);
void		var_shutdown(void);
int			var_set(const char *name, const char *value, int attrs);
int			var_get(const char *name, const char **value);
int			var_unset(const char *name);

#endif
