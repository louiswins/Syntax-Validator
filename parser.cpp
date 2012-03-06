#include "parser.h"
#include "symtab.h"
#include "lexer.h"
#include <iostream>
#include <sstream>
#include <cstdlib>
using namespace std;

namespace {
	void syntax_error(unsigned line_no) {
		cout << "line " << line_no << ": syntax error\n";
		exit(EXIT_FAILURE);
	}
	void identifier_redef_error(unsigned line_no) {
		cout << "line " << line_no << ": static semantic error - "
			"identifier redefined\n";
	}
	void identifier_undef_error(unsigned line_no) {
		cout << "line " << line_no << ": static semantic error - "
			"identifier undefined\n";
		exit(EXIT_FAILURE);
	}
	void const_assign_error(unsigned line_no) {
		cout << "line " << line_no << ": static semantic error - "
			"invalid assignment\n";
	}
}

void parser::verify_program() {
	block();
	if (l.hasToken()) {
		syntax_error(l.line_no());
	}
}

void parser::block() {
	s.push_block();
	while (is_declaration(l.token())) {
		declaration();
	}
	while (is_statement(l.token())) {
		statement();
	}
	s.pop_block();
}

void parser::declaration() {
	if (l.token() == lexer::var_tok) {
		variable_declaration();
	} else if (l.token() == lexer::const_tok) {
		constant_declaration();
	} else {
		syntax_error(l.line_no());
	}
}
void parser::variable_declaration() {
	match("var");
	if (l.token() != lexer::id_tok) {
		syntax_error(l.line_no());
	}
	if (s.exists_in_block(l.token_raw())) {
		identifier_redef_error(l.line_no());
	} else {
		s.decl_var(l.token_raw());
	}
	match();
	while (l.token() == lexer::comma_tok) {
		match(",");
		if (l.token() != lexer::id_tok) {
			syntax_error(l.line_no());
		}
		if (s.exists_in_block(l.token_raw())) {
			identifier_redef_error(l.line_no());
		} else {
			s.decl_var(l.token_raw());
		}
		match();
	}
}
void parser::constant_declaration() {
	match("const");
	if (l.token() != lexer::id_tok) {
		syntax_error(l.line_no());
	}
	if (s.exists_in_block(l.token_raw())) {
		identifier_redef_error(l.line_no());
	} else {
		s.decl_const(l.token_raw());
	}
	match();
	match("=");
	number();
}

void parser::statement() {
	if (l.token() == lexer::id_tok) {
		assignment();
	} else if (l.token() == lexer::print_tok) {
		print_stmt();
	} else if (l.token() == lexer::if_tok) {
		if_stmt();
	} else if (l.token() == lexer::loop_tok) {
		do_stmt();
	} else {
		syntax_error(l.line_no());
	}
}

void parser::assignment() {
	symbol::var_type type = s.find(l.token_raw()).type();
	if (type == symbol::undef) {
		identifier_undef_error(l.line_no());
	} else if (type == symbol::constant) {
		const_assign_error(l.line_no());
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
	while (l.token() == lexer::do_tok) {
		do_block();
	}
	match("end");
}
void parser::do_stmt() {
	match("loop");
	while (l.token() == lexer::do_tok) {
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
	if (is_relop(l.token())) {
		match();
		simple();
	}
}
void parser::simple() {
	uni_term();
	while (l.token() == lexer::amp_tok) {
		match();
		uni_term();
	}
}
void parser::uni_term() {
	if (l.token() == lexer::per_tok) {
		match();
		uni_term();
	} else {
		term();
	}
}
void parser::term() {
	factor();
	if (l.token() == lexer::at_tok) {
		match();
		term();
	}
}
void parser::factor() {
	if (l.token() == lexer::lparen_tok) {
		match();
		expression();
		match(")");
	} else if (l.token() == lexer::number_tok) {
		number();
	} else if (l.token() == lexer::id_tok) {
		if (s.find(l.token_raw()).type() == symbol::undef) {
			identifier_undef_error(l.line_no());
		} else {
			match();
		}
	} else {
		syntax_error(l.line_no());
	}
}

void parser::number() {
	if (l.token() != lexer::number_tok) {
		syntax_error(l.line_no());
	} else {
		// sillily, we don't even do anything with it yet.
		match();
	}
}



bool parser::match(const char *tok) {
	if (tok == l.token_raw()) {
		l.getToken();
		return true;
	} else {
		syntax_error(l.line_no());
		return false;
	}
}
bool parser::match() {
	l.getToken();
	return true;
}
