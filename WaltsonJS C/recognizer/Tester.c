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
#include "Types.h"
#include "Recognizer.h"

int main(int argc, char *argv[]) {
	assert(argc == 2);
	initRecognizer(argv[1]);
	recognize();
	closeRecognizer();
	return 0;
}
