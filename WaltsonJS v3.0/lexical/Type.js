export default class Type {

	constructor() {
		return this
	}
	
	static OPEN_BRACKET = 'OPEN_BRACKET'
	static CLOSE_BRACKET = 'CLOSE_BRACKET'
	static OPEN_BRACE = 'OPEN_BRACE'
	static CLOSE_BRACE = 'CLOSE_BRACE'
	static COLON = 'COLON'
	static COMMA = 'COMMA'
	static MINUS = 'MINUS'
	static STRING = 'STRING'
	static NUMBER = 'NUMBER'
	static ID = 'ID'
	static TRUE = 'true'
	static FALSE = 'false'

	static ARRAY = 'ARRAY'
	static OBJECT = 'OBJECT'
	static PROGRAM = 'PROGRAM'
	static ATTRIBUTE_LIST = 'ATTRIBUTE_LIST'
	static MIXIN_LIST = 'MIXIN_LIST'

	static ENVIRONMENT = 'ENVIRONMENT'
	static TABLE = 'TABLE'
	static ID_LIST = 'ID_LIST'
	static VALUE_LIST = 'VALUE_LIST'

	static END_OF_INPUT = 'END_OF_INPUT'
	
}