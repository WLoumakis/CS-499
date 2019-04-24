/**
 * Tester module for testing the Environments modules.
 * 
 * Created by Walt Loumakis
 */

#include <stdio.h>
#include <stdlib.h>
#include "Types.h"
#include "Lexeme.h"
#include "Environments.h"

#define null NULL

int main(void) {
	int numEnvs = 5;
	FILE *fp = stdout;

	fprintf(fp, "Creating %d environments. . .\n", numEnvs);
	Lexeme **envs = (Lexeme **)malloc(sizeof(Lexeme *) * numEnvs);
	for (int i = 0; i < numEnvs; i++)
		if (i == 0) envs[i] = create();
		else envs[i] = extend(envs[i-1], null, null);
	
	fprintf(fp, "Now displaying the global environment. . .\n");
	displayEnvironment(envs[0], fp);
	displayEnvironments(envs[0], fp);

	char *var1val = (char *)malloc(sizeof(char) * 4 + 1);
	char *temp = "var1";
	for (int i = 0; i < 4; i++)
		var1val[i] = temp[i];
	var1val[4] = 0;
	Lexeme *var1 = newLexeme(ID, UNDEFINED, UNDEFINED, var1val, UNDEFINED);
	char *val1val = (char *)malloc(sizeof(char) * 12 + 1);
	temp = "Hello World!";
	for (int i = 0; i < 12; i++)
		val1val[i] = temp[i];
	val1val[12] = 0;
	Lexeme *val1 = newLexeme(STRING, UNDEFINED, UNDEFINED, val1val, UNDEFINED);
	fprintf(fp, "Inserting %s = %s into envs[%d]. . .\n", getSval(var1), getSval(val1), 3);
	insert(envs[3], var1, val1);

	fprintf(fp, "Now displaying environments visible from envs[%d]. . .\n", 3);
	displayEnvironments(envs[3], fp);

	for (int i = 0; i < numEnvs; i++)
		freeEnv(envs[i]);
	free(envs);
	return 0;
}

