#ifndef SYMTAB_H
#define SYMTAB_H

#include <vector>
#include <utility>
#include <string>

class symtab {
public:
	enum var_type {
		variable,
		constant,
		undef
	};
private:
	typedef std::pair<std::string, var_type> identifier;
	typedef std::vector<identifier> table;

	static const identifier block;
	table tab;



public:

	symtab() {}

	symtab::var_type find(const std::string& which) const;
	bool exists_in_block(const std::string& which) const;

	bool decl_var(const std::string& name) {
		return push_item(name, variable);
	}
	bool decl_const(const std::string& name) {
		return push_item(name, constant);
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
	bool push_item(const std::string& name, var_type type);
};

#endif
