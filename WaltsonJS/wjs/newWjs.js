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
	EXPRESSION: 'EXPRESSION',

	GLUE: 'GLUE'
	
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
	},

	displayLexeme() {
		switch (this.type) {
			case Type.ID:
				console.log(this.value)
				break
			case Type.STRING:
				console.log('"' + this.value + '"')
				break
			case Type.SKIP:
				console.log('')
				break
			case Type.NUMBER:
				console.log(this.value)
				break
			case Type.ENVIRONMENT:
				console.log(this.value + '.ENVIRONMENT')
				break
			case Type.TRUE:
				console.log('true')
				break
			case Type.FALSE:
				console.log('false')
				break
			case Type.HASH:
				console.log(this.value)
				break
			case Type.AT:
				console.log(this.value)
				break
			case Type.DOLLAR:
				console.log(this.value)
				break
			case Type.URL:
				console.log('<' + this.value + '>')
				break
			case Type.OPEN_PAREN:
				console.log('BOOLEAN EXPRESSION')
				break
			case Type.AND:
				console.log('&&')
				break
			case Type.OR:
				console.log('||')
				break
			case Type.OPEN_BRACE:
				console.log('OBJECT')
				break
			case Type.OPEN_BRACKET:
				console.log('ARRAY')
				break
			case Type.ARRAY:
				console.log(this.value)
				break
			default:
				throw Error('Error displaying value with type ' + this.type + '!')
		}
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

	lexVarOrKey: function() {
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
		if (ret == 'true')
			return new Lexeme(Type.TRUE, undefined, this.line, undefined, undefined)
		if (ret == 'false')
			return new Lexeme(Type.FALSE, undefined, undefined, undefined, undefined)
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
					return this.lexVarOrKey()
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
				this.printIntent(tree)
				break
			case Type.AT:
				this.printEntity(tree)
				break
			case Type.DOLLAR:
				this.printContext(tree)
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

	printIntent: function(tree) {
		fs.appendFileSync(this.outfile, "#")
		fs.appendFileSync(this.outfile, tree.getValue())
	},

	printEntity: function(tree) {
		fs.appendFileSync(this.outfile, "@")
		fs.appendFileSync(this.outfile, tree.getValue())
	},

	printContext: function(tree) {
		fs.appendFileSync(this.outfile, "$")
		fs.appendFileSync(this.outfile, tree.getValue())
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

var Translator = function(outfile) {
	this.outfile = outfile
	this.numTabs = 0
	this.inObject = false
	this.inExpr = false
}

Translator.prototype = {

	printTabs: function() {
		for (let i = 0; i < this.numTabs; i++)
			fs.appendFileSync(this.outfile, '\t')
	},

	translate: function(tree) {
		if (tree == null) return
		switch(tree.getType()) {
			case Type.TRUE:
				fs.appendFileSync(this.outfile, 'true')
				break
			case Type.FALSE:
				fs.appendFileSync(this.outfile, 'false')
				break
			case Type.STRING:
				fs.appendFileSync(this.outfile, '"' + tree.getValue() + '"')
				break
			case Type.SKIP:
				break
			case Type.URL:
				fs.appendFileSync(this.outfile, '"' + tree.getValue() + '"')
				break
			case Type.NUMBER:
				fs.appendFileSync(this.outfile, tree.getValue())
				break
			case Type.ID:
				fs.appendFileSync(this.outfile, tree.getValue())
				break
			case Type.PROGRAM:
				this.translateProgram(tree)
				break
			case Type.EXPLICIT:
				this.translateExplicit(tree)
				break
			case Type.IMPLICIT:
				this.translateImplicit(tree)
				break
			case Type.MINUS:
				this.translateMinus(tree)
				break
			case Type.OPEN_PAREN:
				this.translateBoolExpr(tree)
				break
			case Type.HASH:
				this.translateIntent(tree)
				break
			case Type.AT:
				this.translateEntity(tree)
				break
			case Type.DOLLAR:
				this.translateContext(tree)
				break
			case Type.AND:
				this.translateAnd(tree)
				break
			case Type.OR:
				this.translateOr(tree)
				break
			case Type.OPEN_BRACE:
				this.translateObject(tree)
				break
			case Type.ATTRIBUTE_LIST:
				this.translateAttributeList(tree)
				break
			case Type.OPEN_BRACKET:
				this.translateArray(tree)
				break
			case Type.MIXIN_LIST:
				this.translateMixinList(tree)
				break
			default:
				throw Error('Error translating type ' + tree.getType() + '!')
		}
	},

	translateProgram: function(tree) {
		fs.writeFileSync(this.outfile, '')
		while (tree != null) {
			this.printTabs()
			this.translate(tree.car())
			tree = tree.cdr()
			fs.appendFileSync(this.outfile, '\n')
		}
	},

	translateExplicit: function(tree) {
		this.translate(tree.car())
		if (this.inObject)
			fs.appendFileSync(this.outfile, ': ')
		else
			fs.appendFileSync(this.outfile, ' = ')
		this.translate(tree.cdr())
		this.inExpr = false
	},

	translateImplicit: function(tree) {
		this.translate(tree.car())
	},

	translateMinus: function(tree) {
		fs.appendFileSync(this.outfile, '-')
		this.translate(tree.cdr())
	},

	translateBoolExpr: function(tree) {
		if (this.inExpr == false) {
			fs.appendFileSync(this.outfile, '"(')
			this.inExpr = true
			this.translate(tree.cdr())
			this.inExpr = false
			fs.appendFileSync(this.outfile, ')"')
		}
		else {
			fs.appendFileSync(this.outfile, '(')
			this.translate(tree.cdr())
			fs.appendFileSync(this.outfile, ')')
		}
	},

	translateIntent: function(tree) {
		if (this.inExpr == false)
			fs.appendFileSync(this.outfile, '"#' + tree.getValue() + '"')
		else
			fs.appendFileSync(this.outfile, '#' + tree.getValue())
	},

	translateEntity: function(tree) {
		if (this.inExpr == false)
			fs.appendFileSync(this.outfile, '"@' + tree.getValue() + '"')
		else
			fs.appendFileSync(this.outfile, '@' + tree.getValue())
	},

	translateContext: function(tree) {
		if (this.inExpr == false)
			fs.appendFileSync(this.outfile, '"$' + tree.getValue() + '"')
		else
			fs.appendFileSync(this.outfile, '$' + tree.getValue())
	},

	translateAnd: function(tree) {
		if (this.inExpr == false) {
			fs.appendFileSync(this.outfile, '"')
			this.inExpr = true
			this.translate(tree.car())
			fs.appendFileSync(this.outfile, ' && ')
			this.translate(tree.cdr())
			this.inExpr = false
			fs.appendFileSync(this.outfile, '"')

		}
		else {
			this.translate(tree.car())
			fs.appendFileSync(this.outfile, ' && ')
			this.translate(tree.cdr())
		}
	},

	translateOr: function(tree) {
		if (this.inExpr == false) {
			fs.appendFileSync(this.outfile, '"')
			this.inExpr = true
			this.translate(tree.car())
			fs.appendFileSync(this.outfile, ' || ')
			this.translate(tree.cdr())
			this.inExpr = false
			fs.appendFileSync(this.outfile, '"')
		}
		else {
			this.translate(tree.car())
			fs.appendFileSync(this.outfile, ' || ')
			this.translate(tree.cdr())
		}
	},

	translateObject: function(tree) {
		fs.appendFileSync(this.outfile, '{')
		if (tree.cdr() != null) {
			fs.appendFileSync(this.outfile, '\n')
			this.numTabs++
			this.translate(tree.cdr())
			this.numTabs--
			this.printTabs()
		}
		fs.appendFileSync(this.outfile, '}')
	},

	translateAttributeList: function(tree) {
		while (tree != null) {
			this.inObject = 1
			this.printTabs()
			this.translate(tree.car())
			tree = tree.cdr()
			if (tree != null)
				fs.appendFileSync(this.outfile, ',')
			fs.appendFileSync(this.outfile, '\n')
		}
		this.inObject = 0
	},

	translateArray: function(tree) {
		fs.appendFileSync(this.outfile, '[')
		if (tree.cdr() != null) {
			fs.appendFileSync(this.outfile, '\n')
			this.numTabs++
			this.translate(tree.cdr())
			this.numTabs--
			this.printTabs()
		}
		fs.appendFileSync(this.outfile, ']')
	},

	translateMixinList: function(tree) {
		while (tree != null) {
			this.printTabs()
			this.translate(tree.car())
			tree = tree.cdr()
			if (tree != null)
				fs.appendFileSync(this.outfile, ',')
			fs.appendFileSync(this.outfile, '\n')
		}
	}

}

var Environment = function() {
	let env = Lexeme.prototype.cons(Type.ENVIRONMENT, Lexeme.prototype.cons(Type.TABLE, null, null), null)
	env.setValue(Type.ENVIRONMENT)
	return env
}

Environment.prototype = {

	insert: function(env, variable, value) {
		let table = env.car()
		if (table.car() == null) {
			table.setCar(Lexeme.prototype.cons(Type.ID_LIST, variable, null))
			table.setCdr(Lexeme.prototype.cons(Type.VALUE_LIST, value, null))
		}
		else {
			let vars = table.car()
			let vals = table.cdr()
			while (vars.cdr() != null) {
				vars = vars.cdr()
				vals = vals.cdr()
			}
			vars.setCdr(Lexeme.prototype.cons(Type.ID_LIST, variable, null))
			vals.setCdr(Lexeme.prototype.cons(Type.ID_LIST, value, null))
		}
		return value
	},

	lookup: function(env, variable) {
		while (env != null) {
			let table = env.car()
			let vars = table.car()
			let vals = table.cdr()
			while (vars != null) {
				if (this.sameVariable(vars.car(), variable))
					return vals.car()
				vars = vars.cdr()
				vals = vals.cdr()
			}
			env = env.cdr()
		}
		throw Error('Could not find ', variable.getValue(), ' within scope!')
	},

	sameVariable: function(left, right) {
		return (left.getValue() == right.getValue())
	},

	exists: function(env, variable) {
		while (env != null) {
			let table = env.car()
			let vars = table.car()
			let vals = table.cdr()
			while (vars != null) {
				if (this.sameVariable(vars.car(), variable))
					return true
				vars = vars.cdr()
				vals = vals.cdr()
			}
			env = env.cdr()
		}
		return false
	},

	lookupLocal: function(env, variable) {
		let table = env.car()
		let vars = table.car()
		let vals = table.cdr()
		while (vars != null) {
			if (this.sameVariable(vars.car(), variable))
				return vals.car()
			vars = vars.cdr()
			vals = vals.cdr()
		}
		throw Error('Could not find ', variable.getValue(), ' in the environment!')
	},

	existsLocal: function(env, variable) {
		let table = env.car()
		let vars = table.car()
		let vals = table.cdr()
		while (vars != null) {
			if (this.sameVariable(vars.car(), variable))
				return true
			vars = vars.cdr()
			vals = vals.cdr()
		}
		return false
	},

	update: function(env, variable, value) {
		while (env != null) {
			let table = env.car()
			let vars = table.car()
			let vals = table.cdr()
			while (vars != null) {
				if (this.sameVariable(vars.car(), variable))
					return vals.setCar(value)
				vars = vars.cdr()
				vals = vals.cdr()
			}
			env = env.cdr()
		}
		throw Error('Error updating ', variable.getValue(), ': Could not find variable within scope!')
	},

	updateLocalSpecific: function(env, variable, value) {
		let table = env.car()
		let vars = table.car()
		let vals = table.cdr()
		while (vars != null) {
			if (vars.car() == variable)
				return vals.setCar(value)
			vars = vars.cdr()
			vals = vals.cdr()
		}
	},

	deleteHelper: function(vars, vals, table) {
		if (vars == null) {
			let retVar = table.car()
			let retVal = table.cdr()
			table.setCar(retVar.cdr())
			table.setCdr(retVal.cdr())

			retVar.setCdr(null)
			retVal.setCdr(null)

			return retVal.car()
		}

		let retVar = vars.cdr()
		let retVal = vals.cdr()

		vars.setCdr(retVar.cdr())
		vals.setCdr(retVal.cdr())

		return retVal.car()
	},

	delete: function(env, variable) {
		while (env != null) {
			let table = env.car()
			let vars = table.car()
			let vals = table.cdr()
			let prevVar = null
			let prevVal = null
			while (vars != null) {
				if (this.sameVariable(vars.car(), variable))
					return deleteHelper(prevVar, prevVal, table)
				prevVar = vars
				prevVal = vals
				vars = vars.cdr()
				vals = vals.cdr()
			}
			env = env.cdr()
		}
		throw Error('Error deleting ', variable.getValue(), ': Could not find variable within scope!')
	},

	extend: function(env, type, vars, vals) {
		let xenv = Lexeme.prototype.cons(Type.ENVIRONMENT, Lexeme.prototype.cons(Type.TABLE, vars, vals), env)
		xenv.setValue(type)
		return xenv
	},

	displayEnvironment: function(env) {
		if (env == null) return
		let table = env.car()
		let vars = table.car()
		let vals = table.cdr()
		console.log('The local environment is. . .')
		if (vars == null) {
			console.log('EMPTY!')
		}
		while (vars != null) {
			process.stdout.write("var " + vars.car().getValue() + " = ")
			vals.car().displayLexeme()
			vars = vars.cdr()
			vals = vals.cdr()
		}
		console.log('-------------------------------------------------')
	},

	displayEnvironments: function(env) {
		console.log('--------The environments within scope are--------')
		while (env != null) {
			this.displayEnvironment(env)
			env = env.cdr()
		}
	}

}

var Evaluator = function() {
	
}

Evaluator.prototype = {

	eval: function(tree, env) {
		if (tree == null)
			return
		switch (tree.getType()) {
			case Type.TRUE:
				return tree
			case Type.FALSE:
				return tree
			case Type.STRING:
				return tree
			case Type.SKIP:
				return tree
			case Type.URL:
				return tree
			case Type.NUMBER:
				return tree
			case Type.ID:
				return Environment.prototype.lookup(env, tree)
			case Type.PROGRAM:
				return this.evalProgram(tree, env)
			case Type.EXPLICIT:
				return this.evalExplicit(tree, env)
			case Type.IMPLICIT:
				return this.evalImplicit(tree, env)
			case Type.MINUS:
				return this.evalMinus(tree, env)
			case Type.OPEN_PAREN:
				return tree
			case Type.HASH:
				return tree
			case Type.AT:
				return tree
			case Type.DOLLAR:
				return tree
			case Type.AND:
				return tree
			case Type.OR:
				return tree
			case Type.OPEN_BRACE:
				return this.evalObject(tree, env)
			case Type.ATTRIBUTE_LIST:
				return this.evalAttributeList(tree, env)
			case Type.OPEN_BRACKET:
				return this.evalArray(tree, env)
			case Type.MIXIN_LIST:
				return this.evalMixinList(tree, env)
			default:
				throw Error('Error evaluating type ' + tree.getType() + '!')
		}
	},

	evalProgram: function(tree, env) {
		let ret = null
		while (tree != null) {
			ret = this.eval(tree.car(), env)
			tree = tree.cdr()
		}
		return ret
	},

	evalExplicit: function(tree, env) {
		let id = tree.car()
		let result = this.eval(tree.cdr(), env)
		return Environment.prototype.insert(env, id, result)
	},

	evalImplicit: function(tree, env) {
		let id = new Lexeme(Type.ID, "unbound", undefined, undefined, undefined)
		let result = this.eval(tree.car(), env)
		return Environment.prototype.insert(env, id, result)
	},

	evalMinus: function(tree, env) {
		let num = this.eval(tree.cdr(), env)
		return new Lexeme(Type.NUMBER, -1 * num, undefined, undefined, undefined)
	},

	evalObject: function(tree, env) {
		let xenv = Environment.prototype.extend(env, Type.OBJECT, null, null)
		this.eval(tree.cdr(), xenv)
		return xenv
	},

	evalAttributeList: function(list, env) {
		let ret = null
		while (list != null) {
			ret = this.eval(list.car(), env)
			list = list.cdr()
		}
		return ret
	},

	evalArray: function(tree, env) {
		let xenv = Environment.prototype.extend(env, Type.ARRAY, null, null)
		this.eval(tree.cdr(), xenv)
		return xenv
	},

	evalMixinList: function(list, env) {
		let ret = null
		while (list != null) {
			ret = this.eval(list.car(), env)
			list = list.cdr()
		}
		return ret
	}

}

var WJS = function(outfile, global) {
	this.outfile = outfile
	this.global = global
	this.iam_apikey = false
	this.iam_access_token = false
	this.iam_url = false
	this.username = false
	this.password = false
	this.url = false
	this.version = false
	this.workspace_id = false
	this.intents = false
	this.entities = false
	this.dialog_nodes = false
	this.counterexamples = false
	this.translator = new Translator(outfile)
	this.inObject = false
}

WJS.prototype = {

	translate: function() {
		let table = this.global.car()
		let vars = table.car()
		let vals = table.cdr()
		fs.writeFileSync(this.outfile, '')
		while (vars != null) {
			if (this.inObject == false)
				fs.appendFileSync(this.outfile, 'var ')
			this.translator.translate(vars.car())
			if (this.inObject == false)
				fs.appendFileSync(this.outfile, ' = ')
			else
				fs.appendFileSync(this.outfile, ': ')
			this.translator.translate(vals.car())
			vars = vars.cdr()
			vals = vals.cdr()
		}
	},

	intentsPresent: function() {
		return this.intents
	},

	entitiesPresent: function() {
		return this.entities
	},

	dialogNodesPresent: function() {
		return this.dialog_nodes
	},

	counterexamplesPresent: function() {
		return this.counterexamples
	},

	performChecksAndSets: function() {
		console.log('before checkAndSetGlobal()')
		this.checkAndSetGlobal()
		console.log('after checkAndSetGlobal')
		if (this.anyParamMissing()) {
			console.log('before checkAndSetImplicitGlobalVars()')
			this.checkAndSetImplicitGlobalVars()
			console.log('after checkAndSetImplicitGlobalVars()')
		}
		console.log('after chceking for paramMissing and doing checkAndSetImplicitGlobalVars()')
		if (this.anyParamMissing()) {
			console.log('before checkOneAboveGlobal()')
			this.checkOneAboveGlobal()
			console.log('after checkOneAboveGlobal()')
		}
	},

	anyParamMissing: function() {
		return (!this.intents || !this.entities || !this.dialog_nodes || !this.counterexamples)
	},

	checkAndSetGlobal: function() {
		this.iam_apikey = this.checkIamApikey(this.global)
		this.iam_access_token = this.checkIamAccessToken(this.global)
		this.iam_url = this.checkIamUrl(this.global)
		this.username = this.checkUsername(this.global)
		this.password = this.checkPassword(this.global)
		this.url = this.checkUrl(this.global)
		this.version = this.checkVersion(this.global)
		this.workspace_id = this.checkWorkspaceId(this.global)
		this.intents = this.checkIntents(this.global)
		this.entities = this.checkEntities(this.global)
		this.dialog_nodes = this.checkDialogNodes(this.global)
		this.counterexamples = this.checkCounterexamples(this.global)
	},

	checkIamApikey: function(env) {
		let iam_apikey = new Lexeme(Type.ID, 'iam_apikey', undefined, undefined, undefined)
		return Environment.prototype.exists(env, iam_apikey)
	},

	checkIamAccessToken: function(env) {
		let iam_access_token = new Lexeme(Type.ID, 'iam_access_token', undefined, undefined, undefined)
		return Environment.prototype.exists(env, iam_access_token)
	},

	checkIamUrl: function(env) {
		let iam_url = new Lexeme(Type.ID, 'iam_url', undefined, undefined, undefined)
		return Environment.prototype.exists(env, iam_url)
	},

	checkUsername: function(env) {
		let username = new Lexeme(Type.ID, 'username', undefined, undefined, undefined)
		return Environment.prototype.exists(env, username)
	},

	checkPassword: function(env) {
		let password = new Lexeme(Type.ID, 'password', undefined, undefined, undefined)
		return Environment.prototype.exists(env, password)
	},

	checkUrl: function(env) {
		let url = new Lexeme(Type.ID, 'url', undefined, undefined, undefined)
		return Environment.prototype.exists(env, url)
	},

	checkVersion: function(env) {
		let version = new Lexeme(Type.ID, 'version', undefined, undefined, undefined)
		return Environment.prototype.exists(env, version)
	},

	checkWorkspaceId: function(env) {
		let workspace_id = new Lexeme(Type.ID, 'workspace_id', undefined, undefined, undefined)
		return Environment.prototype.exists(env, workspace_id)
	},
	
	checkIntents: function(env) {
		let intents = new Lexeme(Type.ID, "intents", undefined, undefined, undefined)
		return Environment.prototype.existsLocal(env, intents)
	},

	checkEntities: function(env) {
		let entities = new Lexeme(Type.ID, "entities", undefined, undefined, undefined)
		return Environment.prototype.existsLocal(env, entities)
	},

	checkDialogNodes: function(env) {
		let dialog_nodes = new Lexeme(Type.ID, "dialog_nodes", undefined, undefined, undefined)
		return Environment.prototype.existsLocal(env, dialog_nodes)
	},

	checkCounterexamples: function(env) {
		let counterexamples = new Lexeme(Type.ID, "counterexamples", undefined, undefined, undefined)
		return Environment.prototype.existsLocal(env, counterexamples)
	},

	checkAndSetImplicitGlobalVars: function() {
		let table = this.global.car()
		let vars = table.car()
		let vals = table.cdr()
		let watson_attr = 0
		while (vars != null) {
			if ((vars.car().getValue() == 'unbound'||
				this.isWatsonParam(vars.car().getValue())) &&
			   	(vals.car().getType() == Type.SKIP ||
				vals.car().getType() == Type.ARRAY)) {
					switch (watson_attr) {
						case 0:
							if (vars.car().getValue() == 'unbound')
								vars.car().setValue('intents')
							watson_attr++
							break
						case 1:
							if (vars.car().getValue() == 'unbound')
								vars.car().setValue('entities')
							watson_attr++
							break
						case 2:
							if (vars.car().getValue() == 'unbound')
								vars.car().setValue('dialog_nodes')
							watson_attr++
							break
						case 3:
							if (vars.car().getValue() == 'unbound')
								vars.car().setValue('counterexamples')
							watson_attr++
							break
						default:
							throw Error("Couldn't understand more than 4 implicit definitions!")
					}
					if (vals.car().getType() == Type.SKIP)
						vals.setCar(new Lexeme(Type.ARRAY, [], undefined, undefined, undefined))
			}

			vars = vars.cdr()
			vals = vals.cdr()
		}
	},

	checkOneAboveGlobal: function() {
		let valid = []
		let table = this.global.car()
		let vars = table.car()
		let vals = table.cdr()
		while (vals != null) {
			if (vals.car().getType() == Type.ENVIRONMENT)
				valid.push(Lexeme.prototype.cons(Type.GLUE, vars.car(), vals.car()))
			vars = vars.cdr()
			vals = vals.cdr()
		}
		for (let i = 0; i < valid.length; i++) {
			if (this.workspace_id == false && this.checkWorkspaceId(valid[i].cdr())) {
				this.flatten(valid[i])
			}
			if (this.intents == false && this.checkIntents(valid[i].cdr())) {
				this.flatten(valid[i])
			}
			if (this.entities == false && this.checkEntities(valid[i].cdr())) {
				this.flatten(valid[i])
			}
			if (this.dialog_nodes == false && this.checkDialogNodes(valid[i].cdr())) {
				this.flatten(valid[i])
			}
			if (this.counterexamples == false && this.checkCounterexamples(valid[i].cdr())) {
				this.flatten(valid[i])
			}
			//Environment.prototype.delete(this.global, valid[i].car())
		}
		
	},

	flatten: function(tree) {
		Environment.prototype.updateLocalSpecific(this.global, tree.car(), tree.cdr())
	},

	isWatsonParam: function(str) {
		if (str == 'intents' ||
			str == 'entities' ||
			str == 'dialog_nodes' ||
			str == 'counterexamples')
				return true
		return false
	}

}

module.exports = {
	Type: Type,
	Lexeme: Lexeme,
	Lexer: Lexer,
	Parser: Parser,
	Pretty: Pretty,
	Environment: Environment,
	Translator: Translator,
	Evaluator, Evaluator,
	WJS, WJS
}
