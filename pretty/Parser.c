/**
 * Parser module for checking syntax errors.
 * This is an adapted Recognizer that returns
 * a parse tree comprised of Lexemes.
 * 
 * Created by Walt Loumakis
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Parser.h"
#include "Lexeme.h"
#include "Types.h"
#include "Lexer.h"

#define null NULL

Lexeme *current;

/**************************************************************************/
/*                     Grammar Rule Method Prototypes                     */
/**************************************************************************/

/*************************************/
/*            Universals             */
/*************************************/

static Lexeme *boolean();

/*************************************/
/*            Workspaces             */
/*************************************/

static Lexeme *workspaces();
static Lexeme *workspace();
static Lexeme *workspaceObjs();
static Lexeme *workspaceObj();
static Lexeme *workspaceAttributes();
static Lexeme *workspaceAttribute();
static Lexeme *name();
static Lexeme *language();
static Lexeme *counterExamples();
static Lexeme *counterExamplesList();
static Lexeme *counterExample();
static Lexeme *learning_opt_out();
static Lexeme *system_settings();
static Lexeme *systemSettingsObj();
static Lexeme *systemSettingsAttributes();
static Lexeme *systemSettingsAttribute();
static Lexeme *tooling();
static Lexeme *toolingObj();
static Lexeme *storeGenericResponses();
static Lexeme *disambiguation();
static Lexeme *disambiguationAttributes();
static Lexeme *disambiguationAttribute();
static Lexeme *prompt();
static Lexeme *none_of_the_above_prompt();
static Lexeme *enabled();
static Lexeme *sensitivity();
static Lexeme *human_agent_assist();
static Lexeme *humanAgentAssistAttributes();
static Lexeme *humanAgentAssistAttribute();

/* Pending functions */

static int namePending();
static int languagePending();
static int counterExamplesPending();
static int learning_opt_outPending();
static int toolingPending();
static int disambiguationPending();
static int promptPending();
static int none_of_the_above_promptPending();
static int enabledPending();

/*************************************/
/*              Intents              */
/*************************************/

static Lexeme *intentsBlock();
static Lexeme *intents();
static Lexeme *intent();
static Lexeme *intentAttributes();
static Lexeme *intentAttribute();
static Lexeme *intentName();
static Lexeme *userExamples();
static Lexeme *userExample();
static Lexeme *examples();
static Lexeme *example();
static Lexeme *exampleAttributes();
static Lexeme *exampleAttribute();
static Lexeme *mentions();
static Lexeme *mentionObjs();
static Lexeme *mentionObjsAttributes();
static Lexeme *mentionObjsAttribute();

/* Pending functions */

static int intentsBlockPending();
static int intentNamePending();

/*************************************/
/*             Entities              */
/*************************************/

static Lexeme *entitiesBlock();
static Lexeme *entities();
static Lexeme *entity();
static Lexeme *entityAttributes();
static Lexeme *entityAttribute();
static Lexeme *entityName();
static Lexeme *entityValues();
static Lexeme *entityValuesList();
static Lexeme *entityValue();
static Lexeme *entityValueAttributes();
static Lexeme *entityValueAttribute();
static Lexeme *entityValueName();
static Lexeme *synonyms();
static Lexeme *synonymList();
static Lexeme *synonym();
static Lexeme *patterns();
static Lexeme *patternList();
static Lexeme *pattern();
static Lexeme *entityValueType();
static Lexeme *fuzzy_match();

/* Pending functions */

static int entitiesBlockPending();
static int entityNamePending();
static int entityValuesPending();
static int entityValueNamePending();
static int synonymPending();
static int patternsPending();

/*************************************/
/*           Decision Tree           */
/*************************************/

static Lexeme *tree();
static Lexeme *nodeList();
static Lexeme *node();
static Lexeme *nodeAttributes();
static Lexeme *nodeAttribute();

/* Pending functions */

static int treePending();

/******************/
/*      Types     */
/******************/

static Lexeme *type();

/* Pending functions */

static int typePending();

/******************/
/*      Title     */
/******************/

static Lexeme *title();

/* Pending functions */

static int titlePending();

/******************/
/*     Output     */
/******************/

static Lexeme *output();
static Lexeme *optOutputValues();
static Lexeme *outputValues();
static Lexeme *outputValue();

/* Pending functions */

static int outputPending();
static int outputValuesPending();
static int outputValuePending();

///////////////////////
//       Text        //
///////////////////////

static Lexeme *text();
static Lexeme *textChoice();
static Lexeme *textValuesObj();
static Lexeme *textValuesObjAttributes();
static Lexeme *textValuesObjAttribute();
static Lexeme *values();
static Lexeme *stringList();
static Lexeme *selectionPolicy();
static Lexeme *delimeter();

/* Pending functions */

static int textPending();
static int valuesPending();
static int selectionPolicyPending();

///////////////////////
//       Pause       //
///////////////////////

static Lexeme *pause();
static Lexeme *pauseAttributes();
static Lexeme *pauseAttribute();
static Lexeme *time();
static Lexeme *typing();

/* Pending functions */

static int pausePending();
static int timePending();

///////////////////////
//       Image       //
///////////////////////

static Lexeme *image();
static Lexeme *imageAttributes();
static Lexeme *imageAttribute();
static Lexeme *source();

/* Pending functions */

static int imagePending();
static int sourcePending();

///////////////////////
//      Option       //
///////////////////////

static Lexeme *option();
static Lexeme *optionAttributes();
static Lexeme *optionAttribute();
static Lexeme *preference();
static Lexeme *options();
static Lexeme *optionsObjList();
static Lexeme *optionsObj();
static Lexeme *optionsObjAttributes();
static Lexeme *optionsObjAttribute();
static Lexeme *label();
static Lexeme *value();
static Lexeme *inputDataObj();
static Lexeme *inputAttribute();

