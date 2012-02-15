#include "lexer.h"
#include <cctype>
#include <iostream>
#include <cstdio>
using namespace std;

void lexer::extract_token() {
	discard_whitespace();

	int tok = cin.peek();
	switch (tok) {
		case '\n':
			++_line_no;
		case '@':
		case '%':
		case '&':
		case '(':
		case ')':
			_token = tok;
			cin.get();
			return;
		case EOF:
			_token = "";
			return;
		default:
			if (isdigit(tok)) {
				_token = "";
				do {
					_token += cin.get();
				} while (isdigit(cin.peek()));
				return;
			}
	}
	// We have found an unknown token! Oh dear me!
	cout << "line " << line_no() << " lexical error\n";
	//cout << "line " << line_no() << " lexical error: "
	//	"unknown char " << (char) tok << " (" << tok << ")\n";
	is_error = true;
}

string lexer::token() {
	if (is_error) {
		return "";
	}
	// We do this awkward flag stuff so that a lexical error as the first
	// token of the line doesn't print an error too soon.
	if (must_get_token) {
		extract_token();
		must_get_token = false;
	}
	return _token;
}
