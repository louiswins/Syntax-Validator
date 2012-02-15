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
	must_get_token = false;
	do {
		discard_whitespace();
		int tok = cin.peek();
		switch (tok) {
			// One-character tokens
			case '@':
				_token = at_tok;
				_token_raw = "@";
				cin.get();
				return;
			case '%':
				_token = per_tok;
				_token_raw = "%";
				cin.get();
				return;
			case '&':
				_token = amp_tok;
				_token_raw = "&";
				cin.get();
				return;
			case '(':
				_token = lparen_tok;
				_token_raw = "(";
				cin.get();
				return;
			case ')':
				_token = rparen_tok;
				_token_raw = ")";
				cin.get();
				return;
			case ',':
				_token = comma_tok;
				_token_raw = ",";
				cin.get();
				return;
			case '=':
				_token = equal_tok;
				_token_raw = "=";
				cin.get();
				return;
			case EOF:
				_token = invalid_tok;
				_token_raw = "";
				return;
			// One- or two-character tokens
			case '<':
				cin.get();
				if (cin.peek() == '=') {
					_token = le_tok;
					_token_raw = "<=";
					cin.get();
					return;
				} else {
					_token = lt_tok;
					_token_raw = "<";
					return;
				}
			case '>':
				cin.get();
				if (cin.peek() == '=') {
					_token = ge_tok;
					_token_raw = ">=";
					cin.get();
					return;
				} else {
					_token = gt_tok;
					_token_raw = ">";
					return;
				}
			// Two-character tokens
			case ':':
				cin.get();
				if (cin.peek() == '=') {
					_token = assign_tok;
					_token_raw = ":=";
					cin.get();
					return;
				} else {
					illegal_tok_error(line_no());
					break;
				}
			case '-':
				cin.get();
				if (cin.peek() == '>') {
					_token = arrow_tok;
					_token_raw = "->";
					cin.get();
					return;
				} else {
					illegal_tok_error(line_no());
					break;
				}	
			case '/':
				cin.get();
				if (cin.peek() == '=') {
					_token = ne_tok;
					_token_raw = "/=";
					cin.get();
					return;
				} else {
					illegal_tok_error(line_no());
					break;
				}
			// General cases
			default:
				if (isdigit(tok)) {
					_token_raw = "";
					do {
						_token_raw += cin.get();
					} while (isdigit(cin.peek()) && _token_raw.length() < 20);
					if (_token_raw.length() == 20 && isdigit(cin.peek())) {
						too_long_error(line_no());
						while (isdigit(cin.peek())) { cin.get(); }
					}
					_token = number_tok;
					return;
				}
				if (isalpha(tok)) {
					_token_raw = "";
					do {
						_token_raw += cin.get();
					} while (isalpha(cin.peek()) && _token_raw.length() < 20);
					if (_token_raw.length() == 20 && isalpha(cin.peek())) {
						too_long_error(line_no());
						while (isalpha(cin.peek())) { cin.get(); }
					}
					if (_token_raw == "const") {
						_token = const_tok;
					} else if (_token_raw == "do") {
						_token = do_tok;
					} else if (_token_raw == "end") {
						_token = end_tok;
					} else if (_token_raw == "if") {
						_token = if_tok;
					} else if (_token_raw == "loop") {
						_token = loop_tok;
					} else if (_token_raw == "print") {
						_token = print_tok;
					} else if (_token_raw == "var") {
						_token = var_tok;
					} else {
						_token = id_tok;
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
