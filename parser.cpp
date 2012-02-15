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
		cout << "line " << line_no << " static semantic error - "
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
	if (_l.token() == lexer::var_tok) {
		variable_declaration();
	} else if (_l.token() == lexer::const_tok) {
		constant_declaration();
	} else {
		syntax_error(_l.line_no());
	}
}
void parser::variable_declaration() {
	match("var");
	if (_l.token() != lexer::id_tok) {
		syntax_error(_l.line_no());
	}
	if (_s.exists_in_block(_l.token_raw())) {
		identifier_redef_error(_l.line_no());
	} else {
		_s.decl_var(_l.token_raw());
	}
	match();
	while (_l.token() == lexer::comma_tok) {
		match(",");
		if (_l.token() != lexer::id_tok) {
			syntax_error(_l.line_no());
		}
		if (_s.exists_in_block(_l.token_raw())) {
			identifier_redef_error(_l.line_no());
		} else {
			_s.decl_var(_l.token_raw());
		}
		match();
	}
}
void parser::constant_declaration() {
	match("const");
	if (_l.token() != lexer::id_tok) {
		syntax_error(_l.line_no());
	}
	if (_s.exists_in_block(_l.token_raw())) {
		identifier_redef_error(_l.line_no());
	} else {
		_s.decl_const(_l.token_raw());
	}
	match();
	match("=");
	number();
}

void parser::statement() {
	if (_l.token() == lexer::id_tok) {
		assignment();
	} else if (_l.token() == lexer::print_tok) {
		print_stmt();
	} else if (_l.token() == lexer::if_tok) {
		if_stmt();
	} else if (_l.token() == lexer::do_tok) {
		do_stmt();
	} else {
		syntax_error(_l.line_no());
	}
}

void parser::assignment() {
	symbol::var_type type = _s.find(_l.token_raw()).type();
	if (type == symbol::undef) {
		identifier_undef_error(_l.line_no());
	} else if (type == symbol::constant) {
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
	while (_l.token() == lexer::do_tok) {
		do_block();
	}
	match("end");
}
void parser::do_stmt() {
	match("loop");
	while (_l.token() == lexer::do_tok) {
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
	while (_l.token() == lexer::amp_tok) {
		match();
		uni_term();
	}
}
void parser::uni_term() {
	if (_l.token() == lexer::per_tok) {
		match();
		uni_term();
	} else {
		term();
	}
}
void parser::term() {
	factor();
	if (_l.token() == lexer::at_tok) {
		match();
		term();
	}
}
void parser::factor() {
	if (_l.token() == lexer::lparen_tok) {
		match();
		expression();
		match(")");
	} else if (_l.token() == lexer::number_tok) {
		number();
	} else if (_l.token() == lexer::id_tok) {
		if (_s.find(_l.token_raw()).type() == symbol::undef) {
			identifier_undef_error(_l.line_no());
		} else {
			match();
		}
	} else {
		syntax_error(_l.line_no());
	}
}

void parser::number() {
	if (_l.token() != lexer::number_tok) {
		syntax_error(_l.line_no());
	} else {
		// sillily, we don't even do anything with it yet.
		match();
	}
}



inline bool parser::is_declaration(lexer::tok_type tok) {
	return (tok == lexer::var_tok || tok == lexer::const_tok);
}
inline bool parser::is_statement(lexer::tok_type tok) {
	return (tok == lexer::id_tok    ||
		tok == lexer::print_tok ||
		tok == lexer::if_tok    ||
		tok == lexer::do_tok);
}
inline bool parser::is_relop(lexer::tok_type tok) {
	return (tok == lexer::equal_tok ||
		tok == lexer::lt_tok    ||
		tok == lexer::gt_tok    ||
		tok == lexer::ne_tok    ||
		tok == lexer::le_tok    ||
		tok == lexer::ge_tok);
}

bool parser::match(const char *tok) {
	if (tok == _l.token_raw()) {
		_l.getToken();
		return true;
	} else {
		syntax_error(_l.line_no());
		return false;
	}
}
bool parser::match() {
	_l.getToken();
	return true;
}