/* Pending functions */

static int optionPending();
static int preferencePending();
static int labelPending();

///////////////////////
// Connect to Agent  //
///////////////////////

static Lexeme *connect_to_agent();
static Lexeme *connectToAgentObj();
static Lexeme *connectToAgentObjAttribute();

/* Pending functions */

static int connect_to_agentPending();

/******************/
/*    Metadata    */
/******************/

static Lexeme *metadata();
static Lexeme *metadataObj();
static Lexeme *optMetadataAttributes();
static Lexeme *metadataAttributes();
static Lexeme *metadataAttribute();
static Lexeme *fallback();

/* Pending functions */

static int metadataPending();
static int metadataAttributesPending();
static int metadataAttributePending();
static int fallbackPending();

/******************/
/*   Conditions   */
/******************/

static Lexeme *conditions();

/* Pending functions */

static int conditionsPending();

/******************/
/*  Dialog Node   */
/******************/

static Lexeme *dialog_node();

/* Pending functions */

static int dialog_nodePending();

/******************/
/*  Prev Sibling  */
/******************/

static Lexeme *previous_sibling();

/* Pending functions */

static int previous_siblingPending();

/******************/
/*   Description  */
/******************/

static Lexeme *description();

/* Pending functions */

static int descriptionPending();

/******************/
/*     Parent     */
/******************/

static Lexeme *parent();

/* Pending functions */

static int parentPending();

/******************/
/*     Context    */
/******************/

static Lexeme *context();
static Lexeme *contextObj();
static Lexeme *contextObjAttributes();
static Lexeme *contextObjAttribute();

/* Pending functions */

static int contextPending();

/******************/
/*    Next Step   */
/******************/

static Lexeme *next_step();
static Lexeme *nextStepObj();
static Lexeme *nextStepObjAttributes();
static Lexeme *nextStepObjAttribute();
static Lexeme *behavior();
static Lexeme *selector();

/* Pending functions */

static int next_stepPending();
static int behaviorPending();

/******************/
/*     Actions    */
/******************/

static Lexeme *actions();
static Lexeme *actionsObjList();
static Lexeme *actionsObj();
static Lexeme *actionsObjAttributes();
static Lexeme *actionsObjAttribute();
static Lexeme *action_type();
static Lexeme *parameters();
static Lexeme *parametersObj();
static Lexeme *parametersAttributes();
static Lexeme *parametersAttribute();
static Lexeme *result_variable();
static Lexeme *credentials();

/* Pending functions */

static int actionsPending();
static int action_typePending();
static int parametersPending();
static int result_variablePending();

/******************/
/*   Event Name   */
/******************/

static Lexeme *event_name();

/* Pending functions */

static int event_namePending();

/******************/
/*    Variable    */
/******************/

static Lexeme *variable();

/* Pending functions */

static int variablePending();

/******************/
/*   Digress In   */
/******************/

static Lexeme *digress_in();

/* Pending functions */

static int digress_inPending();

/******************/
/*   Digress Out  */
/******************/

static Lexeme *digress_out();

/* Pending functions */

static int digress_outPending();

/******************/
/* Dgrs Out Slots */
/******************/

static Lexeme *digress_out_slots();

/* Pending functions */

static int digress_out_slotsPending();

/******************/
/*   User Label   */
/******************/

static Lexeme *user_label();

/**************************************************************************/
/*                       Private Parser  Functions                        */
/**************************************************************************/

static Lexeme *advance() {
	Lexeme *prev = current;
	current = lex();
	return prev;
}

static int check(char *type) {
	if (strcmp(type, getType(current)) == 0)
		return 1;
	return 0;
}

static Lexeme *match(char *type) {
	if (check(type))
		return advance();
	else {
		fprintf(stderr, "Error on line %d: Expected %s, got %s.\n", getLine(current), type, getType(current));
		exit(2);
	}
}

/**************************************************************************/
/*                        Public Parser Functions                         */
/**************************************************************************/

void initParser(char *filename) {
	initLexer(filename);
}

void closeParser() {
	closeLexer();
}

void freeParseTree(Lexeme *tree) {
	if (tree == null)
		return;
	freeParseTree(car(tree));
	freeParseTree(cdr(tree));
	free(tree);
}

Lexeme *parse() {
	current = lex();
	Lexeme *tree = workspaces();
	free(match(ENDofINPUT));
	return tree;
}

/**************************************************************************/
/*                          Grammar Rule Methods                          */
/**************************************************************************/

/*************************************/
/*            Universals             */
/*************************************/

static Lexeme *boolean() {
	if (check(TRUEVAL))
		return advance();
	else if (check(FALSEVAL))
		return advance();
	else
		return newLexeme(BAD_BOOL, UNDEFINED, UNDEFINED, getType(current), getLine(current));
}

/*************************************/
/*            Workspaces             */
/*************************************/

static Lexeme *workspaces() {
	Lexeme *a, *b;
	a = workspace();
	if (check(COMMA)) {
		free(advance());
		b = workspaces();
	}
	else
		b = null;
	return cons(WORKSPACES, a, b);
}

static Lexeme *workspace() {
	Lexeme *a;
	free(match(WORKSPACE));
	free(match(COLON));
	free(match(OBRACKET));
	a = workspaceObjs();
	free(match(CBRACKET));
	return a;
}

static Lexeme *workspaceObjs() {
	Lexeme *a, *b;
	a = workspaceObj();
	if (check(COMMA)) {
		free(advance());
		b = workspaceObjs();
	}
	else
		b = null;
	return cons(WORKSPACE_OBJ, a, b);
}

static Lexeme *workspaceObj() {
	Lexeme *a;
	free(match(OBRACE));
	a = workspaceAttributes();
	free(match(CBRACE));
	return a;
}

