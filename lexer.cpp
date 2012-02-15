#include "lexer.h"
#include <cctype>
#include <iostream>
#include <cstdio>
using namespace std;

namespace {
	void illegal_tok_error(unsigned line_no) {
		cout << "line " << line_no << " lexical error - illegal tokens\n";
	}
	void too_long_error(unsigned line_no) {
		cout << "line " << line_no << " lexical error - exceeding maximum token length\n";
	}
}

void lexer::extract_token() {
	do {
		discard_whitespace();
		int tok = cin.peek();
		switch (tok) {
			// One-character tokens
			case '@':
			case '%':
			case '&':
			case '(':
			case ')':
			case ',':
			case '=':
				_token = tok;
				cin.get();
				return;
			case EOF:
				_token = "";
				return;
			// One- or two-character tokens
			case '<':
				cin.get();
				if (cin.peek() == '=') {
					_token = "<=";
					cin.get();
					return;
				} else {
					_token = "<";
					return;
				}
			case '>':
				cin.get();
				if (cin.peek() == '=') {
					_token = ">=";
					cin.get();
					return;
				} else {
					_token = ">";
					return;
				}
			// Two-character tokens
			case ':':
				cin.get();
				if (cin.peek() == '=') {
					_token = ":=";
					cin.get();
					return;
				} else {
					illegal_tok_error(line_no());
					break;
				}
			case '-':
				cin.get();
				if (cin.peek() == '>') {
					_token = "->";
					cin.get();
					return;
				} else {
					illegal_tok_error(line_no());
					break;
				}	
			case '/':
				cin.get();
				if (cin.peek() == '=') {
					_token = "/=";
					cin.get();
					return;
				} else {
					illegal_tok_error(line_no());
					break;
				}
			// General cases
			default:
				if (isdigit(tok)) {
					_token = "";
					do {
						_token += cin.get();
					} while (isdigit(cin.peek()) && _token.length() < 20);
					if (_token.length() == 20 && isdigit(cin.peek())) {
						too_long_error(line_no());
						while (isdigit(cin.peek())) { cin.get(); }
					}
					return;
				}
				if (isalpha(tok)) {
					_token = "";
					do {
						_token += cin.get();
					} while (isalpha(cin.peek()) && _token.length() < 20);
					if (_token.length() == 20 && isalpha(cin.peek())) {
						too_long_error(line_no());
						while (isalpha(cin.peek())) { cin.get(); }
					}
					return;
				}
				// We have found an unknown token! Oh dear me!
				cin.get();
				illegal_tok_error(line_no());
				break;
		}
	} while (1);
}

const string& lexer::token() {
	if (is_error) {
		_token = "";
	} else if (must_get_token) {
		// We do this awkward flag stuff so that a lexical error as the
		// first token of the line doesn't print an error too soon.
		extract_token();
		must_get_token = false;
	}
	return _token;
}
