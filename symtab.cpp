#include "symtab.h"
#include <string>

const symbol symtab::find(const std::string& which) const {
	for (table::const_reverse_iterator i = tab.rbegin(); i != tab.rend(); ++i) {
		if (i->name() == which) {
			return *i;
		}
	}
	return symbol("", symbol::undef);
}
bool symtab::exists_in_block(const std::string& which) const {
	for (table::const_reverse_iterator i = tab.rbegin(); i != tab.rend(); ++i) {
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
