/**
 * Recognizer module for checking syntax errors.
 * 
 * Created by Walt Loumakis.
 */

#include <stdio.h>
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

static int booleanPending();

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

static int workspacesPending();
static int workspacePending();
static int workspaceObjsPending();
static int workspaceObjPending();
static int workspaceAttributesPending();
static int workspaceAttributePending();
static int namePending();
static int languagePending();
static int learning_opt_outPending();
static int system_settingsPending();
static int systemSettingsObjPending();
static int systemSettingsAttributesPending();
static int systemSettingsAttributePending();
static int toolingPending();
static int toolingObjPending();
static int storeGenericResponsesPending();
static int disambiguationPending();
static int disambiguationAttributesPending();
static int disambiguationAttributePending();
static int promptPending();
static int none_of_the_above_promptPending();
static int enabledPending();
static int sensitivityPending();
static int human_agent_assistPending();
static int humanAgentAssistAttributesPending();
static int humanAgentAssistAttributePending();

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
static void examples();
static void example();
static void exampleAttributes();
static void exampleAttribute();
static void mentions();
static void mentionObjs();
static void mentionObjsAttributes();
static void mentionObjsAttribute();

static int intentsBlockPending();
static int intentsPending();
static int intentPending();
static int intentAttributesPending();
static int intentAttributePending();
static int intentNamePending();
static int userExamplesPending();
static int examplesPending();
static int examplePending();
static int exampleAttributesPending();
static int exampleAttributePending();
static int mentionsPending();
static int mentionObjsPending();
static int mentionObjsAttributesPending();
static int mentionObjsAttributePending();

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

static int entitiesBlockPending();
static int entitiesPending();
static int entityPending();
static int entityAttributesPending();
static int entityAttributePending();
static int entityNamePending();
static int entityValuesPending();
static int entityValuesListPending();
static int entityValuePending();
static int entityValueAttributesPending();
static int entityValueAttributePending();
static int entityValueNamePending();
static int synonymsPending();
static int synonymListPending();
static int synonymPending();
static int patternsPending();
static int patternListPending();
static int patternPending();
static int entityValueTypePending();

/*************************************/
/*           Decision Tree           */
/*************************************/

static void tree();
static void nodeList();
static void nodeAttributes();
static void nodeAttribute();

static int treePending();
static int nodeListPending();
static int nodeAttributesPending();
static int nodeAttributePending();

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
/*     Output     */
/******************/

static void output();
static void optOutputValues();
static void outputValues();
static void outputValue();

static int outputPending();
static int optOutputValuesPending();
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
static int textChoicePending();
static int textValuesObjPending();
static int textValuesObjAttributesPending();
static int textValuesObjAttributePending();
static int valuesPending();
static int stringListPending();
static int selectionPolicyPending();
static int delimeterPending();

///////////////////////
//       Pause       //
///////////////////////

static void pause();
static void pauseAttributes();
static void pauseAttribute();
static void time();
static void typing();

static int pausePending();
static int pauseAttributesPending();
static int pauseAttributePending();
static int timePending();
static int typingPending();

///////////////////////
//       Image       //
///////////////////////

static void image();
static void imageAttributes();
static void imageAttribute();
static void source();

static int imagePending();
static int imageAttributesPending();
static int imageAttributePending();
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
static int optionAttributesPending();
static int optionAttributePending();
static int preferencePending();
static int optionsPending();
static int optionsObjListPending();
static int optionsObjPending();
static int optionsObjAttributesPending();
static int optionsObjAttributePending();
static int labelPending();
static int valuePending();
static int inputDataObjPending();
static int inputAttributePending();

///////////////////////
// Connect to Agent  //
///////////////////////

static void connect_to_agent();
static void connectToAgentObj();
static void connectToAgentObjAttribute();

