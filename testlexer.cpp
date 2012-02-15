#include "lexer.h"
#include <iostream>
using namespace std;

int main() {
	lexer l;

	while (l.hasToken()) {
		cout << "got token '" << l.token() << "'\n";
		l.getToken();
	}

	return 0;
}
