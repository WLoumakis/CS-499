/**
 * Recognizer module for checking syntax errors.
 * 
 * Created by Walt Loumakis.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Recognizer.h"
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

static void boolean();

/*************************************/
/*            Workspaces             */
/*************************************/

static void workspaces();
static void workspace();
static void workspaceObjs();
static void workspaceObj();
static void workspaceAttributes();
static void workspaceAttribute();
static void name();
static void language();
static void counterExamples();
static void counterExamplesList();
static void counterExample();
static void learning_opt_out();
static void system_settings();
static void systemSettingsObj();
static void systemSettingsAttributes();
static void systemSettingsAttribute();
static void tooling();
static void toolingObj();
static void storeGenericResponses();
static void disambiguation();
static void disambiguationAttributes();
static void disambiguationAttribute();
static void prompt();
static void none_of_the_above_prompt();
static void enabled();
static void sensitivity();
static void human_agent_assist();
static void humanAgentAssistAttributes();
static void humanAgentAssistAttribute();

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

static void intentsBlock();
static void intents();
static void intent();
static void intentAttributes();
static void intentAttribute();
static void intentName();
static void userExamples();
static void userExample();
static void examples();
static void example();
static void exampleAttributes();
static void exampleAttribute();
static void mentions();
static void mentionObjs();
static void mentionObjsAttributes();
static void mentionObjsAttribute();

static int intentsBlockPending();
static int intentNamePending();

/*************************************/
/*             Entities              */
/*************************************/

static void entitiesBlock();
static void entities();
static void entity();
static void entityAttributes();
static void entityAttribute();
static void entityName();
static void entityValues();
static void entityValuesList();
static void entityValue();
static void entityValueAttributes();
static void entityValueAttribute();
static void entityValueName();
static void synonyms();
static void synonymList();
static void synonym();
static void patterns();
static void patternList();
static void pattern();
static void entityValueType();
static void fuzzy_match();

static int entitiesBlockPending();
static int entityNamePending();
static int entityValuesPending();
static int entityValueNamePending();
static int synonymPending();
static int patternsPending();

/*************************************/
/*           Decision Tree           */
/*************************************/

static void tree();
static void nodeList();
static void node();
static void nodeAttributes();
static void nodeAttribute();

static int treePending();

/******************/
/*      Types     */
/******************/

static void type();

static int typePending();

/******************/
/*      Title     */
/******************/

static void title();

static int titlePending();

/******************/
/*    Children    */
/******************/

static void children();

static int childrenPending();

/******************/
/*     Output     */
/******************/

static void output();
static void optOutputValues();
static void outputValues();
static void outputValue();

static int outputPending();
static int outputValuesPending();
static int outputValuePending();

///////////////////////
//       Text        //
///////////////////////

static void text();
static void textChoice();
static void textValuesObj();
static void textValuesObjAttributes();
static void textValuesObjAttribute();
static void values();
static void stringList();
static void selectionPolicy();
static void delimeter();

static int textPending();
static int valuesPending();
static int selectionPolicyPending();

///////////////////////
//       Pause       //
///////////////////////

static void pause();
static void pauseAttributes();
static void pauseAttribute();
static void time();
static void typing();

static int pausePending();
static int timePending();

///////////////////////
//       Image       //
///////////////////////

static void image();
static void imageAttributes();
static void imageAttribute();
static void source();

static int imagePending();
static int sourcePending();

///////////////////////
//      Option       //
///////////////////////

static void option();
static void optionAttributes();
static void optionAttribute();
static void preference();
static void options();
static void optionsObjList();
static void optionsObj();
static void optionsObjAttributes();
static void optionsObjAttribute();
static void label();
static void value();
static void inputDataObj();
static void inputAttribute();

static int optionPending();
static int preferencePending();
static int labelPending();

///////////////////////
// Connect to Agent  //
///////////////////////

static void connect_to_agent();
static void connectToAgentObj();
static void connectToAgentObjAttribute();

static int connect_to_agentPending();

/******************/
/*    Metadata    */
/******************/

static void metadata();
static void metadataObj();
static void optMetadataAttributes();
static void metadataAttributes();
static void metadataAttribute();
static void fallback();

