#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include <string>

class parser {
public:
	parser() {}
	parser(lexer l) : _l(l) {}

	ptree::ptr read_expression();
	bool has_expression() const { return _l.hasToken(); }
private:
	ptree::ptr ampersand_op();
	ptree::ptr percent_op();
	ptree::ptr at_op();
	ptree::ptr term();
	ptree::ptr number();
	bool match(const std::string tok);

	lexer _l;
};

#endif /* PARSER_H */
