var Type = {
	
	OPEN_PAREN: 'OPEN_PAREN',
	CLOSE_PAREN: 'CLOSE_PAREN',
	OPEN_BRACKET: 'OPEN_BRACKET',
	CLOSE_BRACKET: 'CLOSE_BRACKET',
	OPEN_BRACE: 'OPEN_BRACE',
	CLOSE_BRACE: 'CLOSE_BRACE',
	COLON: 'COLON',
	COMMA: 'COMMA',
	MINUS: 'MINUS',
	STRING: 'STRING',
	SKIP: 'SKIP',
	URL: 'URL',
	HASH: 'HASH',
	AT: 'AT',
	DOLLAR: 'DOLLAR',
	NUMBER: 'NUMBER',
	AND: 'AND',
	OR: 'OR',
	ID: 'ID',
	TRUE: 'true',
	FALSE: 'false',

	ARRAY: 'ARRAY',
	OBJECT: 'OBJECT',
	PROGRAM: 'PROGRAM',
	ATTRIBUTE_LIST: 'ATTRIBUTE_LIST',
	MIXIN_LIST: 'MIXIN_LIST',

	ENVIRONMENT: 'ENVIRONMENT',
	TABLE: 'TABLE',
	ID_LIST: 'ID_LIST',
	VALUE_LIST: 'VALUE_LIST',

	END_OF_INPUT: 'END_OF_INPUT',

	EXPLICIT: 'EXPLICIT',
	IMPLICIT: 'IMPLICIT',
	EXPRESSION: 'EXPRESSION'
	
}

var Lexeme = function(type, val, line, left, right) {
	this.type = type
	this.value = val
	this.line = line
	this.left = left
	this.right = right
}

Lexeme.prototype = {
	getType: function() {
		return this.type
	},

	getValue: function() {
		return this.value
	},

	getLine: function() {
		return this.line
	},

	car: function() {
		return this.left
	},

	cdr: function() {
		return this.right
	},

	setType: function(type) {
		let ret = this.type
		this.type = type
		return ret
	},

	setValue: function(value) {
		let ret = this.value
		this.value = value
		return ret
	},

	setLine: function(line) {
		let ret = this.line
		this.line = line
		return ret
	},

	setCar: function(left) {
		let ret = this.left
		this.left = left
		return ret
	},

	setCdr: function(right) {
		let ret = this.right
		this.right = right
		return ret
	},

	cons: function(type, left, right) {
		return new Lexeme(type, undefined, undefined, left, right)
	}

}

let fs = require('fs')

var Lexer = function(filename) {
	this.file = fs.readFileSync(filename, 'utf-8')
	this.position = 0
	this.line = 1
}