static int metadataPending();
static int metadataAttributesPending();
static int metadataAttributePending();
static int fallbackPending();

/******************/
/*   Conditions   */
/******************/

static void conditions();

static int conditionsPending();

/******************/
/*  Dialog Node   */
/******************/

static void dialog_node();

static int dialog_nodePending();

/******************/
/*  Prev Sibling  */
/******************/

static void previous_sibling();

static int previous_siblingPending();

/******************/
/*   Description  */
/******************/

static void description();

static int descriptionPending();

/******************/
/*     Parent     */
/******************/

static void parent();

static int parentPending();

/******************/
/*     Context    */
/******************/

static void context();
static void contextObj();
static void contextObjAttributes();
static void contextObjAttribute();

static int contextPending();

/******************/
/*    Next Step   */
/******************/

static void next_step();
static void nextStepObj();
static void nextStepObjAttributes();
static void nextStepObjAttribute();
static void behavior();
static void selector();

static int next_stepPending();
static int behaviorPending();

/******************/
/*     Actions    */
/******************/

static void actions();
static void actionsObjList();
static void actionsObj();
static void actionsObjAttributes();
static void actionsObjAttribute();
static void action_type();
static void parameters();
static void parametersObj();
static void parametersAttributes();
static void parametersAttribute();
static void result_variable();
static void credentials();

static int actionsPending();
static int action_typePending();
static int parametersPending();
static int result_variablePending();

/******************/
/*   Event Name   */
/******************/

static void event_name();

static int event_namePending();

/******************/
/*    Variable    */
/******************/

static void variable();

static int variablePending();

/******************/
/*   Digress In   */
/******************/

static void digress_in();

static int digress_inPending();

/******************/
/*   Digress Out  */
/******************/

static void digress_out();

static int digress_outPending();

/******************/
/* Dgrs Out Slots */
/******************/

static void digress_out_slots();

static int digress_out_slotsPending();

/******************/
/*   User Label   */
/******************/

static void user_label();

/**************************************************************************/
/*                     Private Recognizing Functions                      */
/**************************************************************************/

static void advance() {
	current = lex();
}

static int check(char *type) {
	if (strcmp(type, getType(current)) == 0)
		return 1;
	return 0;
}

static void match(char *type) {
	if (check(type))
		advance();
	else {
		fprintf(stderr, "Error on line %d: Expected %s, got %s.\n", getLine(current), type, getType(current));
		printf("illegal\n");
		exit(2);
	}
}

/**************************************************************************/
/*                      Public Recognizing Functions                      */
/**************************************************************************/

void initRecognizer(char *filename) {
	initLexer(filename);
}

void closeRecognizer() {
	closeLexer();
}

void recognize() {
	current = lex();
	workspaces();
	match(ENDofINPUT);
	printf("legal\n");
}

/**************************************************************************/
/*                          Grammar Rule Methods                          */
/**************************************************************************/

/*************************************/
/*            Universals             */
/*************************************/

static void boolean() {
	if (check(TRUEVAL))
		advance();
	else if (check(FALSEVAL))
		advance();
}

/*************************************/
/*            Workspaces             */
/*************************************/

static void workspaces() {
	workspace();
	if (check(COMMA)) {
		advance();
		workspaces();
	}
}

static void workspace() {
	match(WORKSPACE);
	match(COLON);
	match(OBRACKET);
	workspaceObjs();
	match(CBRACKET);
}

static void workspaceObjs() {
	workspaceObj();
	if (check(COMMA)) {
		advance();
		workspaceObjs();
	}
}

static void workspaceObj() {
	match(OBRACE);
	workspaceAttributes();
	match(CBRACE);
}

static void workspaceAttributes() {
	workspaceAttribute();
	if (check(COMMA)) {
		advance();
		workspaceAttributes();
	}
}

static void workspaceAttribute() {
	if (namePending())
		name();
	else if (descriptionPending())
		description();
	else if (languagePending())
		language();
	else if (intentsBlockPending())
		intentsBlock();
	else if (entitiesBlockPending())
		entitiesBlock();
	else if (treePending())
		tree();
	else if (counterExamplesPending())
		counterExamples();
	else if (metadataPending())
		metadata();
	else if (learning_opt_outPending())
		learning_opt_out();
	else
		system_settings();
}

