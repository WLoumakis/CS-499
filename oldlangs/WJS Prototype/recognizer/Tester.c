/**
 * Tester module for testing the Lexeme,
 * Lexer, Types, and Recognizer modules.
 * 
 * Created by Walt Loumakis
 */

#include <assert.h>
#include "Recognizer.h"

int main(int argc, char *argv[]) {
	assert(argc == 2);
	initRecognizer(argv[1]);
	recognize();
	closeRecognizer();
	return 0;
}
