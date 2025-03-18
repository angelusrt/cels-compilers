#ifndef cels_compilers_h
#define cels_compilers_h

#include "../packages/cels/nodes.h"
#include "../packages/cels/strings.h"
#include "../packages/cels/types.h"

typedef struct interval {
	ulong x, y;
} interval;

typedef enum syntax_error {
	syntax_successfull,
	syntax_generic_error,
	syntax_invalid_comment_initiation_error,
} syntax_error;

typedef enum syntax_type {
	syntax_function_type
} syntax_type;

typedef struct syntax_entity {
	syntax_type type;
	interval interval;
	string name;
	string statement;
} syntax_entity;

typedef struct syntax_node syntax_node;
typedef munodes(syntax_node, syntax_entity) syntax_node;
typedef mutrees(syntax_node) syntax_tree;
typedef mutree_iterators(syntax_node) syntax_tree_iterator;
typedef errors(syntax_tree) esyntax_tree;

cels_warn_unused
esyntax_tree cels_compilers_parse_source(const string source);

#endif