static void name() {
	match(NAME);
	match(COLON);
	match(STRING);
}

static void language() {
	match(LANGUAGE);
	match(COLON);
	match(STRING);
}

static void counterExamples() {
	match(COUNTEREXAMPLES);
	match(COLON);
	match(OBRACKET);
	counterExamplesList();
	match(CBRACKET);
}

static void counterExamplesList() {
	counterExample();
	if (check(COMMA)) {
		advance();
		counterExamplesList();
	}
}

static void counterExample() {
	match(TEXT);
	match(COLON);
	match(STRING);
}

static void learning_opt_out() {
	match(LEARNING_OPT_OUT);
	match(COLON);
	boolean();
}

static void system_settings() {
	match(SYSTEM_SETTINGS);
	match(COLON);
	systemSettingsObj();
}

static void systemSettingsObj() {
	match(OBRACE);
	systemSettingsAttributes();
	match(CBRACE);
}

static void systemSettingsAttributes() {
	systemSettingsAttribute();
	if (check(COMMA)) {
		advance();
		systemSettingsAttributes();
	}
}

static void systemSettingsAttribute() {
	if (toolingPending())
		tooling();
	else if (disambiguationPending())
		disambiguation();
	else
		human_agent_assist();
}

static void tooling() {
	match(TOOLING);
	match(COLON);
	toolingObj();
}

static void toolingObj() {
	match(OBRACE);
	storeGenericResponses();
	match(CBRACE);
}

static void storeGenericResponses() {
	match(STORE_GENERIC_RESPONSES);
	match(COLON);
	boolean();
}

static void disambiguation() {
	match(DISAMBIGUATION);
	match(COLON);
	match(OBRACE);
	disambiguationAttributes();
	match(CBRACE);
}

static void disambiguationAttributes() {
	disambiguationAttribute();
	if (check(COMMA)) {
		advance();
		disambiguationAttributes();
	}
}

static void disambiguationAttribute() {
	if (promptPending())
		prompt();
	else if (none_of_the_above_promptPending())
		none_of_the_above_prompt();
	else if (enabledPending())
		enabled();
	else
		sensitivity();
}

static void prompt() {
	match(PROMPT);
	match(COLON);
	match(STRING);
}

static void none_of_the_above_prompt() {
	match(NONE_OF_THE_ABOVE_PROMPT);
	match(COLON);
	match(STRING);
}

static void enabled() {
	match(ENABLED);
	match(COLON);
	boolean();
}

static void sensitivity() {
	match(SENSITIVITY);
	match(COLON);
	match(STRING);
}

static void human_agent_assist() {
	match(OBRACE);
	humanAgentAssistAttributes();
	match(CBRACE);
}

static void humanAgentAssistAttributes() {
	humanAgentAssistAttribute();
	if (check(COMMA)) {
		advance();
		humanAgentAssistAttributes();
	}
}

//TODO: Figure out a more comprehensive list of humanAgentAssist attributes.
static void humanAgentAssistAttribute() {
	return;
}

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

static void intentsBlock() {
	match(INTENTS);
	match(COLON);
	match(OBRACKET);
	intents();
	match(CBRACKET);
}

static void intents() {
	intent();
	if (check(COMMA)) {
		advance();
		intents();
	}
}

static void intent() {
	match(OBRACE);
	intentAttributes();
	match(CBRACE);
}

static void intentAttributes() {
	intentAttribute();
	if (check(COMMA)) {
		advance();
		intentAttributes();
	}
}

static void intentAttribute() {
	if (intentNamePending())
		intentName();
	else if (descriptionPending())
		description();
	else
		userExamples();
}

static void intentName() {
	match(INTENT);
	match(COLON);
	match(STRING);
}

static void userExamples() {
	userExample();
	if (check(COMMA)) {
		advance();
		userExamples();
	}
}

static void userExample() {
	match(EXAMPLES);
	match(COLON);
	match(OBRACKET);
	examples();
	match(CBRACKET);
}

