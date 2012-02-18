#include <string>
#include <algorithm>
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>
#include "symtab.h"
using boost::lambda::bind;
using boost::lambda::_1;

// Here we search through all possibilities, so we can use the stl find_if.
symbol symtab::find(const std::string& which) const {
	table::const_reverse_iterator result = std::find_if(tab.rbegin(), tab.rend(),
			bind(&symbol::name, _1) == which);
	return (result == tab.rend()) ?
		symbol("", symbol::undef) :
		*result;
}
// Here we bail out partway through, so it is unreasonable to implement in
// terms of stl (at least with the std::deque representation we have)
bool symtab::exists_in_block(const std::string& which) const {
	table::const_reverse_iterator i = tab.rbegin(),
		end = tab.rend();
	for (; i != end; ++i) {
		if (*i == block) {
			return false;
		}
		if (i->name() == which) {
			return true;
		}
	}
	return false;
}

bool symtab::push_item(symbol sym) {
	if (exists_in_block(sym.name())) {
		return false;
	}
	tab.push_back(sym);
	return true;
}

const symbol symtab::block = symbol("", symbol::undef);
