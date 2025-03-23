#include "packages/cels/strings.c"
#include "packages/cels/vectors.c"
#include "packages/cels/errors.c"
#include "packages/cels/maths.c"
#include "packages/cels/nodes.c"
#include "packages/cels/mems.c"

#include "modules/compilers.h"
#include "modules/compilers.c"

#include "modules/lexers.h"
#include "modules/lexers.c"

const string source0 = strings_premake(
	"/**thats a function documentation*/\n"
	"int main(void) { \n"
	"\tint a = 1024;\n"
	"\treturn 0;\n"
	"}"
);


int main(void) {
	const allocator mem = arenas_init(1024);

	esyntax_tree tree = cels_compilers_parse_source(source0, &mem);
	if (tree.error != syntax_successfull) {
		printf("parsing gave error '%d'\n", tree.error);
		exit(1);
	}

	syntax_tree *t = &tree.value;
	syntax_tree_iterator it = {0};
	while (mutrees_next(t, &it)) {
		strings_println(&it.data->data.name);
		printf("type: %d\n", it.data->data.type);
	}
	
	return 0;
}