static void examples() {
	example();
	if (check(COMMA)) {
		advance();
		examples();
	}
}

static void example() {
	match(OBRACE);
	exampleAttributes();
	match(CBRACE);
}

static void exampleAttributes() {
	exampleAttribute();
	if (check(COMMA)) {
		advance();
		exampleAttributes();
	}
}

static void exampleAttribute() {
	if (textPending())
		text();
	else
		mentions();
}

static void mentions() {
	match(MENTIONS);
	match(COLON);
	match(OBRACKET);
	mentionObjs();
	match(CBRACKET);
}

static void mentionObjs() {
	match(OBRACE);
	mentionObjsAttributes();
	match(CBRACE);
}

static void mentionObjsAttributes() {
	mentionObjsAttribute();
	if (check(COMMA)) {
		advance();
		mentionObjsAttributes();
	}
}

static void mentionObjsAttribute() {
	entityName();
}

static int intentsBlockPending() {
	return check(INTENTS);
}

static int intentNamePending() {
	return check(INTENT);
}

/*************************************/
/*             Entities              */
/*************************************/

static void entitiesBlock() {
	match(ENTITIES);
	match(COLON);
	match(OBRACKET);
	entities();
	match(CBRACKET);
}

static void entities() {
	entity();
	if (check(COMMA)) {
		advance();
		entities();
	}
}

static void entity() {
	match(OBRACE);
	entityAttributes();
	match(CBRACE);
}

static void entityAttributes() {
	entityAttribute();
	if (check(COMMA)) {
		advance();
		entityAttributes();
	}
}

static void entityAttribute() {
	if (entityNamePending())
		entityName();
	else if (descriptionPending())
		description();
	else if (metadataPending())
		metadata();
	else if (entityValuesPending())
		entityValues();
	else
		fuzzy_match();
}

static void entityName() {
	match(ENTITY);
	match(COLON);
	match(STRING);
}

static void entityValues() {
	match(VALUES);
	match(COLON);
	match(OBRACKET);
	entityValuesList();
	match(CBRACKET);
}

static void entityValuesList() {
	entityValue();
	if (check(COMMA)) {
		advance();
		entityValuesList();
	}
}

static void entityValue() {
	match(OBRACE);
	entityValueAttributes();
	match(CBRACE);
}

static void entityValueAttributes() {
	entityValueAttribute();
	if (check(COMMA)) {
		advance();
		entityValueAttributes();
	}
}

static void entityValueAttribute() {
	if (entityValueNamePending())
		entityValueName();
	else if (metadataPending())
		metadata();
	else if (synonymPending())
		synonyms();
	else if (patternsPending())
		patterns();
	else
		entityValueType();
}

static void entityValueName() {
	match(VALUE);
	match(COLON);
	match(STRING);
}

static void synonyms() {
	match(SYNONYMS);
	match(COLON);
	match(OBRACKET);
	synonymList();
	match(CBRACKET);
}

static void synonymList() {
	synonym();
	if (check(COMMA)) {
		advance();
		synonymList();
	}
}

static void synonym() {
	match(SYNONYM);
	match(COLON);
	match(STRING);
}

static void patterns() {
	match(PATTERNS);
	match(COLON);
	match(OBRACKET);
	patternList();
	match(CBRACKET);
}

static void patternList() {
	pattern();
	if (check(COMMA)) {
		advance();
		patternList();
	}
}

static void pattern() {
	match(PATTERN);
	match(COLON);
	match(STRING);
}

static void entityValueType() {
	match(VALUE_TYPE);
	match(COLON);
	match(STRING);
}

static void fuzzy_match() {
	match(FUZZY_MATCH);
	match(COLON);
	boolean();
}

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

static void tree() {
	match(TREE);
	match(COLON);
	match(OBRACKET);
	nodeList();
	match(CBRACKET);
}

static void nodeList() {
	node();
	if (check(COMMA)) {
		advance();
		nodeList();
	}
}

static void node() {
	match(OBRACE);
	nodeAttributes();
	match(CBRACE);
}

static void nodeAttributes() {
	nodeAttribute();
	if (check(COMMA)) {
		advance();
		nodeAttributes();
	}
}