static Lexeme *workspaceAttributes() {
	Lexeme *a, *b;
	a = workspaceAttribute();
	if (check(COMMA)) {
		free(advance());
		b = workspaceAttributes();
	}
	else
		b = null;
	return cons(WORKSPACE_ATTR, a, b);
}

static Lexeme *workspaceAttribute() {
	if (namePending())
		return name();
	else if (descriptionPending())
		return description();
	else if (languagePending())
		return language();
	else if (intentsBlockPending())
		return intentsBlock();
	else if (entitiesBlockPending())
		return entitiesBlock();
	else if (treePending())
		return tree();
	else if (counterExamplesPending())
		return counterExamples();
	else if (metadataPending())
		return metadata();
	else if (learning_opt_outPending())
		return learning_opt_out();
	else
		return system_settings();
}

static Lexeme *name() {
	free(match(NAME));
	free(match(COLON));
	return cons(NAME, match(STRING), null);
}

static Lexeme *language() {
	free(match(LANGUAGE));
	free(match(COLON));
	return cons(LANGUAGE, match(STRING), null);
}

static Lexeme *counterExamples() {
	Lexeme *a;
	free(match(COUNTEREXAMPLES));
	free(match(COLON));
	free(match(OBRACKET));
	a = counterExamplesList();
	free (match(CBRACKET));
	return a;
}

static Lexeme *counterExamplesList() {
	Lexeme *a, *b;
	a = counterExample();
	if (check(COMMA)) {
		free(advance());
		b = counterExamplesList();
	}
	else
		b = null;
	return cons(COUNTEREXAMPLES, a, b);
}

static Lexeme *counterExample() {
	free(match(TEXT));
	free(match(COLON));
	return match(STRING);
}

static Lexeme *learning_opt_out() {
	free(match(LEARNING_OPT_OUT));
	free(match(COLON));
	return cons(LEARNING_OPT_OUT, boolean(), null);
}

static Lexeme *system_settings() {
	free(match(SYSTEM_SETTINGS));
	free(match(COLON));
	return systemSettingsObj();
}

static Lexeme *systemSettingsObj() {
	Lexeme *a;
	free(match(OBRACE));
	a = systemSettingsAttributes();
	free(match(CBRACE));
	return a;
}

static Lexeme *systemSettingsAttributes() {
	Lexeme *a, *b;
	a = systemSettingsAttribute();
	if (check(COMMA)) {
		free(advance());
		b = systemSettingsAttributes();
	}
	else
		b = null;
	return cons(SYSTEM_SETTINGS, a, b);
}

static Lexeme *systemSettingsAttribute() {
	if (toolingPending())
		return tooling();
	else if (disambiguationPending())
		return disambiguation();
	else
		return human_agent_assist();
}

static Lexeme *tooling() {
	free(match(TOOLING));
	free(match(COLON));
	return cons(TOOLING, toolingObj(), null);
}

static Lexeme *toolingObj() {
	Lexeme *a;
	free(match(OBRACE));
	a = storeGenericResponses();
	free(match(CBRACE));
	return a;
}

static Lexeme *storeGenericResponses() {
	free(match(STORE_GENERIC_RESPONSES));
	free(match(COLON));
	return boolean();
}

static Lexeme *disambiguation() {
	Lexeme *a;
	free(match(DISAMBIGUATION));
	free(match(COLON));
	free(match(OBRACE));
	a = disambiguationAttributes();
	free(match(CBRACE));
	return a;
}

static Lexeme *disambiguationAttributes() {
	Lexeme *a, *b;
	a = disambiguationAttribute();
	if (check(COMMA)) {
		free(advance());
		b = disambiguationAttributes();
	}
	else
		b = null;
	return cons(DISAMBIGUATION, a, b);
}

static Lexeme *disambiguationAttribute() {
	if (promptPending())
		return prompt();
	else if (none_of_the_above_promptPending())
		return none_of_the_above_prompt();
	else if (enabledPending())
		return enabled();
	else
		return sensitivity();
}

static Lexeme *prompt() {
	free(match(PROMPT));
	free(match(COLON));
	return cons(PROMPT, match(STRING), null);
}

static Lexeme *none_of_the_above_prompt() {
	free(match(NONE_OF_THE_ABOVE_PROMPT));
	free(match(COLON));
	return cons(NONE_OF_THE_ABOVE_PROMPT, match(STRING), null);
}

static Lexeme *enabled() {
	free(match(ENABLED));
	free(match(COLON));
	return cons(ENABLED, boolean(), null);
}

static Lexeme *sensitivity() {
	free(match(SENSITIVITY));
	free(match(COLON));
	return cons(SENSITIVITY, match(STRING), null);
}

static Lexeme *human_agent_assist() {
	Lexeme *a;
	free(match(OBRACE));
	a = humanAgentAssistAttributes();
	free(match(CBRACE));
	return a;
}

static Lexeme *humanAgentAssistAttributes() {
	Lexeme *a, *b;
	a = humanAgentAssistAttribute();
	if (check(COMMA)) {
		free(advance());
		b = humanAgentAssistAttributes();
	}
	else
		b = null;
	return cons(HUMAN_AGENT_ASSIST, a, b);
}

//TODO: Figure out a more comprehensive list of humanAgentAssist attributes.
static Lexeme *humanAgentAssistAttribute() {
	return null;
}

/* Pending functions */

static int namePending() {
	return check(NAME);
}

static int languagePending() {
	return check(LANGUAGE);
}

static int counterExamplesPending() {
	return check(COUNTEREXAMPLES);
}

static int learning_opt_outPending() {
	return check(LEARNING_OPT_OUT);
}

static int toolingPending() {
	return check(TOOLING);
}

