#include <string>
#include <algorithm>
#include "symtab.h"
using namespace std;

const symbol& symtab::find(const string& which) const {
	static symbol not_found = symbol(symbol::undef);
	for (table::const_reverse_iterator i = tab.rbegin(); i != tab.rend(); ++i) {
		block::const_iterator item = i->find(which);
		if (item != i->end()) return item->second;
	}
	return not_found;
}
bool symtab::exists_in_block(const string& which) const {
	return tab.back().count(which) > 0;
}

bool symtab::add_symbol(const string& name, const symbol& sym) {
	if (exists_in_block(name)) {
		return false;
	}
	tab.back().insert(make_pair(name, sym));
	return true;
}
