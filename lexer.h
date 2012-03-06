#ifndef LEXER_H
#define LEXER_H

#include <iostream>
#include <cstdio>

class lexer {
public:
	enum tok_type {
		invalid_tok,

		amp_tok,
		arrow_tok,
		assign_tok,
		at_tok,
		comma_tok,
		const_tok,
		do_tok,
		end_tok,
		equal_tok,
		ge_tok,
		gt_tok,
		id_tok,
		if_tok,
		le_tok,
		loop_tok,
		lt_tok,
		lparen_tok,
		ne_tok,
		number_tok,
		per_tok,
		print_tok,
		rparen_tok,
		var_tok
	};

	lexer() : _line_no(1), must_get_token(true) {}

	void getToken() { must_get_token = true; }

	bool hasToken() const { return std::cin.peek() != EOF; }

	tok_type token();
	const std::string& token_raw();

	unsigned line_no() const { return _line_no; }

	void flush_line();

private:
	unsigned discard_whitespace();

	void extract_token();

	unsigned _line_no;
	std::string _token_raw;
	tok_type _token;
	bool must_get_token;
};



inline lexer::tok_type lexer::token() {
	if (must_get_token) {
		extract_token();
	}
	return _token;
}
inline const std::string& lexer::token_raw() {
	if (must_get_token) {
		extract_token();
	}
	return _token_raw;
}
inline void lexer::flush_line() {
	while (!std::cin.eof() && std::cin.get() != '\n');
	++_line_no;
	must_get_token = true;
}
inline unsigned lexer::discard_whitespace() {
	int c;
	unsigned ret = 0;
	while ((c = std::cin.peek()) == ' ' || c == '\t' || c == '\n') {
		if (c == '\n') { ++ret; }
		std::cin.get();
	}
	_line_no += ret;
	return ret;
}

#endif /* LEXER_H */