Lexer.prototype = {

	readChar: function() {
		let ch = this.file[this.position]
		if (ch == '\n')
			this.line++
		this.position++
		return ch
	},

	putBack: function(ch) {
		if (ch == '\n')
			this.line--
		this.position--
	},

	skipWhiteSpace: function() {
		let ch = this.readChar()
		while(/\s/.test(ch)) {
			ch = this.readChar()
		}
		this.putBack(ch)
	},

	lexString: function() {
		let ch
		let last
		let ret = ""
		while ((ch = this.readChar()) != '"' || (ch == '"'  && last == '\\')) {
			ret += ch
			last = ch
		}
		// Throw away the terminating quote by not pushing back

		// If we didn't read anything, throw an error.
		if (ret.length == 0)
			throw Error('Error on line ' + this.line + ': a string must not be empty!')

		return new Lexeme(Type.STRING, ret, this.line, undefined, undefined)
	},

	lexURL: function() {
		let ch
		let ret = ""
		while ((ch = this.readChar()) != '>') {
			ret += ch
		}
		// Throw away the terminating angle bracket by not pushing back

		// If we didn't read anything, throw an error.
		if (ret.length == 0)
			throw Error('Error on line ' + this.line + ': a URL must not be empty!')

		return new Lexeme(Type.URL, ret, this.line, undefined, undefined)
	},

	lexNumber: function() {
		let ch = this.readChar()
		let real = 0
		let ret = ""
		while (/\d/.test(ch) || ch == '.') {
			if (real && ch == '.') {
				throw Error("Error on line " + this.line + ": number with two decimals!")
			}
			if (ch == '.') {
				real = 1
			}
			ret += ch
			ch = this.readChar()
		}
		this.putBack(ch)
		return new Lexeme(Type.NUMBER, (real == 1) ? parseFloat(ret) : parseInt(ret), this.line, undefined, undefined)
	},

	lexVar: function() {
		let ch = this.readChar()
		let ret = ""
		let size = 64
		let index = 0
		while (/\w/.test(ch) || ch == '_') {
			if (index == size)
				throw Error("Error on line " + this.line + ": variable name \"" + ret + "\" is too long!")
			ret += ch
			index++
			ch = this.readChar()
		}
		this.putBack(ch)
		return new Lexeme(Type.ID, ret, this.line, undefined, undefined)
	},

	lexIntent: function() {
		let ret = ""
		let ch = this.readChar()

		// We only want alphanumeric, underscore, hyphen, and dot characters
		while (/[a-zA-Z0-9\_\-\.]/.test(ch)) {
			ret += ch
			ch = this.readChar()
		}

		// If we didn't read anything, throw an error.
		if (ret.length == 0)
			throw Error('Error on line ' + this.line + ': An intent must have a name!')

		// A white-space character broke us out of the loop, so put it back.
		this.putBack(ch)

		return new Lexeme(Type.HASH, ret, this.line, undefined, undefined)
	},

	lexEntity: function() {
		let ret = ""
		let ch = this.readChar()

		// We only want alphanumeric, underscore, hyphen, and dot characters
		while (/[a-zA-Z0-9\_\-\.]/.test(ch)) {
			ret += ch
			ch = this.readChar()
		}

		// If we didn't read anything, throw an error.
		if (ret.length == 0)
			throw Error('Error on line ' + this.line + ': An entity must have a name!')

		// A white-space character broke us out of the loop, so put it back.
		this.putBack(ch)

		return new Lexeme(Type.AT, ret, this.line, undefined, undefined)
	},

	lexContext: function() {
		let ret = ""
		let ch = this.readChar()

		// We only want alphanumeric, underscore, hyphen, and dot characters
		while (/[a-zA-Z0-9\_\-\.]/.test(ch)) {
			ret += ch
			ch = this.readChar()
		}

		// If we didn't read anything, throw an error.
		if (ret.length == 0)
			throw Error('Error on line ' + this.line + ': A context variable must have a name!')

		// A white-space character broke us out of the loop, so put it back.
		this.putBack(ch)

		return new Lexeme(Type.DOLLAR, ret, this.line, undefined, undefined)
	},

	lex: function() {
		this.skipWhiteSpace()
		let ch = this.readChar()
		if (ch == undefined)
			return new Lexeme(Type.END_OF_INPUT, undefined, this.line, undefined, undefined)
		switch (ch) {
			case '(':
				return new Lexeme(Type.OPEN_PAREN, undefined, this.line, undefined, undefined)
			case ')':
				return new Lexeme(Type.CLOSE_PAREN, undefined, this.line, undefined, undefined)
			case '[':
				return new Lexeme(Type.OPEN_BRACKET, undefined, this.line, undefined, undefined)
			case ']':
				return new Lexeme(Type.CLOSE_BRACKET, undefined, this.line, undefined, undefined)
			case '{':
				return new Lexeme(Type.OPEN_BRACE, undefined, this.line, undefined, undefined)
			case '}':
				return new Lexeme(Type.CLOSE_BRACE, undefined, this.line, undefined, undefined)
			case ':':
				return new Lexeme(Type.COLON, undefined, this.line, undefined, undefined)
			case ',':
				return new Lexeme(Type.COMMA, undefined, this.line, undefined, undefined)
			case '*':
				return new Lexeme(Type.SKIP, undefined, this.line, undefined, undefined)
			case '&':
				return new Lexeme(Type.AND, undefined, this.line, undefined, undefined)
			case '|':
				return new Lexeme(Type.OR, undefined, this.line, undefined, undefined)
			case '-':
				return new Lexeme(Type.MINUS, undefined, this.line, undefined, undefined)
			case '"': return this.lexString()
			case '<': return this.lexURL()
			case '#': return this.lexIntent()
			case '@': return this.lexEntity()
			case '$': return this.lexContext()
			default:
				if (/\d/.test(ch)) {
					this.putBack(ch)
					return this.lexNumber()
				}
				else if (/\w/.test(ch)) {
					this.putBack(ch)
					return this.lexVar()
				}
				else {
					throw Error('Error on line ' + this.line + ': could not understand character ' + ch + '!')
				}
		}
	}

}

module.exports = {
	Type: Type,
	Lexeme: Lexeme,
	Lexer: Lexer
}
