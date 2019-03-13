/**
 * Tester module for testing the Lexeme,
 * Lexer, and Types modules.
 * 
 * Created by Walt Loumakis
 */

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "Lexeme.h"
#include "Lexer.h"
#include "Types.h"

int main(int argc, char *argv[]) {
	assert(argc == 2);
	initLexer(argv[1]);
	Lexeme *l = lex();
	while (1) {
		if (displayLexeme(l, stdout)) {
			freeLexeme(l);
			break;
		}
		freeLexeme(l);
		l = lex();
		if (strcmp(getType(l), ENDofINPUT) == 0) {
			displayLexeme(l, stdout);
			freeLexeme(l);
			break;
		}
	}

	closeLexer();
	return 0;
}
