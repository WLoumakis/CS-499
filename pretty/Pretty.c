/**
 * Pretty Printer module created for printing
 * a parse tree into its respective code.
 * 
 * Created by Walt Loumakis
 */

#include <stdio.h>
#include "Pretty.h"
#include "Parser.h"
#include "Lexeme.h"

#define null NULL

void pretty(Lexeme *tree) {
	while (tree != null) {
		pp(car(tree));
		tree = cdr(tree);
	}
}