static void nodeAttribute() {
	if (typePending())
		type();
	else if (titlePending())
		title();
	else if (childrenPending())
		children();
	else if (outputPending())
		output();
	else if (metadataPending())
		metadata();
	else if (conditionsPending())
		conditions();
	else if (dialog_nodePending())
		dialog_node();
	else if (previous_siblingPending())
		previous_sibling();
	else if (descriptionPending())
		description();
	else if (parentPending())
		parent();
	else if (contextPending())
		context();
	else if (next_stepPending())
		next_step();
	else if (actionsPending())
		actions();
	else if (event_namePending())
		event_name();
	else if (variablePending())
		variable();
	else if (digress_inPending())
		digress_in();
	else if (digress_outPending())
		digress_out();
	else if (digress_out_slotsPending())
		digress_out_slots();
	else
		user_label();
}

static int treePending() {
	return check(TREE);
}

/******************/
/*      Types     */
/******************/

static void type() {
	match(TYPE);
	match(COLON);
	match(STRING);
}

static int typePending() {
	return check(TYPE);
}

/******************/
/*      Title     */
/******************/

static void title() {
	match(TITLE);
	match(COLON);
	match(STRING);
}

static int titlePending() {
	return check(TITLE);
}

/******************/
/*    Children    */
/******************/

static void children() {
	match(CHILDREN);
	match(COLON);
	nodeList();
}

static int childrenPending() {
	return check(CHILDREN);
}

/******************/
/*     Output     */
/******************/

static void output() {
	match(OUTPUT);
	match(COLON);
	match(OBRACE);
	optOutputValues();
	match(CBRACE);
}

static void optOutputValues() {
	if (outputValuesPending())
		outputValues();
}

static void outputValues() {
	outputValue();
	if (check(COMMA)) {
		advance();
		outputValues();
	}
}

static void outputValue() {
	if (textPending())
		text();
	else if (pausePending())
		pause();
	else if (imagePending())
		image();
	else if (optionPending())
		option();
	else
		connect_to_agent();
}

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

static void text() {
	match(TEXT);
	match(COLON);
	textChoice();
}

static void textChoice() {
	if (check(STRING))
		match(STRING);
	else
		textValuesObj();
}

static void textValuesObj() {
	match(OBRACE);
	textValuesObjAttributes();
	match(CBRACE);
}

static void textValuesObjAttributes() {
	textValuesObjAttribute();
	if (check(COMMA)) {
		advance();
		textValuesObjAttributes();
	}
}

static void textValuesObjAttribute() {
	if (valuesPending())
		values();
	else if (selectionPolicyPending())
		selectionPolicy();
	else
		delimeter();
}

static void values() {
	match(VALUES);
	match(COLON);
	match(OBRACKET);
	stringList();
	match(CBRACKET);
}

static void stringList() {
	match(STRING);
	if (check(COMMA)) {
		advance();
		stringList();
	}
}

static void selectionPolicy() {
	match(SELECTION_POLICY);
	match(COLON);
	match(STRING);
}

static void delimeter() {
	match(DELIMETER);
	match(COLON);
	match(STRING);
}

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

static void pause() {
	match(PAUSE);
	match(COLON);
	match(OBRACE);
	pauseAttributes();
	match(CBRACE);
}

static void pauseAttributes() {
	pauseAttribute();
	if (check(COMMA)) {
		advance();
		pauseAttributes();
	}
}

static void pauseAttribute() {
	if (timePending())
		time();
	else
		typing();
}

static void time() {
	match(TIME);
	match(COLON);
	match(NUMBER);
}

static void typing() {
	match(TYPING);
	match(COLON);
	boolean();
}

static int pausePending() {
	return check(PAUSE);
}

static int timePending() {
	return check(TIME);
}

///////////////////////
//       Image       //
///////////////////////

static void image() {
	match(IMAGE);
	match(COLON);
	match(OBRACE);
	imageAttributes();
	match(CBRACE);
}

static void imageAttributes() {
	imageAttribute();
	if (check(COMMA)) {
		advance();
		imageAttributes();
	}
}

static void imageAttribute() {
	if (sourcePending())
		source();
	else if (titlePending())
		title();
	else
		description();
}

