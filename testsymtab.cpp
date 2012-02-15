#include "symtab.h"
#include <iostream>
using namespace std;

int main() {
	symtab s;

	s.decl_var("louis");
	s.decl_var("wins");
	s.decl_const("hey");

	cout << "OUTER BLOCK:\n";
	cout << "louis: " << s.find("louis") << endl;
	cout << "wins: " << s.find("wins") << endl;
	cout << "hey: " << s.find("hey") << endl;
	cout << "what: " << s.find("what") << endl << endl;

	s.push_block();

	s.decl_const("wins");
	s.decl_var("what");

	cout << "INNER BLOCK:\n";
	cout << "louis: " << s.find("louis") << endl;
	cout << "wins: " << s.find("wins") << endl;
	cout << "hey: " << s.find("hey") << endl;
	cout << "what: " << s.find("what") << endl << endl;

	s.pop_block();

	cout << "BACK TO OUTER BLOCK:\n";
	cout << "louis: " << s.find("louis") << endl;
	cout << "wins: " << s.find("wins") << endl;
	cout << "hey: " << s.find("hey") << endl;
	cout << "what: " << s.find("what") << endl;

	s.pop_block();

	return 0;
}
