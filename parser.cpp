#include "parser.h"
#include <iostream>
#include <sstream>
using namespace std;

ptree::ptr parser::read_expression() {
	ptree::ptr ret = ampersand_op();
	if (!match("\n")) {
		_l.flush_line();
		return ptree::ptr();
	}
	return ret;
}

ptree::ptr parser::ampersand_op() {
	ptree::ptr ret = percent_op();

	while (ret && _l.token() == "&") {
		match("&");
		ptree::ptr tmp = percent_op();
		if (!tmp) return ptree::ptr();
		ret = ptree::ptr(new amp_operator(ret, tmp));
	}

	return ret;
}

ptree::ptr parser::percent_op() {
	if (_l.token() == "%") {
		match("%");
		ptree::ptr tmp = percent_op();
		if (!tmp) return ptree::ptr();
		return ptree::ptr(new pct_operator(tmp));
	} else {
		return at_op();
	}
}

ptree::ptr parser::at_op() {
	ptree::ptr ret = term();

	if (ret && _l.token() == "@") {
		match("@");
		ptree::ptr tmp = at_op();
		if (!tmp) return ptree::ptr();
		ret = ptree::ptr(new at_operator(ret, tmp));
	}
	return ret;
}

ptree::ptr parser::term() {
	if (_l.token() == "(") {
		match("(");
		ptree::ptr ret = ampersand_op();
		if (!match(")")) return ptree::ptr();
		return ret;
	} else {
		return number();
	}
}

ptree::ptr parser::number() {
	int ret;
	if (!(istringstream(_l.token()) >> ret)) {
		// the conversion failed; it's not actually a number.
		return ptree::ptr();
	}
	match(_l.token());
	return ptree::ptr(new int_literal(ret));
}

bool parser::match(const string tok) {
	// lexical error
	if (!_l.good()) return false;
	if (tok == _l.token()) {
		_l.getToken();
		return true;
	} else {
		cout << "line " << _l.line_no() << " syntax error\n";
		return false;
	}
}