static int disambiguationPending() {
	return check(DISAMBIGUATION);
}

static int promptPending() {
	return check(PROMPT);
}

static int none_of_the_above_promptPending() {
	return check(NONE_OF_THE_ABOVE_PROMPT);
}

static int enabledPending() {
	return check(ENABLED);
}

/*************************************/
/*              Intents              */
/*************************************/

static Lexeme *intentsBlock() {
	Lexeme *a;
	free(match(INTENTS));
	free(match(COLON));
	free(match(OBRACKET));
	a = intents();
	free(match(CBRACKET));
	return a;
}

static Lexeme *intents() {
	Lexeme *a, *b;
	a = intent();
	if (check(COMMA)) {
		free(advance());
		b = intents();
	}
	else
		b = null;
	return cons(INTENTS, a, b);
}

static Lexeme *intent() {
	Lexeme *a;
	free(match(OBRACE));
	a = intentAttributes();
	free(match(CBRACE));
	return a;
}

static Lexeme *intentAttributes() {
	Lexeme *a, *b;
	a = intentAttribute();
	if (check(COMMA)) {
		free(advance());
		b = intentAttributes();
	}
	else
		b = null;
	return cons(INTENT, a, b);
}

static Lexeme *intentAttribute() {
	if (intentNamePending())
		return intentName();
	else if (descriptionPending())
		return description();
	else
		return userExamples();
}

static Lexeme *intentName() {
	free(match(INTENT));
	free(match(COLON));
	return cons(INTENT_NAME, match(STRING), null);
}

static Lexeme *userExamples() {
	Lexeme *a, *b;
	a = userExample();
	if (check(COMMA)) {
		free(advance());
		b = userExamples();
	}
	else
		b = null;
	return cons(USER_EXAMPLES, a, b);
}

static Lexeme *userExample() {
	Lexeme *a;
	free(match(EXAMPLES));
	free(match(COLON));
	free(match(OBRACKET));
	a = examples();
	free(match(CBRACKET));
	return a;
}

static Lexeme *examples() {
	Lexeme *a, *b;
	a = example();
	if (check(COMMA)) {
		free(advance());
		b = examples();
	}
	else
		b = null;
	return cons(EXAMPLES, a, b);
}

static Lexeme *example() {
	Lexeme *a;
	free(match(OBRACE));
	a = exampleAttributes();
	free(match(CBRACE));
	return a;
}

static Lexeme *exampleAttributes() {
	Lexeme *a, *b;
	a = exampleAttribute();
	if (check(COMMA)) {
		free(advance());
		b = exampleAttributes();
	}
	else
		b = null;
	return cons(EXAMPLE_ATTR, a, b);
}

static Lexeme *exampleAttribute() {
	if (textPending())
		return text();
	else
		return mentions();
}

static Lexeme *mentions() {
	Lexeme *a;
	free(match(MENTIONS));
	free(match(COLON));
	free(match(OBRACKET));
	a = mentionObjs();
	free(match(CBRACKET));
	return a;
}

static Lexeme *mentionObjs() {
	Lexeme *a;
	free(match(OBRACE));
	a = mentionObjsAttributes();
	free(match(CBRACE));
	return a;
}

static Lexeme *mentionObjsAttributes() {
	Lexeme *a, *b;
	a = mentionObjsAttribute();
	if (check(COMMA)) {
		free(advance());
		b = mentionObjsAttributes();
	}
	else
		b = null;
	return cons(MENTIONS_ATTR, a, b);
}

static Lexeme *mentionObjsAttribute() {
	return entityName();
}

/* Pending functions */

static int intentsBlockPending() {
	return check(INTENTS);
}

static int intentNamePending() {
	return check(INTENT);
}

/*************************************/
/*             Entities              */
/*************************************/

static Lexeme *entitiesBlock() {
	Lexeme *a;
	free(match(ENTITIES));
	free(match(COLON));
	free(match(OBRACKET));
	a = entities();
	free(match(CBRACKET));
	return a;
}

static Lexeme *entities() {
	Lexeme *a, *b;
	a = entity();
	if (check(COMMA)) {
		free(advance());
		b = entities();
	}
	else
		b = null;
	return cons(ENTITIES, a, b);
}

static Lexeme *entity() {
	Lexeme *a;
	free(match(OBRACE));
	a = entityAttributes();
	free(match(CBRACE));
	return a;
}

static Lexeme *entityAttributes() {
	Lexeme *a, *b;
	a= entityAttribute();
	if (check(COMMA)) {
		free(advance());
		b = entityAttributes();
	}
	else
		b = null;
	return cons(ENTITY_ATTR, a, b);
}

static Lexeme *entityAttribute() {
	if (entityNamePending())
		return entityName();
	else if (descriptionPending())
		return description();
	else if (metadataPending())
		return metadata();
	else if (entityValuesPending())
		return entityValues();
	else
		return fuzzy_match();
}

static Lexeme *entityName() {
	free(match(ENTITY));
	free(match(COLON));
	return cons(ENTITY_NAME, match(STRING), null);
}

static Lexeme *entityValues() {
	Lexeme *a;
	free(match(VALUES));
	free(match(COLON));
	free(match(OBRACKET));
	a = entityValuesList();
	free(match(CBRACKET));
	return a;
}

static Lexeme *entityValuesList() {
	Lexeme *a, *b;
	a = entityValue();
	if (check(COMMA)) {
		free(advance());
		b = entityValuesList();
	}
	else
		b = null;
	return cons(ENTITY_VALS, a, b);
}

static Lexeme *entityValue() {
	Lexeme *a;
	free(match(OBRACE));
	a = entityValueAttributes();
	free(match(CBRACE));
	return a;
}

