var Type = {
	
	OPEN_BRACKET: 'OPEN_BRACKET',
	CLOSE_BRACKET: 'CLOSE_BRACKET',
	OPEN_BRACE: 'OPEN_BRACE',
	CLOSE_BRACE: 'CLOSE_BRACE',
	COLON: 'COLON',
	COMMA: 'COMMA',
	MINUS: 'MINUS',
	STRING: 'STRING',
	NUMBER: 'NUMBER',
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

	END_OF_INPUT: 'END_OF_INPUT'
	
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

	getLeft: function() {
		return this.left
	},

	getRight: function() {
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

	setLeft: function(left) {
		let ret = this.left
		this.left = left
		return ret
	},

	setRight: function(right) {
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
		console.error('Error on line ', this.current.getLine(), ': expected ${expected}, got ', this.current.getType())
		throw Error('Illegal input.')
	},

	check: function(expected) {
		if (expected == this.current.getType())
			return true
		return false
	},

	advance: function() {
		let temp = this.current
		if (!this.check(Type.END_OF_INPUT))
			this.current = this.lexer.lex()
		return temp
	},

	program: function() {
		let a, b
		a = this.definition()
		if (this.check(Type.COMMA)) {
			this.advance()
			b = this.program()
		}
		else
			b = null
		return Lexeme.prototype.cons(Type.PROGRAM, a, b)
	},

	definition: function() {
		let a = this.match(Type.ID)
		this.match(Type.COLON)
		return Lexeme.prototype.cons(Type.COLON, a, this.unary())
	},

	unary: function() {
		if (this.check(Type.TRUE) ||
			this.check(Type.FALSE) ||
			this.check(Type.STRING))
				return this.advance()
		else if (this.numberPending())
			return this.number()
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

	object: function() {
		let a
		this.match(Type.OPEN_BRACE)
		a = this.optAttrList()
		this.match(Type.CLOSE_BRACE)
		return Lexeme.prototype.cons(Type.OBJECT, a, null)
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
		return this.check(Type.ID)
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

var Pretty = function(outfile) {
	this.outfile = outfile
	this.numTabs = 0
}

Pretty.prototype = {

	getBuffer: function() {
		return this.buffer
	},

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
			case Type.NUMBER:
				fs.appendFileSync(this.outfile, tree.getValue())
				break
			case Type.ID:
				fs.appendFileSync(this.outfile, tree.getValue())
				break
			case Type.PROGRAM:
				this.printProgram(tree)
				break
			case Type.MINUS:
				this.printMinus(tree)
				break
			case Type.COLON:
				this.printAssign(tree)
				break
			case Type.OBJECT:
				this.printObject(tree)
				break
			case Type.ATTRIBUTE_LIST:
				this.printAttributeList(tree)
				break
			case Type.MIXIN_LIST:
				this.printArray(tree)
				break
			default:
				throw Error('Could not discern type ' + tree.getType() + '!')
		}
	},

	printProgram: function(tree) {
		fs.writeFileSync(this.outfile, '')
		while (tree != null) {
			this.printTabs()
			this.pp(tree.getLeft())
			tree = tree.getRight()
			if (tree != null) {
				fs.appendFileSync(this.outfile, ',')
			}
			fs.appendFileSync(this.outfile, '\n')
		}
		return this.buffer
	},

	printMinus: function(tree) {
		fs.appendFileSync(this.outfile, '-')
		this.pp(tree.getRight())
	},

	printAssign: function(tree) {
		this.pp(tree.getLeft())
		fs.appendFileSync(this.outfile, ": ")
		this.pp(tree.getRight())
	},

	printObject: function(tree) {
		fs.appendFileSync(this.outfile, '{')
		if (tree.getLeft() != null) {
			fs.appendFileSync(this.outfile, '\n')
			this.numTabs++
			this.pp(tree.getLeft())
			this.numTabs--
			this.printTabs()
		}
		fs.appendFileSync(this.outfile, '}')
	},

	printAttributeList: function(tree) {
		while (tree != null) {
			this.printTabs()
			this.pp(tree.getLeft())
			tree = tree.getRight()
			if (tree != null) {
				fs.appendFileSync(this.outfile, ',')
			}
			fs.appendFileSync(this.outfile, '\n')
		}
	},

	printArray: function(tree) {
		fs.appendFileSync(this.outfile, "[\n")
		this.numTabs++
		while (tree != null) {
			this.printTabs()
			this.pp(tree.getLeft())
			tree = tree.getRight()
			if (tree != null) {
				fs.appendFileSync(this.outfile, ',')
			}
			fs.appendFileSync(this.outfile, '\n')
		}
		this.numTabs--
		this.printTabs()
		fs.appendFileSync(this.outfile, ']')
	}

}

var Translator = function(outfile) {
	this.outfile = outfile
	this.numTabs = 0
	this.inObject = false
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
			case Type.NUMBER:
				fs.appendFileSync(this.outfile, tree.getValue())
				break
			case Type.ID:
				fs.appendFileSync(this.outfile, tree.getValue())
				break
			case Type.PROGRAM:
				this.translateProgram(tree)
				break
			case Type.MINUS:
				this.translateMinus(tree)
				break
			case Type.COLON:
				this.translateAssign(tree)
				break
			case Type.OBJECT:
				this.translateObject(tree)
				break
			case Type.ATTRIBUTE_LIST:
				this.translateAttributeList(tree)
				break
			case Type.MIXIN_LIST:
				this.translateArray(tree)
				break
			default:
				throw Error('Error translating type ' + tree.getType() + '!')
		}
	},

	translateProgram: function(tree) {
		fs.writeFileSync(this.outfile, '')
		while (tree != null) {
			this.printTabs()
			this.translate(tree.getLeft())
			tree = tree.getRight()
			fs.appendFileSync(this.outfile, '\n')
		}
	},

	translateMinus: function(tree) {
		fs.appendFileSync(this.outfile, '-')
		this.translate(tree.getRight())
	},

	translateAssign: function(tree) {
		if (!this.inObject)
			fs.appendFileSync(this.outfile, 'var ')
		this.translate(tree.getLeft())
		if (this.inObject)
			fs.appendFileSync(this.outfile, ': ')
		else fs.appendFileSync(this.outfile, ' = ')
		this.translate(tree.getRight())
	},

	translateObject: function(tree) {
		fs.appendFileSync(this.outfile, '{')
		if (tree.getLeft() != null) {
			fs.appendFileSync(this.outfile, '\n')
			this.numTabs++
			this.translate(tree.getLeft())
			this.numTabs--
			this.printTabs()
		}
		fs.appendFileSync(this.outfile, '}')
	},

	translateAttributeList: function(tree) {
		while (tree != null) {
			this.inObject = 1
			this.printTabs()
			this.translate(tree.getLeft())
			tree = tree.getRight()
			if (tree != null)
				fs.appendFileSync(this.outfile, ',')
			fs.appendFileSync(this.outfile, '\n')
		}
		this.inObject = 0
	},

	translateArray: function(tree) {
		fs.appendFileSync(this.outfile, '[\n')
		this.numTabs++
		while (tree != null) {
			this.printTabs()
			this.translate(tree.getLeft())
			tree = tree.getRight()
			if (tree != null)
				fs.appendFileSync(this.outfile, ',')
			fs.appendFileSync(this.outfile, '\n')
		}
		this.numTabs--
		this.printTabs()
		fs.appendFileSync(this.outfile, ']')
	}

}

module.exports = {
	Type: Type,
	Lexeme: Lexeme,
	Lexer: Lexer,
	Parser: Parser,
	Pretty: Pretty,
	Translator: Translator
}
