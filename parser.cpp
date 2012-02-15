#include "parser.h"
#include "symtab.h"
#include "lexer.h"
#include <iostream>
#include <sstream>
#include <cstdlib>
using namespace std;

namespace {
	void syntax_error(unsigned line_no) {
		cout << "line " << line_no << " syntax error\n";
		exit(EXIT_FAILURE);
	}
	void identifier_redef_error(unsigned line_no) {
		cout << "line " << line_no << " static semantic error - "
			"identifier redefined\n";
	}
	void identifier_undef_error(unsigned line_no) {
		cout << "line " << line_no << " static semantic error - "
			"identifier undefined\n";
		exit(EXIT_FAILURE);
	}
	void const_assign_error(unsigned line_no) {
		cout << "line " << line_no << "static semantic error - "
			"invalid assignment\n";
	}
}

void parser::verify_program() {
	block();
	if (_l.hasToken()) {
		syntax_error(_l.line_no());
	}
}

void parser::block() {
	_s.push_block();
	while (is_declaration(_l.token())) {
		declaration();
	}
	while (is_statement(_l.token())) {
		statement();
	}
	_s.pop_block();
}

void parser::declaration() {
	if (_l.token() == "var") {
		variable_declaration();
	} else if (_l.token() == "const") {
		constant_declaration();
	} else {
		syntax_error(_l.line_no());
	}
}
void parser::variable_declaration() {
	match("var");
	if (!id_definition(_l.token())) {
		identifier_redef_error(_l.line_no());
	} else {
		_s.decl_var(_l.token());
	}
	match();
	while (_l.token() == ",") {
		match(",");
		if (!id_definition(_l.token())) {
			identifier_redef_error(_l.line_no());
		} else {
			_s.decl_var(_l.token());
		}
		match();
	}
}
void parser::constant_declaration() {
	match("const");
	if (!id_definition(_l.token())) {
		identifier_redef_error(_l.line_no());
	} else {
		_s.decl_const(_l.token());
	}
	match();
	match("=");
	number();
}

void parser::statement() {
	if (_l.token() == "print") {
		print_stmt();
	} else if (_l.token() == "if") {
		if_stmt();
	} else if (_l.token() == "do") {
		do_stmt();
	} else {
		assignment();
	}
}

void parser::assignment() {
	symtab::var_type type = id_lookup(_l.token());
	if (type == symtab::undef) {
		identifier_undef_error(_l.line_no());
	} else if (type == symtab::constant) {
		const_assign_error(_l.line_no());
	}
	match();
	match(":=");
	expression();
}

void parser::print_stmt() {
	match("print");
	expression();
}
void parser::if_stmt() {
	match("if");
	while (_l.token() == "do") {
		do_block();
	}
	match("end");
}
void parser::do_stmt() {
	match("loop");
	while (_l.token() == "do") {
		do_block();
	}
	match("end");
}
void parser::do_block() {
	match("do");
	expression();
	match("->");
	block();
	match("end");
}

void parser::expression() {
	simple();
	if (is_relop(_l.token())) {
		match();
		simple();
	}
}
void parser::simple() {
	uni_term();
	while (_l.token() == "&") {
		match();
		uni_term();
	}
}
void parser::uni_term() {
	if (_l.token() == "%") {
		match();
		uni_term();
	} else {
		term();
	}
}
void parser::term() {
	factor();
	if (_l.token() == "@") {
		match();
		term();
	}
}
void parser::factor() {
	if (_l.token() == "(") {
		match();
		expression();
		match(")");
	} else if (is_number(_l.token())) {
		number();
	} else if (is_ident(_l.token())) {
		if (id_lookup(_l.token()) == symtab::undef) {
			identifier_undef_error(_l.line_no());
		} else {
			match();
		}
	}
}

void parser::number() {
	if (!is_number(_l.token())) {
		syntax_error(_l.line_no());
	} else {
		// sillily, we don't even do anything with it yet.
		match();
	}
}



bool parser::id_definition(const string& tok) {
	if (!is_ident(tok)) {
		syntax_error(_l.line_no());
	}
	return !_s.exists_in_block(tok);
}
symtab::var_type parser::id_lookup(const string& tok) {
	if (!is_ident(tok)) {
		syntax_error(_l.line_no());
	}
	return _s.find(tok);
}

inline bool parser::is_reserved_word(const string& tok) {
	return (tok == "const" ||
		tok == "do"    ||
		tok == "end"   ||
		tok == "loop"  ||
		tok == "var");
}
inline bool parser::is_declaration(const string& tok) {
	return (tok == "var" || tok == "const");
}
inline bool parser::is_statement(const string& tok) {
	return (tok == "print" ||
		tok == "if"    ||
		tok == "do"    ||
		is_ident(tok));
}
inline bool parser::is_relop(const string& tok) {
	return (tok == "="  ||
		tok == "<"  ||
		tok == ">"  ||
		tok == "/=" ||
		tok == "<=" ||
		tok == ">=");
}
inline bool parser::is_ident(const string& tok) {
	if (tok.length() == 0) return false;
	if (is_reserved_word(tok)) return false;
	for (string::const_iterator i = tok.begin(); i != tok.end(); ++i) {
		if (!isalpha(*i)) return false;
	}
	return true;
}

bool parser::is_number(const string& tok) {
	int ret;
	if (!(istringstream(_l.token()) >> ret)) {
		return false;
	}
	return true;
}

bool parser::match(const char *tok) {
	// lexical error
	if (!_l.good()) return false;
	if (tok == _l.token()) {
		_l.getToken();
		return true;
	} else {
		syntax_error(_l.line_no());
		return false;
	}
}
bool parser::match() {
	if (!_l.good()) return false;
	_l.getToken();
	return true;
}
