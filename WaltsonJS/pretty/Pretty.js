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
		console.error('Error on line ' + this.current.getLine() + ': expected ' + expected + ', got ' + this.current.getType())
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
		if (this.check(Type.COMMA))
			this.advance()
		if (this.definitionPending())
			b = this.program()
		else
			b = null
		return Lexeme.prototype.cons(Type.PROGRAM, a, b)
	},

	definition: function() {
		if (this.check(Type.ID))
			return this.explicitDef()
		else if (this.check(Type.STRING)) {
			let a = this.advance()
			if (this.check(Type.COLON)) {
				this.advance()
				let x = new Lexeme(Type.ID, a.getValue(), a.getLine(), a.car(), a.cdr())
				return Lexeme.prototype.cons(Type.EXPLICIT, x, this.unary())
			}
			return Lexeme.prototype.cons(Type.IMPLICIT, a, null)
		}
		else
			return this.implicitDef()
	},

	explicitDef: function() {
		let a = this.match(Type.ID)
		this.match(Type.COLON)
		return Lexeme.prototype.cons(Type.EXPLICIT, a, this.unary())
	},

	implicitDef: function() {
		return Lexeme.prototype.cons(Type.IMPLICIT, this.unary(), null)
	},

	unary: function() {
		if (this.check(Type.TRUE) ||
			this.check(Type.FALSE) ||
			this.check(Type.STRING) ||
			this.check(Type.SKIP) ||
			this.check(Type.URL))
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
		a = this.boolUnary()
		if (this.boolOpPending()) {
			b = this.boolOp()
			return Lexeme.prototype.cons(b.getType(), a, this.boolUnary())
		}
		return a
	},

	boolUnary: function() {
		if (this.check(Type.OPEN_PAREN)) {
			this.advance()
			let a = this.boolExpr()
			this.match(Type.CLOSE_PAREN)
			return Lexeme.prototype.cons(Type.OPEN_PAREN, null, a)
		}
		else if (this.intentPending())
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
		return this.match(Type.HASH)
	},

	entity: function() {
		return this.match(Type.AT)
	},

	context: function() {
		return this.match(Type.DOLLAR)
	},

	object: function() {
		let a
		this.match(Type.OPEN_BRACE)
		a = this.optAttrList()
		this.match(Type.CLOSE_BRACE)
		return Lexeme.prototype.cons(Type.OPEN_BRACE, null, a)
	},

	optAttrList: function() {
		if (this.attrListPending())
			return this.attrList()
		return null
	},

	attrList: function() {
		let a, b
		a = this.definition()
		if (this.check(Type.COMMA))
			this.advance()
		if (this.attrListPending())
			b = this.attrList()
		else
			b = null
		return Lexeme.prototype.cons(Type.ATTRIBUTE_LIST, a, b)
	},

	array: function() {
		this.match(Type.OPEN_BRACKET)
		let a = this.optMixinList()
		this.match(Type.CLOSE_BRACKET)
		return Lexeme.prototype.cons(Type.OPEN_BRACKET, null, a)
	},

	optMixinList: function() {
		if (this.mixinListPending())
			return this.mixinList()
		return null
	},

	mixinList: function() {
		let a, b
		a = this.mixin()
		if (this.check(Type.COMMA))
			this.advance()
		if (this.mixinListPending())
			b = this.mixinList()
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
		return (this.check(Type.ID) ||
				this.check(Type.STRING) ||
				this.implicitDefPending())
	},

	implicitDefPending: function() {
		return this.unaryPending()
	},

	unaryPending: function() {
		return (this.check(Type.TRUE) || 
				this.check(Type.FALSE) ||
				this.check(Type.STRING) ||
				this.check(Type.SKIP) ||
				this.check(Type.URL) ||
				this.numberPending() ||
				this.boolExprPending() ||
				this.objectPending() ||
				this.arrayPending())
	},

	numberPending: function() {
		return (this.nonNegNumPending() ||
				this.negNumPending())
	},

	boolExprPending: function() {
		return (this.check(Type.OPEN_PAREN) ||
				this.boolUnaryPending())
	},

	boolOpPending: function() {
		return (this.check(Type.AND) ||
				this.check(Type.OR))
	},

	boolUnaryPending: function() {
		return (this.intentPending() ||
				this.entityPending() ||
				this.contextPending())
	},

	intentPending: function() {
		return this.check(Type.HASH)
	},

	entityPending: function() {
		return this.check(Type.AT)
	},

	contextPending: function() {
		return this.check(Type.DOLLAR)
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
	},

	arrayPending: function() {
		return this.check(Type.OPEN_BRACKET)
	},

	mixinListPending: function() {
		return this.mixinPending()
	},

	mixinPending: function() {
		return (this.definitionPending() ||
				this.objectPending())
	}

}

