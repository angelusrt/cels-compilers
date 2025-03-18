#include "compilers.h"

esyntax_tree cels_compilers_parse_source(const string source) {
	syntax_tree tree = {0};
	mutrees_init(tree)
	
	size_t last_beggining = 0;
	size_t last_ending = 0;

	size_t token_beggining = 0;
	size_t token_ending = 0;

	size_t scope_beggining = 0;
	size_t scope_ending = 0;
	size_t scope_level = 0;

	bool is_within_quotes = false;

	size_t comment_beggining = 0;
	size_t comment_ending = 0;
	size_t comment_type = 0;
	bool is_within_comment = false;

	for (size_t i = 0; i < source.size - 1; i++) {

		/* alias */

		char current_letter = source.data[i];

		char last_letter = 
			i == 0 ? 
			source.data[i] : 
			source.data[i - 1];

		char next_letter = 
			i == source.size - 2 ? 
			source.data[source.size - 2] : 
			source.data[i + 1];

		/* comment */

		bool may_be_beggining_comment = 
			!is_within_quotes && 
			!is_within_comment && 
			current_letter == '/';

		if (may_be_beggining_comment) {
			if (next_letter == '/') {
				comment_type = 0;
				comment_beggining = i;
				is_within_comment = true;
				continue;
			} else if (next_letter == '*') {
				comment_type = 1;
				comment_beggining = i;
				is_within_comment = true;
				continue;
			}

			return (esyntax_tree){
				.error=syntax_invalid_comment_initiation_error
			};
		}

		if (is_within_comment) {
			bool is_line_comment_ending = 
				comment_type == 0 && 
				current_letter == '\n';

			bool is_multiline_comment_ending = 
				comment_type == 1 && 
				current_letter == '/' && 
				last_letter == '*';

			if (is_line_comment_ending) {
				comment_ending = i - 1;
				is_within_comment = false;
			} else if (is_multiline_comment_ending) {
				if (comment_beggining >= i - 2) {
					return (esyntax_tree){
						.error=syntax_invalid_comment_initiation_error
					};
				} 

				comment_ending = i;
				is_within_comment = false;
			}
			
			continue;
		}

		/* quotes */

		bool is_beggining_quotes = !is_within_quotes && current_letter == '"';
		if (is_beggining_quotes) {
			is_within_quotes = true;
		} 
		
		if (is_within_quotes) {
			bool is_ending_quotes = current_letter == '"' && last_letter != '\\';
			if (is_ending_quotes) {
				is_within_quotes = false;
			}

			continue;
		}

		/* jump whitespace */

		bool is_whitespace = chars_is_whitespace(current_letter);
		if (!is_within_quotes && is_whitespace) {
			bool was_alphanumeric = chars_is_alphanumeric(last_letter);
			if (i > token_beggining && was_alphanumeric) {
				token_ending = i;
				goto has_changed;
			}
		}

		/* scope */

		bool is_scope_beggining = 
			scope_ending <= scope_beggining && 
			scope_level == 0 && 
			current_letter == '{';

		if (is_scope_beggining) {
			scope_beggining = i;
			++scope_level;
			continue;
		}

		bool is_scope_ending = 
			scope_ending <= scope_beggining && 
			scope_level == 1 && 
			current_letter == '}';

		if (is_scope_ending) {
			scope_ending = i;
			--scope_level;
			goto has_changed;
		}

		bool is_scope_up = 
			scope_ending <= scope_beggining && 
			scope_level >= 1 && 
			current_letter == '{';

		if (is_scope_up) {
			++scope_level;
			continue;
		}
		
		bool is_scope_down = 
			scope_ending <= scope_beggining && 
			scope_level > 1 && 
			current_letter == '}';

		if (is_scope_down) {
			--scope_level;
			continue;
		}

		/* token recognition */

		bool was_not_alphanumeric = !chars_is_alphanumeric(last_letter);
		bool is_alphanumeric = chars_is_alphanumeric(current_letter);
		if (was_not_alphanumeric && is_alphanumeric) {
			token_beggining = i;
		}

		if (!was_not_alphanumeric && !is_alphanumeric) {
			token_ending = i;
			goto has_changed;
		}

		continue;

		/* debugging */

		has_changed:
		printf(
			"lb: %zu, le: %zu, "
			"tb: %zu, te: %zu, "
			"cb: %zu, ce: %zu, "
			"sb: %zu, se: %zu\n",
			last_beggining, 
			last_ending, 
			token_beggining, 
			token_ending,
			comment_beggining,
			comment_ending,
			scope_beggining,
			scope_ending);

		if (token_ending > token_beggining) {
			printf(
				"token: %.*s\n",
				(int)(token_ending - token_beggining),
				source.data + token_beggining);
		}
	}

	return (esyntax_tree){.error=fail};
}
