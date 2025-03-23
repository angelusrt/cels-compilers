#include "compilers.h"

esyntax_tree cels_compilers_parse_source(
	const string source, const allocator *mem) {

	syntax_tree tree = {0};
	mutrees_init(tree)

	estring_vec tokens = cels_lexers_scan(source, mem);
	if (tokens.error != ok) {
		printf("there was an error during lexer phase.\n");
		exit(1);
	}

	string_vec *ts = &tokens.value;
	for (size_t i = 0; i < ts->size; i++) {
		strings_println(&ts->data[i]);
	}

	//TODO: must implement stack

	printf("\n");

	for (size_t i = 0; i < ts->size; i++) {

		/* alias */

		string *current_token = &ts->data[i];

		string *next_token = 
			i == ts->size - 2 ? 
			&ts->data[ts->size - 2] : 
			&ts->data[i + 1];
	}
	
	return (esyntax_tree){.error=fail};
}