var Pretty = function(outfile) {
	this.outfile = outfile
	this.numTabs = 0
}

Pretty.prototype = {

	printTabs: function() {
		for (let i = 0; i < this.numTabs; i++)
			fs.appendFileSync(this.outfile, '\t')
	},

	pp: function(tree) {
		if (tree == null) return
		switch (tree.getType()) {
			case Type.TRUE:
				fs.appendFileSync(this.outfile, "true")
				break
			case Type.FALSE:
				fs.appendFileSync(this.outfile, "false")
				break
			case Type.STRING:
				fs.appendFileSync(this.outfile, "\"" + tree.getValue() + "\"")
				break
			case Type.SKIP:
				fs.appendFileSync(this.outfile, "*")
				break
			case Type.URL:
				fs.appendFileSync(this.outfile, "<" + tree.getValue() + ">")
				break
			case Type.NUMBER:
				fs.appendFileSync(this.outfile, tree.getValue())
				break
			case Type.ID:
				fs.appendFileSync(this.outfile, tree.getValue())
				break
			case Type.PROGRAM:
				this.printProgram(tree)
				break
			case Type.EXPLICIT:
				this.printExplicit(tree)
				break
			case Type.IMPLICIT:
				this.printImplicit(tree)
				break
			case Type.MINUS:
				this.printMinus(tree)
				break
			case Type.OPEN_PAREN:
				this.printBoolExpr(tree)
				break
			case Type.HASH:
				fs.appendFileSync(this.outfile, '#' + tree.getValue())
				break
			case Type.AT:
				fs.appendFileSync(this.outfile, '@' + tree.getValue())
				break
			case Type.DOLLAR:
				fs.appendFileSync(this.outfile, '$' + tree.getValue())
				break
			case Type.AND:
				this.printAnd(tree)
				break
			case Type.OR:
				this.printOr(tree)
				break
			case Type.OPEN_BRACE:
				this.printObject(tree)
				break
			case Type.ATTRIBUTE_LIST:
				this.printAttributeList(tree)
				break
			case Type.OPEN_BRACKET:
				this.printArray(tree)
				break
			case Type.MIXIN_LIST:
				this.printMixinList(tree)
				break
			default:
				throw Error('Could not discern type ' + tree.getType() + '!')
		}
	},

	printProgram: function(tree) {
		fs.writeFileSync(this.outfile, '')
		while (tree != null) {
			this.printTabs()
			this.pp(tree.car())
			tree = tree.cdr()
			fs.appendFileSync(this.outfile, '\n')
		}
		return this.buffer
	},

	printExplicit: function(tree) {
		this.pp(tree.car())
		fs.appendFileSync(this.outfile, ": ")
		this.pp(tree.cdr())
	},

	printImplicit: function(tree) {
		this.pp(tree.car())
	},

	printMinus: function(tree) {
		fs.appendFileSync(this.outfile, '-')
		this.pp(tree.cdr())
	},

	printBoolExpr: function(tree) {
		fs.appendFileSync(this.outfile, "(")
		this.pp(tree.cdr())
		fs.appendFileSync(this.outfile, ")")
	},

	printAnd: function(tree) {
		this.pp(tree.car())
		fs.appendFileSync(this.outfile, " && ")
		this.pp(tree.cdr())
	},

	printOr: function(tree) {
		this.pp(tree.car())
		fs.appendFileSync(this.outfile, " || ")
		this.pp(tree.cdr())
	},

	printObject: function(tree) {
		fs.appendFileSync(this.outfile, '{')
		if (tree.cdr() != null) {
			fs.appendFileSync(this.outfile, '\n')
			this.numTabs++
			this.pp(tree.cdr())
			this.numTabs--
			this.printTabs()
		}
		fs.appendFileSync(this.outfile, '}')
	},

	printAttributeList: function(tree) {
		while (tree != null) {
			this.printTabs()
			this.pp(tree.car())
			tree = tree.cdr()
			fs.appendFileSync(this.outfile, '\n')
		}
	},

	printArray: function(tree) {
		fs.appendFileSync(this.outfile, "[")
		if (tree.cdr() != null) {
			fs.appendFileSync(this.outfile, "\n")
			this.numTabs++
			this.pp(tree.cdr())
			this.numTabs--
			this.printTabs()
		}
		fs.appendFileSync(this.outfile, "]")
	},

	printMixinList: function(tree) {
		while (tree != null) {
			this.printTabs()
			this.pp(tree.car())
			tree = tree.cdr()
			fs.appendFileSync(this.outfile, "\n")
		}
	}

}

module.exports = {
	Type: Type,
	Lexeme: Lexeme,
	Lexer: Lexer,
	Parser: Parser,
	Pretty: Pretty
}