static int connect_to_agentPending();
static int connectToAgentObjPending();
static int connectToAgentObjAttributePending();

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
static int metadataObjPending();
static int optMetadataAttributesPending();
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
static int contextObjPending();
static int contextObjAttributesPending();
static int contextObjAttributePending();

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
static int nextStepObjPending();
static int nextStepObjAttributesPending();
static int nextStepObjAttributePending();
static int behaviorPending();
static int selectorPending();

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
static int actionsObjListPending();
static int actionsObjPending();
static int actionsObjAttributesPending();
static int actionsObjAttributePending();
static int action_typePending();
static int parametersPending();
static int parametersObjPending();
static int parametersAttributesPending();
static int parametersAttributePending();
static int result_variablePending();
static int credentialsPending();

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

static int user_labelPending();

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

static int booleanPending() {
	return (check(TRUEVAL) ||
			check(FALSEVAL));
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
	//TODO: else if (counterExamplesPending)
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

static int workspacesPending() {
	return workspacePending();
}

static int workspacePending() {
	return check(WORKSPACE);
}

static int workspaceObjsPending() {
	return workspaceObjPending();
}

static int workspaceObjPending() {
	return check(OBRACE);
}

static int workspaceAttributesPending() {
	return workspaceAttributePending();
}

static int workspaceAttributePending() {
	return (namePending() ||
			descriptionPending() ||
			languagePending() ||
			intentsBlockPending() ||
			entitiesBlockPending() ||
			treePending() ||
//TODO:			counterExamplesPending() ||
			metadataPending() ||
			learning_opt_outPending() ||
			system_settingsPending());
}

static int namePending() {
	return check(NAME);
}

static int languagePending() {
	return check(LANGUAGE);
}

static int learning_opt_outPending() {
	return check(LEARNING_OPT_OUT);
}

static int system_settingsPending() {
	return check(SYSTEM_SETTINGS);
}

static int systemSettingsObjPending() {
	return check(OBRACE);
}

static int systemSettingsAttributesPending() {
	return systemSettingsAttributePending();
}

static int systemSettingsAttributePending() {
	return (toolingPending() ||
			disambiguationPending() ||
			human_agent_assistPending());
}

static int toolingPending() {
	return check(TOOLING);
}

static int toolingObjPending() {
	return check(OBRACE);
}

static int storeGenericResponsesPending() {
	return check(STORE_GENERIC_RESPONSES);
}

static int disambiguationPending() {
	return check(DISAMBIGUATION);
}

static int disambiguationAttributesPending() {
	return disambiguationAttributePending();
}

static int disambiguationAttributePending() {
	return (promptPending() ||
			none_of_the_above_promptPending() ||
			enabledPending() ||
			sensitivityPending());
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

static int sensitivityPending() {
	return check(SENSITIVITY);
}

static int human_agent_assistPending() {
	return check(HUMAN_AGENT_ASSIST);
}

static int humanAgentAssistAttributesPending() {
	return humanAgentAssistAttributePending();
}

//TODO: Figure out a more comprehensive list of humaAgentAssist attributes.
static int humanAgentAssistAttributePending() {
	return 0;
}


/*************************************/
/*              Intents              */
/*************************************/

static void intentsBlock() {
	
}

static void intents() {

}

static void intent() {

}

static void intentAttributes() {

}

static void intentAttribute() {

}

static void intentName() {

}

static void userExamples() {

}

static void examples() {

}

static void example() {

}

static void exampleAttributes() {

}

static void exampleAttribute() {

}

static void mentions() {

}

static void mentionObjs() {

}

static void mentionObjsAttributes() {

}

static void mentionObjsAttribute() {

}


static int intentsBlockPending() {

}

static int intentsPending() {

}

static int intentPending() {

}

static int intentAttributesPending() {

}

static int intentAttributePending() {

}

static int intentNamePending() {

}

static int userExamplesPending() {

}

static int examplesPending() {

}

static int examplePending() {

}

static int exampleAttributesPending() {

}

static int exampleAttributePending() {

}

static int mentionsPending() {

}

static int mentionObjsPending() {

}

static int mentionObjsAttributesPending() {

}

static int mentionObjsAttributePending() {

}


/*************************************/
/*             Entities              */
/*************************************/

static void entitiesBlock() {

}

static void entities() {

}

static void entity() {

}

static void entityAttributes() {

}

static void entityAttribute() {

}

static void entityName() {

}

static void entityValues() {

}

static void entityValuesList() {

}

static void entityValue() {

}

static void entityValueAttributes() {

}

static void entityValueAttribute() {

}

static void entityValueName() {

}

static void synonyms() {

}

static void synonymList() {

}

static void synonym() {

}

static void patterns() {

}

static void patternList() {

}

static void pattern() {

}

static void entityValueType() {

}


static int entitiesBlockPending() {

}

static int entitiesPending() {

}

static int entityPending() {

}

static int entityAttributesPending() {

}

static int entityAttributePending() {

}

static int entityNamePending() {

}

static int entityValuesPending() {

}

static int entityValuesListPending() {

}

static int entityValuePending() {

}

static int entityValueAttributesPending() {

}

static int entityValueAttributePending() {

}

static int entityValueNamePending() {

}

static int synonymsPending() {

}

static int synonymListPending() {

}

static int synonymPending() {

}

static int patternsPending() {

}

static int patternListPending() {

}

static int patternPending() {

}

static int entityValueTypePending() {

}


/*************************************/
/*           Decision Tree           */
/*************************************/

static void tree() {

}

static void nodeList() {

}

static void nodeAttributes() {

}

static void nodeAttribute() {

}


static int treePending() {

}

static int nodeListPending() {

}

static int nodeAttributesPending() {

}

static int nodeAttributePending() {

}


/******************/
/*      Types     */
/******************/

static void type() {

}


static int typePending() {

}


/******************/
/*      Title     */
/******************/

static void title() {

}


static int titlePending() {

}


/******************/
/*     Output     */
/******************/

static void output() {

}

static void optOutputValues() {

}

static void outputValues() {

}

static void outputValue() {

}


static int outputPending() {

}

static int optOutputValuesPending() {

}

static int outputValuesPending() {

}

static int outputValuePending() {

}


///////////////////////
//       Text        //
///////////////////////

static void text() {

}

static void textChoice() {

}

static void textValuesObj() {

}

static void textValuesObjAttributes() {

}

static void textValuesObjAttribute() {

}

static void values() {

}

static void stringList() {

}

static void selectionPolicy() {

}

static void delimeter() {

}


static int textPending() {

}

static int textChoicePending() {

}

static int textValuesObjPending() {

}

static int textValuesObjAttributesPending() {

}

static int textValuesObjAttributePending() {

}

static int valuesPending() {

}

static int stringListPending() {

}

static int selectionPolicyPending() {

}

static int delimeterPending() {

}


///////////////////////
//       Pause       //
///////////////////////

static void pause() {

}

static void pauseAttributes() {

}

static void pauseAttribute() {

}

static void time() {

}

static void typing() {

}


static int pausePending() {

}

static int pauseAttributesPending() {

}

static int pauseAttributePending() {

}

static int timePending() {

}

static int typingPending() {

}


///////////////////////
//       Image       //
///////////////////////

static void image() {

}

static void imageAttributes() {

}

static void imageAttribute() {

}

static void source() {

}


static int imagePending() {

}

static int imageAttributesPending() {

}

static int imageAttributePending() {

}

static int sourcePending() {

}


///////////////////////
//      Option       //
///////////////////////

static void option() {

}

static void optionAttributes() {

}

static void optionAttribute() {

}

static void preference() {

}

static void options() {

}

static void optionsObjList() {

}

static void optionsObj() {

}

static void optionsObjAttributes() {

}

static void optionsObjAttribute() {

}

static void label() {

}

static void value() {

}

static void inputDataObj() {

}

static void inputAttribute() {

}


static int optionPending() {

}

static int optionAttributesPending() {

}

static int optionAttributePending() {

}

static int preferencePending() {

}

static int optionsPending() {

}

static int optionsObjListPending() {

}

static int optionsObjPending() {

}

static int optionsObjAttributesPending() {

}

static int optionsObjAttributePending() {

}

static int labelPending() {

}

static int valuePending() {

}

static int inputDataObjPending() {

}

static int inputAttributePending() {

}


///////////////////////
// Connect to Agent  //
///////////////////////

static void connect_to_agent() {

}

static void connectToAgentObj() {

}

static void connectToAgentObjAttribute() {

}


static int connect_to_agentPending() {

}

static int connectToAgentObjPending() {

}

static int connectToAgentObjAttributePending() {

}


/******************/
/*    Metadata    */
/******************/

static void metadata() {

}

static void metadataObj() {

}

static void optMetadataAttributes() {

}

static void metadataAttributes() {

}

static void metadataAttribute() {

}

static void fallback() {

}


static int metadataPending() {

}

static int metadataObjPending() {

}

static int optMetadataAttributesPending() {

}

static int metadataAttributesPending() {

}

static int metadataAttributePending() {

}

static int fallbackPending() {

}


/******************/
/*   Conditions   */
/******************/

static void conditions() {

}


static int conditionsPending() {

}


/******************/
/*  Dialog Node   */
/******************/

static void dialog_node() {

}


static int dialog_nodePending() {

}


/******************/
/*  Prev Sibling  */
/******************/

static void previous_sibling() {

}


static int previous_siblingPending() {

}


/******************/
/*   Description  */
/******************/

static void description() {

}


static int descriptionPending() {

}


/******************/
/*     Parent     */
/******************/

static void parent() {

}


static int parentPending() {

}


/******************/
/*     Context    */
/******************/

static void context() {

}

static void contextObj() {

}

static void contextObjAttributes() {

}

static void contextObjAttribute() {

}


static int contextPending() {

}

static int contextObjPending() {

}

static int contextObjAttributesPending() {

}

static int contextObjAttributePending() {

}


/******************/
/*    Next Step   */
/******************/

static void next_step() {

}

static void nextStepObj() {

}

static void nextStepObjAttributes() {

}

static void nextStepObjAttribute() {

}

static void behavior() {

}

static void selector() {

}


static int next_stepPending() {

}

static int nextStepObjPending() {

}

static int nextStepObjAttributesPending() {

}

static int nextStepObjAttributePending() {

}

static int behaviorPending() {

}

static int selectorPending() {

}


/******************/
/*     Actions    */
/******************/

static void actions() {

}

static void actionsObjList() {

}

static void actionsObj() {

}

static void actionsObjAttributes() {

}

static void actionsObjAttribute() {

}

static void action_type() {

}

static void parameters() {

}

static void parametersObj() {

}

static void parametersAttributes() {

}

static void parametersAttribute() {

}

static void result_variable() {

}

static void credentials() {

}


static int actionsPending() {

}

static int actionsObjListPending() {

}

static int actionsObjPending() {

}

static int actionsObjAttributesPending() {

}

static int actionsObjAttributePending() {

}

static int action_typePending() {

}

static int parametersPending() {

}

static int parametersObjPending() {

}

static int parametersAttributesPending() {

}

static int parametersAttributePending() {

}

static int result_variablePending() {

}

static int credentialsPending() {

}


/******************/
/*   Event Name   */
/******************/

static void event_name() {

}


static int event_namePending() {

}


/******************/
/*    Variable    */
/******************/

static void variable() {

}


static int variablePending() {

}


/******************/
/*   Digress In   */
/******************/

static void digress_in() {

}


static int digress_inPending() {

}


/******************/
/*   Digress Out  */
/******************/

static void digress_out() {

}


static int digress_outPending() {

}


/******************/
/* Dgrs Out Slots */
/******************/

static void digress_out_slots() {

}


static int digress_out_slotsPending() {

}


/******************/
/*   User Label   */
/******************/

static void user_label() {

}


static int user_labelPending() {

}

