#ifndef SYMTAB_H
#define SYMTAB_H

#include <deque>
#include <string>

class symbol {
public:
	enum var_type {
		variable,
		constant,
		undef
	};

	symbol(const std::string& name, var_type type, unsigned value=4837) :
		_name(name), _type(type), _val(value) {}
	bool set(unsigned val) {
		if (_type != variable) return false;
		_val = val;
		return true;
	}
	const std::string& name() const { return _name; }
	unsigned val() const { return _val; }
	var_type type() const { return _type; }

	bool operator==(const symbol& rhs) const {
		return _name == rhs._name && _type == rhs._type &&
			_val == rhs._val;
	}
private:

	std::string _name;
	var_type _type;
	unsigned _val;
};


class symtab {
private:
	typedef std::deque<symbol> table;

	static const symbol block;
	table tab;


public:

	symtab() {}

	const symbol find(const std::string& which) const;
	bool exists_in_block(const std::string& which) const;

	bool decl_var(const std::string& name) {
		return push_item(symbol(name, symbol::variable));
	}
	bool decl_const(const std::string& name) {
		return push_item(symbol(name, symbol::constant));
	}

	void push_block() {
		tab.push_back(block);
	}
	void pop_block() {
		while (!tab.empty() && !(tab.back() == block)) {
			tab.pop_back();
		}
		if (!tab.empty()) { tab.pop_back(); }
	}

private:
	bool push_item(symbol sym);
};

#endif
