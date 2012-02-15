#include "symtab.h"
#include <vector>
#include <utility>
#include <string>

symbol symtab::find(const std::string& which) const {
	for (table::const_reverse_iterator i = tab.rbegin(); i != tab.rend(); ++i) {
		if (i->first == which) {
			return i->second;
		}
	}
	return symbol(symbol::undef);
}
bool symtab::exists_in_block(const std::string& which) const {
	for (table::const_reverse_iterator i = tab.rbegin(); i != tab.rend(); ++i) {
		if (*i == block) {
			return false;
		}
		if (i->first == which) {
			return true;
		}
	}
	return false;
}

bool symtab::push_item(const std::string& name, symbol sym) {
	if (exists_in_block(name)) {
		return false;
	}
	tab.push_back(std::make_pair(name, sym));
	return true;
}

const symtab::identifier symtab::block = std::make_pair("", symbol(symbol::undef));
