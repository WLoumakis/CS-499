/**
 * Tester module for testing the Lexeme,
 * Lexer, Types, and Parser modules.
 * 
 * Created by Walt Loumakis
 */

#include <assert.h>
#include "Parser.h"

int main(int argc, char *argv[]) {
	assert(argc == 2);
	initParser(argv[1]);
	Lexeme *tree = parse();
	closeParser();
	freeParseTree(tree);
	return 0;
}
