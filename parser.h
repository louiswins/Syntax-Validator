#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "symtab.h"
#include <string>

class parser {
public:
	parser() {}
	parser(const lexer& l) : _l(l) {}

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

	bool id_definition(const std::string& tok);
	symtab::var_type id_lookup(const std::string& tok);

	bool is_reserved_word(const std::string& tok);
	bool is_declaration(const std::string& tok);
	bool is_statement(const std::string& tok);
	bool is_relop(const std::string& tok);
	bool is_ident(const std::string& tok);
	bool is_number(const std::string& tok);
	bool match(const char *tok);
	bool match();

	lexer _l;
	symtab _s;
};

#endif /* PARSER_H */