static Lexeme *entityValueAttributes() {
	Lexeme *a, *b;
	a = entityValueAttribute();
	if (check(COMMA)) {
		free(advance());
		b = entityValueAttributes();
	}
	else
		b = null;
	return cons(ENTITY_VAL_ATTR, a, b);
}

static Lexeme *entityValueAttribute() {
	if (entityValueNamePending())
		return entityValueName();
	else if (metadataPending())
		return metadata();
	else if (synonymPending())
		return synonyms();
	else if (patternsPending())
		return patterns();
	else
		return entityValueType();
}

static Lexeme *entityValueName() {
	free(match(VALUE));
	free(match(COLON));
	return cons(ENTITY_VAL_NAME, match(STRING), null);
}

static Lexeme *synonyms() {
	Lexeme *a;
	free(match(SYNONYMS));
	free(match(COLON));
	free(match(OBRACKET));
	a = synonymList();
	free(match(CBRACKET));
	return a;
}

static Lexeme *synonymList() {
	Lexeme *a, *b;
	a = synonym();
	if (check(COMMA)) {
		free(advance());
		b = synonymList();
	}
	else
		b = null;
	return cons(SYNONYMS, a, b);
}

static Lexeme *synonym() {
	free(match(SYNONYM));
	free(match(COLON));
	return match(STRING);
}

static Lexeme *patterns() {
	Lexeme *a;
	free(match(PATTERNS));
	free(match(COLON));
	free(match(OBRACKET));
	a = patternList();
	free(match(CBRACKET));
	return a;
}

static Lexeme *patternList() {
	Lexeme *a, *b;
	a = pattern();
	if (check(COMMA)) {
		free(advance());
		b = patternList();
	}
	else
		b = null;
	return cons(PATTERNS, a, b);
}

static Lexeme *pattern() {
	free(match(PATTERN));
	free(match(COLON));
	return match(STRING);
}

static Lexeme *entityValueType() {
	free(match(VALUE_TYPE));
	free(match(COLON));
	return cons(ENTITY_VAL_TYPE, match(STRING), null);
}

static Lexeme *fuzzy_match() {
	free(match(FUZZY_MATCH));
	free(match(COLON));
	return cons(FUZZY_MATCH, boolean(), null);
}

/* Pending functions */

static int entitiesBlockPending() {
	return check(ENTITIES);
}

static int entityNamePending() {
	return check(ENTITY);
}

static int entityValuesPending() {
	return check(VALUES);
}

static int entityValueNamePending() {
	return check(VALUE);
}

static int synonymPending() {
	return check(SYNONYM);
}

static int patternsPending() {
	return check(PATTERNS);
}

/*************************************/
/*           Decision Tree           */
/*************************************/

static Lexeme *tree() {
	Lexeme *a;
	free(match(TREE));
	free(match(COLON));
	free(match(OBRACKET));
	a = nodeList();
	free(match(CBRACKET));
	return cons(TREE, a, null);
}

static Lexeme *nodeList() {
	Lexeme *a, *b;
	a = node();
	if (check(COMMA)) {
		free(advance());
		b = nodeList();
	}
	else
		b = null;
	return cons(NODE_LIST, a, b);
}

static Lexeme *node() {
	Lexeme *a;
	free(match(OBRACE));
	a = nodeAttributes();
	free(match(CBRACE));
	return a;
}

static Lexeme *nodeAttributes() {
	Lexeme *a, *b;
	a = nodeAttribute();
	if (check(COMMA)) {
		free(advance());
		b = nodeAttributes();
	}
	else
		b = null;
	return cons(NODE_ATTR, a, b);
}

static Lexeme *nodeAttribute() {
	if (typePending())
		return type();
	else if (titlePending())
		return title();
	else if (outputPending())
		return output();
	else if (metadataPending())
		return metadata();
	else if (conditionsPending())
		return conditions();
	else if (dialog_nodePending())
		return dialog_node();
	else if (previous_siblingPending())
		return previous_sibling();
	else if (descriptionPending())
		return description();
	else if (parentPending())
		return parent();
	else if (contextPending())
		return context();
	else if (next_stepPending())
		return next_step();
	else if (actionsPending())
		return actions();
	else if (event_namePending())
		return event_name();
	else if (variablePending())
		return variable();
	else if (digress_inPending())
		return digress_in();
	else if (digress_outPending())
		return digress_out();
	else if (digress_out_slotsPending())
		return digress_out_slots();
	else
		return user_label();
}

/* Pending functions */

static int treePending() {
	return check(TREE);
}

/******************/
/*      Types     */
/******************/

static Lexeme *type() {
	free(match(TYPE));
	free(match(COLON));
	return cons(TYPE, match(STRING), null);
}

/* Pending functions */

static int typePending() {
	return check(TYPE);
}

/******************/
/*      Title     */
/******************/

static Lexeme *title() {
	free(match(TITLE));
	free(match(COLON));
	return cons(TITLE, match(STRING), null);
}

/* Pending functions */

static int titlePending() {
	return check(TITLE);
}

/******************/
/*     Output     */
/******************/

static Lexeme *output() {
	Lexeme *a;
	free(match(OUTPUT));
	free(match(COLON));
	free(match(OBRACE));
	a = optOutputValues();
	free(match(CBRACE));
	return a; 
}

static Lexeme *optOutputValues() {
	if (outputValuesPending())
		return outputValues();
	return null;
}

static Lexeme *outputValues() {
	Lexeme *a, *b;
	a = outputValue();
	if (check(COMMA)) {
		free(advance());
		b = outputValues();
	}
	else
		b = null;
	return cons(OUTPUT_VALS, a, b);
}

static Lexeme *outputValue() {
	if (textPending())
		return text();
	else if (pausePending())
		return pause();
	else if (imagePending())
		return image();
	else if (optionPending())
		return option();
	else
		return connect_to_agent();
}

