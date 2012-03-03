#ifndef SYMTAB_H
#define SYMTAB_H

#include <vector>
#include <map>
#include <string>

class symbol {
public:
	enum var_type {
		undef,

		variable,
		constant
	};

	symbol(var_type type, unsigned value=4837) :
		_type(type), _val(value) {}
	bool set(unsigned val) {
		if (_type != variable) return false;
		_val = val;
		return true;
	}
	unsigned val() const { return _val; }
	var_type type() const { return _type; }

	bool operator==(const symbol& rhs) const {
		return _type == rhs._type && _val == rhs._val;
	}
private:

	var_type _type;
	unsigned _val;
};


class symtab {
private:
	typedef std::map<std::string, symbol> block;
	typedef std::vector<block> table;

	table tab;


public:

	symtab() {}

	const symbol& find(const std::string& which) const;
	bool exists_in_block(const std::string& which) const;

	bool decl_var(const std::string& name) {
		return add_symbol(name, symbol(symbol::variable));
	}
	bool decl_const(const std::string& name) {
		return add_symbol(name, symbol(symbol::constant));
	}

	void push_block() {
		tab.push_back(block());
	}
	void pop_block() {
		if (!tab.empty()) {
			tab.pop_back();
		}
	}

private:
	bool add_symbol(const std::string& name, const symbol& sym);
};

#endif