static void source() {
	match(SOURCE);
	match(COLON);
	match(STRING);
}

static int imagePending() {
	return check(IMAGE);
}

static int sourcePending() {
	return check(SOURCE);
}

///////////////////////
//      Option       //
///////////////////////

static void option() {
	match(OPTION);
	match(COLON);
	match(OBRACE);
	optionAttributes();
	match(CBRACE);
}

static void optionAttributes() {
	optionAttribute();
	if (check(COMMA)) {
		advance();
		optionAttributes();
	}
}

static void optionAttribute() {
	if (titlePending())
		title();
	else if (descriptionPending())
		description();
	else if (preferencePending())
		preference();
	else
		options();
}

static void preference() {
	match(PREFERENCE);
	match(COLON);
	match(STRING);
}

static void options() {
	match(OBRACKET);
	optionsObjList();
	match(CBRACKET);
}

static void optionsObjList() {
	optionsObj();
	if (check(COMMA)) {
		advance();
		optionsObjList();
	}
}

static void optionsObj() {
	match(OBRACE);
	optionsObjAttributes();
	match(CBRACE);
}

static void optionsObjAttributes() {
	optionsObjAttribute();
	if (check(COMMA)) {
		advance();
		optionsObjAttributes();
	}
}

static void optionsObjAttribute() {
	if (labelPending())
		label();
	else
		value();
}

static void label() {
	match(LABEL);
	match(COLON);
	match(STRING);
}

static void value() {
	match(VALUE);
	match(COLON);
	inputDataObj();
}

static void inputDataObj() {
	match(OBRACE);
	inputAttribute();
	match(CBRACE);
}

static void inputAttribute() {
	match(TEXT);
	match(COLON);
	match(STRING);
}

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

static void connect_to_agent() {
	match(CONNECT_TO_AGENT);
	match(COLON);
	connectToAgentObj();
}

static void connectToAgentObj() {
	match(OBRACE);
	connectToAgentObjAttribute();
	match(CBRACE);
}

static void connectToAgentObjAttribute() {
	match(MESSAGE_TO_HUMAN_AGENT);
	match(COLON);
	match(STRING);
}

static int connect_to_agentPending() {
	return check(CONNECT_TO_AGENT);
}

/******************/
/*    Metadata    */
/******************/

static void metadata() {
	match(METADATA);
	match(COLON);
	metadataObj();
}

static void metadataObj() {
	match(OBRACE);
	optMetadataAttributes();
	match(CBRACE);
}

static void optMetadataAttributes() {
	if (metadataAttributesPending())
		metadataAttributes();
}

static void metadataAttributes() {
	metadataAttribute();
	if (check(COMMA)) {
		advance();
		metadataAttributes();
	}
}

static void metadataAttribute() {
	if (fallbackPending())
		fallback();
}

static void fallback() {
	match(FALLBACK);
	match(COLON);
	match(STRING);
}

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

static void conditions() {
	match(CONDITIONS);
	match(COLON);
	match(STRING);
}

static int conditionsPending() {
	return check(CONDITIONS);
}

/******************/
/*  Dialog Node   */
/******************/

static void dialog_node() {
	match(DIALOG_NODE);
	match(COLON);
	match(STRING);
}

static int dialog_nodePending() {
	return check(DIALOG_NODE);
}

/******************/
/*  Prev Sibling  */
/******************/

static void previous_sibling() {
	match(PREVIOUS_SIBLING);
	match(COLON);
	match(STRING);
}

static int previous_siblingPending() {
	return check(PREVIOUS_SIBLING);
}

/******************/
/*   Description  */
/******************/

static void description() {
	match(DESCRIPTION);
	match(COLON);
	match(STRING);
}

static int descriptionPending() {
	return check(DESCRIPTION);
}

/******************/
/*     Parent     */
/******************/

static void parent() {
	match(PARENT);
	match(COLON);
	match(STRING);
}

static int parentPending() {
	return check(PARENT);
}

/******************/
/*     Context    */
/******************/

static void context() {
	match(CONTEXT);
	match(COLON);
	contextObj();
}