/* Pending functions */

static int outputPending() {
	return check(OUTPUT);
}

static int outputValuesPending() {
	return outputValuePending();
}

static int outputValuePending() {
	return (textPending() ||
			pausePending() ||
			imagePending() ||
			optionPending() ||
			connect_to_agentPending());
}

///////////////////////
//       Text        //
///////////////////////

static Lexeme *text() {
	free(match(TEXT));
	free(match(COLON));
	return cons(TEXT, textChoice(), null);
}

static Lexeme *textChoice() {
	if (check(STRING))
		return match(STRING);
	else
		return textValuesObj();
}

static Lexeme *textValuesObj() {
	Lexeme *a;
	free(match(OBRACE));
	a = textValuesObjAttributes();
	free(match(CBRACE));
	return a;
}

static Lexeme *textValuesObjAttributes() {
	Lexeme *a, *b;
	a = textValuesObjAttribute();
	if (check(COMMA)) {
		free(advance());
		b = textValuesObjAttributes();
	}
	else
		b = null;
	return cons(TEXT_VAL_ATTR, a, b);
}

static Lexeme *textValuesObjAttribute() {
	if (valuesPending())
		return values();
	else if (selectionPolicyPending())
		return selectionPolicy();
	else
		return delimeter();
}

static Lexeme *values() {
	Lexeme *a;
	free(match(VALUES));
	free(match(COLON));
	free(match(OBRACKET));
	a = stringList();
	free(match(CBRACKET));
	return a;
}

static Lexeme *stringList() {
	Lexeme *a, *b;
	a = match(STRING);
	if (check(COMMA)) {
		free(advance());
		b = stringList();
	}
	else
		b = null;
	return cons(STRING_LIST, a, b);
}

static Lexeme *selectionPolicy() {
	free(match(SELECTION_POLICY));
	free(match(COLON));
	return cons(SELECTION_POLICY, match(STRING), null);
}

static Lexeme *delimeter() {
	free(match(DELIMETER));
	free(match(COLON));
	return cons(DELIMETER, match(STRING), null);
}

/* Pending functions */

static int textPending() {
	return check(TEXT);
}

static int valuesPending() {
	return check(VALUES);
}

static int selectionPolicyPending() {
	return check(SELECTION_POLICY);
}

///////////////////////
//       Pause       //
///////////////////////

static Lexeme *pause() {
	Lexeme *a;
	free(match(PAUSE));
	free(match(COLON));
	free(match(OBRACE));
	a = pauseAttributes();
	free(match(CBRACE));
	return cons(PAUSE, a, null);
}

static Lexeme *pauseAttributes() {
	Lexeme *a, *b;
	a = pauseAttribute();
	if (check(COMMA)) {
		free(advance());
		b = pauseAttributes();
	}
	else
		b = null;
	return cons(PAUSE_ATTR, a, b);
}

static Lexeme *pauseAttribute() {
	if (timePending())
		return time();
	else
		return typing();
}

static Lexeme *time() {
	free(match(TIME));
	free(match(COLON));
	return cons(TIME, match(NUMBER), null);
}

static Lexeme *typing() {
	free(match(TYPING));
	free(match(COLON));
	return cons(TYPING, boolean(), null);
}

/* Pending functions */

static int pausePending() {
	return check(PAUSE);
}

static int timePending() {
	return check(TIME);
}

///////////////////////
//       Image       //
///////////////////////

static Lexeme *image() {
	Lexeme *a;
	free(match(IMAGE));
	free(match(COLON));
	free(match(OBRACE));
	a = imageAttributes();
	free(match(CBRACE));
	return a;
}

static Lexeme *imageAttributes() {
	Lexeme *a, *b;
	a = imageAttribute();
	if (check(COMMA)) {
		free(advance());
		b = imageAttributes();
	}
	else
		b = null;
	return cons(IMAGE_ATTR, a, b);
}

static Lexeme *imageAttribute() {
	if (sourcePending())
		return source();
	else if (titlePending())
		return title();
	else
		return description();
}

static Lexeme *source() {
	free(match(SOURCE));
	free(match(COLON));
	return cons(SOURCE, match(STRING), null);
}

/* Pending functions */

static int imagePending() {
	return check(IMAGE);
}

static int sourcePending() {
	return check(SOURCE);
}

///////////////////////
//      Option       //
///////////////////////

static Lexeme *option() {
	Lexeme *a;
	free(match(OPTION));
	free(match(COLON));
	free(match(OBRACE));
	a = optionAttributes();
	free(match(CBRACE));
	return a;
}

static Lexeme *optionAttributes() {
	Lexeme *a, *b;
	a = optionAttribute();
	if (check(COMMA)) {
		free(advance());
		b = optionAttributes();
	}
	else
		b = null;
	return cons(OPTION_ATTR, a, b);
}

static Lexeme *optionAttribute() {
	if (titlePending())
		return title();
	else if (descriptionPending())
		return description();
	else if (preferencePending())
		return preference();
	else
		return options();
}

static Lexeme *preference() {
	free(match(PREFERENCE));
	free(match(COLON));
	return cons(PREFERENCE, match(STRING), null);
}

static Lexeme *options() {
	Lexeme *a;
	free(match(OBRACKET));
	a = optionsObjList();
	free(match(CBRACKET));
	return a;
}

static Lexeme *optionsObjList() {
	Lexeme *a, *b;
	a = optionsObj();
	if (check(COMMA)) {
		free(advance());
		b = optionsObjList();
	}
	else
		b = null;
	return cons(OPTIONS_OBJS, a, b);
}

static Lexeme *optionsObj() {
	Lexeme *a;
	free(match(OBRACE));
	a = optionsObjAttributes();
	free(match(CBRACE));
	return a;
}

