#include "lexers.h"

estring_vec cels_lexers_scan(string source, const allocator *mem) {
	string_vec tokens = {0};

	error init_error = vectors_init(&tokens, sizeof(string), 16, mem);
	if (init_error) { goto cleanup0; }

	size_t token_beggining = 0;
	size_t token_ending = 0;

	size_t comment_type = 0;
	bool is_within_comment = false;
	bool is_within_quotes = false;
	bool has_ended_with_special_character = false;

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
				token_beggining = i;
				is_within_comment = true;

				continue;
			} else if (next_letter == '*') {
				comment_type = 1;
				token_beggining = i;
				is_within_comment = true;

				continue;
			}

			return (estring_vec){.error=fail};
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
				token_ending = i - 1;
				is_within_comment = false;

				goto push_token;
			} else if (is_multiline_comment_ending) {
				if (token_beggining >= i - 2) {
					return (estring_vec){.error=fail};
				} 

				token_ending = i;
				is_within_comment = false;

				goto push_token;
			}
			
			continue;
		}

		/* quotes */

		bool is_beggining_quotes = 
			!is_within_quotes && 
			current_letter == '"';

		if (is_beggining_quotes) {
			token_beggining = i;
			is_within_quotes = true;

			continue;
		} 
		
		if (is_within_quotes) {
			bool is_ending_quotes = 
				current_letter == '"' && 
				last_letter != '\\';

			if (is_ending_quotes) {
				token_ending = i;
				is_within_quotes = false;

				goto push_token;
			}

			continue;
		}

		/* jump whitespace */

		bool is_whitespace = chars_is_whitespace(current_letter);
		if (!is_within_quotes && is_whitespace) {
			bool was_alphanumeric = chars_is_alphanumeric(last_letter);
			if (i > token_beggining && was_alphanumeric) {
				token_ending = i - 1;
				goto push_token;
			}

			continue;
		}

		/* token recognition */

		bool was_not_alphanumeric = !chars_is_alphanumeric(last_letter);
		bool is_alphanumeric = chars_is_alphanumeric(current_letter);
		if (was_not_alphanumeric && is_alphanumeric) {
			token_beggining = i;
			continue;
		} else if (!was_not_alphanumeric && !is_alphanumeric) {
			token_ending = i - 1;
			has_ended_with_special_character = true;
			goto push_token;
		} else if (!was_not_alphanumeric && is_alphanumeric) {
			continue;
		}

		/* special characters */

		if (token_ending >= token_beggining) {
			token_beggining = i;
			token_ending = i;
			goto push_token;
		}

		continue;

		/* pushes token */

		push_token:;
		string token = strings_copy(
			&source, token_beggining, token_ending + 1, mem);

		error push_error = vectors_push(&tokens, &token, mem);
		if (push_error) {
			strings_free(&token, mem);
			goto cleanup1;
		}

		if (has_ended_with_special_character) {
			has_ended_with_special_character = false;

			token_beggining = i;
			token_ending = i;
			goto push_token;
		}
	}

	return (estring_vec){.value=tokens};

	cleanup1:
	vectors_free(&tokens, (freefunc)strings_free, mem);

	cleanup0:
	return (estring_vec){.error=fail};
}