static void contextObj() {
	match(OBRACE);
	contextObjAttributes();
	match(CBRACE);
}

static void contextObjAttributes() {
	contextObjAttribute();
	if (check(COMMA)) {
		advance();
		contextObjAttributes();
	}
}

//TODO: Figure out a more comprehensive list of context attributes.
static void contextObjAttribute() {
	return;
}

static int contextPending() {
	return check(CONTEXT);
}

/******************/
/*    Next Step   */
/******************/

static void next_step() {
	match(NEXT_STEP);
	match(COLON);
	nextStepObj();
}

static void nextStepObj() {
	match(OBRACE);
	nextStepObjAttributes();
	match(CBRACE);
}

static void nextStepObjAttributes() {
	nextStepObjAttribute();
	if (check(COMMA)) {
		advance();
		nextStepObjAttributes();
	}
}

static void nextStepObjAttribute() {
	if (behaviorPending())
		behavior();
	else if (dialog_nodePending())
		dialog_node();
	else
		selector();
}

static void behavior() {
	match(BEHAVIOR);
	match(COLON);
	match(STRING);
}

static void selector() {
	match(SELECTOR);
	match(COLON);
	match(STRING);
}

static int next_stepPending() {
	return check(NEXT_STEP);
}

static int behaviorPending() {
	return check(BEHAVIOR);
}

/******************/
/*     Actions    */
/******************/

static void actions() {
	match(ACTIONS);
	match(COLON);
	match(OBRACKET);
	actionsObjList();
	match(CBRACKET);
}

static void actionsObjList() {
	actionsObj();
	if (check(COMMA)) {
		advance();
		actionsObjList();
	}
}

static void actionsObj() {
	match(OBRACE);
	actionsObjAttributes();
	match(CBRACE);
}

static void actionsObjAttributes() {
	actionsObjAttribute();
	if (check(COMMA)) {
		advance();
		actionsObjAttributes();
	}
}

static void actionsObjAttribute() {
	if (namePending())
		name();
	else if (action_typePending())
		action_type();
	else if (parametersPending())
		parameters();
	else if (result_variablePending())
		result_variable();
	else
		credentials();
}

static void action_type() {
	match(ACTION_TYPE);
	match(COLON);
	match(STRING);
}

static void parameters() {
	match(PARAMETERS);
	match(COLON);
	parametersObj();
}

static void parametersObj() {
	match(OBRACE);
	parametersAttributes();
	match(CBRACE);
}

static void parametersAttributes() {
	parametersAttribute();
	if (check(COMMA)) {
		advance();
		parametersAttributes();
	}
}

//TODO: Figure out a more comprehensive list of parameters attributes.
static void parametersAttribute() {
	return;
}

static void result_variable() {
	match(RESULT_VARIABLE);
	match(COLON);
	match(STRING);
}

static void credentials() {
	match(CREDENTIALS);
	match(COLON);
	match(STRING);
}

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

static void event_name() {
	match(EVENT_NAME);
	match(COLON);
	match(STRING);
}

static int event_namePending() {
	return check(EVENT_NAME);
}

/******************/
/*    Variable    */
/******************/

static void variable() {
	match(VARIABLE);
	match(COLON);
	match(STRING);
}

static int variablePending() {
	return check(VARIABLE);
}

/******************/
/*   Digress In   */
/******************/

static void digress_in() {
	match(DIGRESS_IN);
	match(COLON);
	match(STRING);
}

static int digress_inPending() {
	return check(DIGRESS_IN);
}

/******************/
/*   Digress Out  */
/******************/

static void digress_out() {
	match(DIGRESS_OUT);
	match(COLON);
	match(STRING);
}

static int digress_outPending() {
	return check(DIGRESS_OUT);
}

/******************/
/* Dgrs Out Slots */
/******************/

static void digress_out_slots() {
	match(DIGRESS_OUT_SLOTS);
	match(COLON);
	match(STRING);
}

static int digress_out_slotsPending() {
	return check(DIGRESS_OUT_SLOTS);
}

/******************/
/*   User Label   */
/******************/

static void user_label() {
	match(USER_LABEL);
	match(COLON);
	match(STRING);
}