static Lexeme *optionsObjAttributes() {
	Lexeme *a, *b;
	a = optionsObjAttribute();
	if (check(COMMA)) {
		free(advance());
		b = optionsObjAttributes();
	}
	else
		b = null;
	return cons(OPTIONS_OBJ_ATTR, a, b);
}

static Lexeme *optionsObjAttribute() {
	if (labelPending())
		return label();
	else
		return value();
}

static Lexeme *label() {
	free(match(LABEL));
	free(match(COLON));
	return cons(LABEL, match(STRING), null);
}

static Lexeme *value() {
	free(match(VALUE));
	free(match(COLON));
	return inputDataObj();
}

static Lexeme *inputDataObj() {
	Lexeme *a;
	free(match(OBRACE));
	a = inputAttribute();
	free(match(CBRACE));
	return a;
}

static Lexeme *inputAttribute() {
	free(match(TEXT));
	free(match(COLON));
	return cons(TEXT, match(STRING), null);
}

/* Pending functions */

static int optionPending() {
	return check(OPTION);
}

static int preferencePending() {
	return check(PREFERENCE);
}

static int labelPending() {
	return check(LABEL);
}

///////////////////////
// Connect to Agent  //
///////////////////////

static Lexeme *connect_to_agent() {
	free(match(CONNECT_TO_AGENT));
	free(match(COLON));
	return cons(CONNECT_TO_AGENT, connectToAgentObj(), null);
}

static Lexeme *connectToAgentObj() {
	Lexeme *a;
	free(match(OBRACE));
	a = connectToAgentObjAttribute();
	free(match(CBRACE));
	return a;
}

static Lexeme *connectToAgentObjAttribute() {
	free(match(MESSAGE_TO_HUMAN_AGENT));
	free(match(COLON));
	return cons(MESSAGE_TO_HUMAN_AGENT, match(STRING), null);
}

/* Pending functions */

static int connect_to_agentPending() {
	return check(CONNECT_TO_AGENT);
}

/******************/
/*    Metadata    */
/******************/

static Lexeme *metadata() {
	free(match(METADATA));
	free(match(COLON));
	return cons(METADATA, metadataObj(), null);
}

static Lexeme *metadataObj() {
	Lexeme *a;
	free(match(OBRACE));
	a = optMetadataAttributes();
	free(match(CBRACE));
	return a;
}

static Lexeme *optMetadataAttributes() {
	if (metadataAttributesPending())
		return metadataAttributes();
	return null;
}

static Lexeme *metadataAttributes() {
	Lexeme *a, *b;
	a = metadataAttribute();
	if (check(COMMA)) {
		free(advance());
		b = metadataAttributes();
	}
	else
		b = null;
	return cons(METADATA_ATTR, a, b);
}

static Lexeme *metadataAttribute() {
	if (fallbackPending())
		return fallback();
	return null;
}

static Lexeme *fallback() {
	free(match(FALLBACK));
	free(match(COLON));
	return cons(FALLBACK, match(STRING), null);
}

/* Pending functions */

static int metadataPending() {
	return check(METADATA);
}

static int metadataAttributesPending() {
	return metadataAttributePending();
}

static int metadataAttributePending() {
	return (fallbackPending());
}

static int fallbackPending() {
	return check(FALLBACK);
}

/******************/
/*   Conditions   */
/******************/

static Lexeme *conditions() {
	free(match(CONDITIONS));
	free(match(COLON));
	return cons(CONDITIONS, match(STRING), null);
}

/* Pending functions */

static int conditionsPending() {
	return check(CONDITIONS);
}

/******************/
/*  Dialog Node   */
/******************/

static Lexeme *dialog_node() {
	free(match(DIALOG_NODE));
	free(match(COLON));
	return cons(DIALOG_NODE, match(STRING), null);
}

/* Pending functions */

static int dialog_nodePending() {
	return check(DIALOG_NODE);
}

/******************/
/*  Prev Sibling  */
/******************/

static Lexeme *previous_sibling() {
	free(match(PREVIOUS_SIBLING));
	free(match(COLON));
	return cons(PREVIOUS_SIBLING, match(STRING), null);
}

/* Pending functions */

static int previous_siblingPending() {
	return check(PREVIOUS_SIBLING);
}

/******************/
/*   Description  */
/******************/

static Lexeme *description() {
	free(match(DESCRIPTION));
	free(match(COLON));
	return cons(DESCRIPTION, match(STRING), null);
}

/* Pending functions */

static int descriptionPending() {
	return check(DESCRIPTION);
}

/******************/
/*     Parent     */
/******************/

static Lexeme *parent() {
	free(match(PARENT));
	free(match(COLON));
	return cons(PARENT, match(STRING), null);
}

/* Pending functions */

static int parentPending() {
	return check(PARENT);
}

/******************/
/*     Context    */
/******************/

static Lexeme *context() {
	free(match(CONTEXT));
	free(match(COLON));
	return cons(CONTEXT, contextObj(), null);
}

static Lexeme *contextObj() {
	Lexeme *a;
	free(match(OBRACE));
	a = contextObjAttributes();
	free(match(CBRACE));
	return a;
}

static Lexeme *contextObjAttributes() {
	Lexeme *a, *b;
	a = contextObjAttribute();
	if (check(COMMA)) {
		free(advance());
		b = contextObjAttributes();
	}
	else
		b = null;
	return cons(CONTEXT_OBJ_ATTR, a, b);
}

//TODO: Figure out a more comprehensive list of context attributes.
static Lexeme *contextObjAttribute() {
	return null;
}

/* Pending functions */

static int contextPending() {
	return check(CONTEXT);
}

/******************/
/*    Next Step   */
/******************/

