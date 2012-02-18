#include <string>
#include <algorithm>
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>
#include "symtab.h"
using namespace std;
using boost::lambda::bind;
using boost::lambda::_1;

// Easily located using std::find_if and boost::lambda
const symbol& symtab::find(const string& which) const {
	static symbol bad_return = symbol("", symbol::undef);
	table::const_reverse_iterator result = find_if(tab.rbegin(), tab.rend(),
			bind(&symbol::name, _1) == which);
	return (result == tab.rend()) ? bad_return : *result;
}
bool symtab::exists_in_block(const string& which) const {
	table::const_reverse_iterator result = find_if(tab.rbegin(), tab.rend(),
			_1 == block || bind(&symbol::name, _1) == which);
	return !(result == tab.rend() || *result == block);
}

bool symtab::push_item(symbol sym) {
	if (exists_in_block(sym.name())) {
		return false;
	}
	tab.push_back(sym);
	return true;
}

const symbol symtab::block = symbol("", symbol::undef);
