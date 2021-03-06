/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_node_list.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djean <djean@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/01 20:01:48 by djean             #+#    #+#             */
/*   Updated: 2017/04/01 20:01:52 by djean            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <assert.h>
#include "exec.h"
#include "ast.h"

extern t_tree_walker	*const g_walkers[];

int		exec_node_list(const t_ast_node *node, const t_string *input,
		bool set_context)
{
	t_array	list_nodes_stack;
	int		ret;

	assert(node != NULL);
	list_nodes_stack = gather_childrens_nodes(node, E_AST_LIST);
	array_pop(&list_nodes_stack, &node);
	ret = g_walkers[node->type](node, input, set_context);
	while (list_nodes_stack.len > 0)
	{
		array_pop(&list_nodes_stack, &node);
		ret = g_walkers[node->type](node, input, set_context);
	}
	array_shutdown(&list_nodes_stack);
	return (ret);
}
