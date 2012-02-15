#ifndef LEXER_H
#define LEXER_H

#include <iostream>
#include <cstdio>

class lexer {
public:
	lexer() : _line_no(1), is_error(false), must_get_token(true) {}

	void getToken() { must_get_token = true; }

	// cin.eof() doesn't return true until you've tried to read a
	// character and failed.
	bool hasToken() const { return std::cin.peek() != EOF; }

	bool good() const { return !is_error && std::cin.good(); }

	const std::string& token();

	unsigned line_no() const { return _line_no; }

	void flush_line() {
		while (!std::cin.eof() && std::cin.get() != '\n');
		++_line_no;
		is_error = false;
		must_get_token = true;
	}

private:
	void discard_whitespace() {
		int c;
		while ((c = std::cin.peek()) == ' ' || c == '\t' || c == '\n') {
			if (c == '\n') { ++_line_no; }
			std::cin.get();
		}
	}

	void extract_token();

	unsigned _line_no;
	bool is_error;
	std::string _token;
	bool must_get_token;
};

#endif /* LEXER_H */
