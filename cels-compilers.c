#include "packages/cels/strings.c"
#include "packages/cels/vectors.c"
#include "packages/cels/errors.c"
#include "packages/cels/maths.c"
#include "packages/cels/nodes.c"
#include "packages/cels/mems.c"

#include "modules/compilers.h"
#include "modules/compilers.c"


const string source = strings_premake(
	"/**thats a function documentation*/\n"
	"int main(void) {\n"
	"\tconst allocator mem = arenas_init(1024);\n"
	"\tstring_vec words = {0};\n"
	"\tvectors_init(&words, sizeof(string), vector_min, &mem);\n\n"
	"\tstring hello = strings_make(\"hello\", &mem);\n"
	"\tvectors_push(&words, &hello, &mem);\n"
	"\tstring world = strings_make(\"world\", &mem);\n"
	"\tvectors_push(&words, &world, &mem);\n\n"
	"\tconst string sep = strings_premake(\", \");\n"
	"\tstring hello_world = string_vecs_join(&words, sep, &mem);\n\n"
	"\tstrings_println(&hello_world); // hello, world\n\n"
	"\tmems_free(&mem, null);\n"
	"\treturn 0;\n"
	"}"
);


int main(void) {
	esyntax_tree tree = cels_compilers_parse_source(source);
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
