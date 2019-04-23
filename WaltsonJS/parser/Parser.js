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
	IMPLICIT: 'IMPLICIT'
	
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
		let ch = this.readChar()		// Gets first "
		let ret = ""
		while ((ch = this.readChar()) != '"') {
			ret += ch
		}
		// Throw away the terminating quote by not pushing back

		return new Lexeme(Type.STRING, ret, this.line, undefined, undefined)
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
			case '#':
				return new Lexeme(Type.HASH, undefined, this.line, undefined, undefined)
			case '@':
				return new Lexeme(Type.AT, undefined, this.line, undefined, undefined)
			case '$':
				return new Lexeme(Type.DOLLAR, undefined, this.line, undefined, undefined)
			case '-':
				return new Lexeme(Type.MINUS, undefined, this.line, undefined, undefined)
			case '"':
				this.putBack(ch)
				return this.lexString()
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
					throw Error('Error on line ' + this.line + ': could not understand character ${ch}!')
				}
		}
	}
}

var Parser = function(filename) {
	this.lexer = new Lexer(filename)
	this.current = undefined
}

Parser.prototype = {
	parse: function() {
		this.current = this.lexer.lex()
		let tree = this.program()
		this.match(Type.END_OF_INPUT)
		return tree
	},
	match: function(expected) {
		if (this.check(expected))
			return this.advance()
		console.error('Error on line ', this.current.getLine(), ': expected', expected, ', got ', this.current.getType())
		throw Error('illegal')
	},
	check: function(expected) {
		if (expected == this.current.getType())
			return true
		return false
	},
	advance: function() {
		let temp = this.current
		this.current = this.lexer.lex()
		return temp
	},
	program: function() {
		let a, b
		a = this.definition()
		if (this.definitionPending())
			b = this.program()
		else
			b = null
		return Lexeme.prototype.cons(Type.PROGRAM, a, b)
	},
	definition: function() {
		if (this.explicitDefPending())
			return this.explicitDef()
		else
			return this.implicitDef()
	},
	explicitDef: function() {
		this.match(Type.ID)
		this.match(Type.COLON)
		return Lexeme.prototype.cons(Type.EXPLICIT, this.unary(), null)
	},
	implicitDef: function() {
		return Lexeme.prototype.cons(Type.IMPLICIT, this.unary(), null)
	},
	unary: function() {
		if (this.check(Type.ID) ||
			this.check(Type.TRUE) ||
			this.check(Type.FALSE) ||
			this.check(Type.STRING) ||
			this.check(Type.SKIP))
				return this.advance()
		else if (this.numberPending())
			return this.number()
		else if (this.boolExprPending())
			return this.boolExpr()
		else if (this.objectPending())
			return this.object()
		else
			return this.array()
	},
	number: function() {
		if (this.nonNegNumPending())
			return this.advance()
		else
			return this.negNum()
	},
	negNum: function() {
		this.match(Type.MINUS)
		return Lexeme.prototype.cons(Type.MINUS, null, this.match(Type.NUMBER))
	},
	boolExpr: function() {
		let a, b
		if (this.check(Type.OPEN_PAREN)) {
			this.advance()
			a = this.boolExpr()
			this.match(Type.CLOSE_PAREN)
			return Lexeme.prototype.cons(Type.OPEN_PAREN, null, a)
		}
		a = this.boolUnary()
		if (this.boolOpPending()) {
			b = this.boolOp()
			return Lexeme.prototype.cons(b.getType(), a, this.boolExpr())
		}
		return a
	},
	boolUnary: function() {
		if (this.intentPending())
			return this.intent()
		else if (this.entityPending())
			return this.entity()
		else
			return this.context()
	},
	boolOp: function() {
		if (this.check(Type.AND)) {
			this.advance()
			return this.match(Type.AND)
		}
		else {
			this.advance()
			return this.match(Type.OR)
		}
	},
	intent: function() {
		this.match(Type.HASH)
		return Lexeme.prototype.cons(Type.HASH, null, this.match(Type.ID))
	},
	entity: function() {
		this.match(Type.AT)
		return Lexeme.prototype.cons(Type.AT, null, this.match(Type.ID))
	},
	context: function() {
		this.match(Type.DOLLAR)
		return Lexeme.prototype.cons(Type.DOLLAR, null, this.match(Type.ID))
	},
	object: function() {
		let a
		this.match(Type.OPEN_BRACE)
		a = this.optAttrList()
		this.match(Type.CLOSE_BRACE)
		return a
	},
	optAttrList: function() {
		if (this.attrListPending())
			return this.attrList()
		return null
	},
	attrList: function() {
		let a, b
		a = this.definition()
		if (this.check(Type.COMMA)) {
			this.advance()
			b = this.attrList()
		}
		else
			b = null
		return Lexeme.prototype.cons(Type.ATTRIBUTE_LIST, a, b)
	},
	array: function() {
		let a
		this.match(Type.OPEN_BRACKET)
		a = this.mixinList()
		this.match(Type.CLOSE_BRACKET)
		return a
	},
	mixinList: function() {
		let a, b
		a = this.mixin()
		if (this.check(Type.COMMA)) {
			this.advance()
			b = this.mixinList()
		}
		else
			b = null
		return Lexeme.prototype.cons(Type.MIXIN_LIST, a, b)
	},
	mixin: function() {
		if (this.definitionPending())
			return this.definition()
		else
			return this.object()
	},
	definitionPending: function() {
		return (this.explicitDefPending() ||
				this.implicitDefPending())
	},
	explicitDefPending: function() {
		return this.check(Type.ID)
	},
	implicitDefPending: function() {
		return this.unaryPending()
	},
	unaryPending: function() {
		return (this.check(Type.ID) ||
				this.check(Type.TRUE) || 
				this.check(Type.FALSE) ||
				this.check(Type.STRING) ||
				this.check(Type.SKIP) ||
				this.numberPending() ||
				this.boolExprPending() ||
				this.objectPending() ||
				this.arrayPending())
	},
	numberPending: function() {
		return (this.nonNegNumPending() ||
				this.negNumPending())
	},
	nonNegNumPending: function() {
		return this.check(Type.NUMBER)
	},
	negNumPending: function() {
		return this.check(Type.MINUS)
	},
	objectPending: function() {
		return this.check(Type.OPEN_BRACE)
	},
	attrListPending: function() {
		return this.definitionPending()
	}
}

module.exports = {
	Type: Type,
	Lexeme: Lexeme,
	Lexer: Lexer,
	Parser: Parser
}
