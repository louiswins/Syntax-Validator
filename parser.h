#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "symtab.h"
#include <string>

class parser {
public:
	parser() {}
	parser(const lexer& _l) : l(_l) {}

	void verify_program();

private:
	void block();
	void declaration();
	void variable_declaration();
	void constant_declaration();
	void statement();
	void assignment();
	void print_stmt();
	void if_stmt();
	void do_stmt();
	void do_block();
	void expression();
	void simple();
	void uni_term();
	void term();
	void factor();
	void number();

	bool is_declaration(lexer::tok_type tok);
	bool is_statement(lexer::tok_type tok);
	bool is_relop(lexer::tok_type tok);
	bool match(const char *tok);
	bool match();

	lexer l;
	symtab s;
};





inline bool parser::is_declaration(lexer::tok_type tok) {
	return (tok == lexer::var_tok || tok == lexer::const_tok);
}
inline bool parser::is_statement(lexer::tok_type tok) {
	return (tok == lexer::id_tok    ||
		tok == lexer::print_tok ||
		tok == lexer::if_tok    ||
		tok == lexer::loop_tok);
}
inline bool parser::is_relop(lexer::tok_type tok) {
	return (tok == lexer::equal_tok ||
		tok == lexer::lt_tok    ||
		tok == lexer::gt_tok    ||
		tok == lexer::ne_tok    ||
		tok == lexer::le_tok    ||
		tok == lexer::ge_tok);
}

#endif /* PARSER_H */