static Lexeme *next_step() {
	free(match(NEXT_STEP));
	free(match(COLON));
	return cons(NEXT_STEP, nextStepObj(), null);
}

static Lexeme *nextStepObj() {
	Lexeme *a;
	free(match(OBRACE));
	a = nextStepObjAttributes();
	free(match(CBRACE));
	return a;
}

static Lexeme *nextStepObjAttributes() {
	Lexeme *a, *b;
	a = nextStepObjAttribute();
	if (check(COMMA)) {
		free(advance());
		b = nextStepObjAttributes();
	}
	else
		b = null;
	return cons(NEXT_STEP_ATTR, a, b);
}

static Lexeme *nextStepObjAttribute() {
	if (behaviorPending())
		return behavior();
	else if (dialog_nodePending())
		return dialog_node();
	else
		return selector();
}

static Lexeme *behavior() {
	free(match(BEHAVIOR));
	free(match(COLON));
	return cons(BEHAVIOR, match(STRING), null);
}

static Lexeme *selector() {
	free(match(SELECTOR));
	free(match(COLON));
	return cons(SELECTOR, match(STRING), null);
}

/* Pending functions */

static int next_stepPending() {
	return check(NEXT_STEP);
}

static int behaviorPending() {
	return check(BEHAVIOR);
}

/******************/
/*     Actions    */
/******************/

static Lexeme *actions() {
	Lexeme *a;
	free(match(ACTIONS));
	free(match(COLON));
	free(match(OBRACKET));
	a = actionsObjList();
	free(match(CBRACKET));
	return a;
}

static Lexeme *actionsObjList() {
	Lexeme *a, *b;
	a = actionsObj();
	if (check(COMMA)) {
		free(advance());
		b = actionsObjList();
	}
	else
		b = null;
	return cons(ACTIONS, a, b);
}

static Lexeme *actionsObj() {
	Lexeme *a;
	free(match(OBRACE));
	a = actionsObjAttributes();
	free(match(CBRACE));
	return a;
}

static Lexeme *actionsObjAttributes() {
	Lexeme *a, *b;
	a = actionsObjAttribute();
	if (check(COMMA)) {
		free(advance());
		b = actionsObjAttributes();
	}
	else
		b = null;
	return cons(ACTIONS_ATTR, a, b);
}

static Lexeme *actionsObjAttribute() {
	if (namePending())
		return name();
	else if (action_typePending())
		return action_type();
	else if (parametersPending())
		return parameters();
	else if (result_variablePending())
		return result_variable();
	else
		return credentials();
}

static Lexeme *action_type() {
	free(match(ACTION_TYPE));
	free(match(COLON));
	return cons(ACTION_TYPE, match(STRING), null);
}

static Lexeme *parameters() {
	free(match(PARAMETERS));
	free(match(COLON));
	return cons(PARAMETERS, parametersObj(), null);
}

static Lexeme *parametersObj() {
	Lexeme *a;
	free(match(OBRACE));
	a = parametersAttributes();
	free(match(CBRACE));
	return a;
}

static Lexeme *parametersAttributes() {
	Lexeme *a, *b;
	a = parametersAttribute();
	if (check(COMMA)) {
		free(advance());
		b = parametersAttributes();
	}
	else
		b = null;
	return cons(PARAMETERS_ATTR, a, b);
}

//TODO: Figure out a more comprehensive list of parameters attributes.
static Lexeme *parametersAttribute() {
	return null;
}

static Lexeme *result_variable() {
	free(match(RESULT_VARIABLE));
	free(match(COLON));
	return cons(RESULT_VARIABLE, match(STRING), null);
}

static Lexeme *credentials() {
	free(match(CREDENTIALS));
	free(match(COLON));
	return cons(CREDENTIALS, match(STRING), null);
}

/* Pending functions */

static int actionsPending() {
	return check(ACTIONS);
}

static int action_typePending() {
	return check(ACTION_TYPE);
}

static int parametersPending() {
	return check(PARAMETERS);
}

static int result_variablePending() {
	return check(RESULT_VARIABLE);
}

/******************/
/*   Event Name   */
/******************/

static Lexeme *event_name() {
	free(match(EVENT_NAME));
	free(match(COLON));
	return cons(EVENT_NAME, match(STRING), null);
}

/* Pending functions */

static int event_namePending() {
	return check(EVENT_NAME);
}

/******************/
/*    Variable    */
/******************/

static Lexeme *variable() {
	free(match(VARIABLE));
	free(match(COLON));
	return cons(VARIABLE, match(STRING), null);
}

/* Pending functions */

static int variablePending() {
	return check(VARIABLE);
}

/******************/
/*   Digress In   */
/******************/

static Lexeme *digress_in() {
	free(match(DIGRESS_IN));
	free(match(COLON));
	return cons(DIGRESS_IN, match(STRING), null);
}

/* Pending functions */

static int digress_inPending() {
	return check(DIGRESS_IN);
}

/******************/
/*   Digress Out  */
/******************/

static Lexeme *digress_out() {
	free(match(DIGRESS_OUT));
	free(match(COLON));
	return cons(DIGRESS_OUT, match(STRING), null);
}

/* Pending functions */

static int digress_outPending() {
	return check(DIGRESS_OUT);
}

/******************/
/* Dgrs Out Slots */
/******************/

static Lexeme *digress_out_slots() {
	free(match(DIGRESS_OUT_SLOTS));
	free(match(COLON));
	return cons(DIGRESS_OUT_SLOTS, match(STRING), null);
}

/* Pending functions */

static int digress_out_slotsPending() {
	return check(DIGRESS_OUT_SLOTS);
}

/******************/
/*   User Label   */
/******************/

static Lexeme *user_label() {
	free(match(USER_LABEL));
	free(match(COLON));
	return cons(USER_LABEL, match(STRING), null);
}
