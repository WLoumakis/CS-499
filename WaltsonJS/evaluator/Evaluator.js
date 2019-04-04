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
	},

	displayLexeme: function() {
		switch (this.type) {
			case Type.STRING:
				console.log('"' + this.value + '"')
				break
			case Type.NUMBER:
				console.log(this.value)
				break
			default:
				console.log(this.type)
				break
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
		throw Error('Error on line ' + this.current.getLine() + ': expected ' + expected + ', got ' + this.current.getType())
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

var Environment = function() {
	return Lexeme.prototype.cons(Type.ENVIRONMENT, Lexeme.prototype.cons(Type.TABLE, null, null), null)
}

Environment.prototype = {

	insert: function(env, variable, value) {
		let table = env.getLeft()
		table.setLeft(Lexeme.prototype.cons(Type.ID_LIST, variable, table.getLeft()))
		table.setRight(Lexeme.prototype.cons(Type.VALUE_LIST, value, table.getRight()))
	},

	lookup: function(env, variable) {
		while (env != null) {
			let table = env.getLeft()
			let vars = table.getLeft()
			let vals = table.getRight()
			while (vars != null) {
				if (this.sameVariable(vars.getLeft(), variable))
					return vals.getLeft()
				vars = vars.getRight()
				vals = vals.getRight()
			}
			env = env.getRight()
		}
		throw Error('Could not find ', variable.getValue(), ' within scope!')
	},

	sameVariable: function(left, right) {
		return (left.getValue() == right.getValue())
	},

	exists: function(env, variable) {
		while (env != null) {
			let table = env.getLeft()
			let vars = table.getLeft()
			let vals = table.getRight()
			while (vars != null) {
				if (this.sameVariable(vars.getLeft(), variable))
					return true
				vars = vars.getRight()
				vals = vals.getRight()
			}
			env = env.getRight()
		}
		return false
	},

	lookupLocal: function(env, variable) {
		let table = env.getLeft()
		let vars = table.getLeft()
		let vals = table.getRight()
		while (vars != null) {
			if (this.sameVariable(vars.getLeft(), variable))
				return vals.getLeft()
			vars = vars.getRight()
			vals = vals.getRight()
		}
		throw Error('Could not find ', variable.getValue(), ' in the environment!')
	},

	existsLocal: function(env, variable) {
		let table = env.getLeft()
		let vars = table.getLeft()
		let vals = table.getRight()
		while (vars != null) {
			if (this.sameVariable(vars.getLeft(), variable))
				return true
			vars = vars.getRight()
			vals = vals.getRight()
		}
		return false
	},

	update: function(env, variable, value) {
		while (env != null) {
			let table = env.getLeft()
			let vars = table.getLeft()
			let vals = table.getRight()
			while (vars != null) {
				if (this.sameVariable(vars.getLeft(), variable))
					return vals.setLeft(value)
				vars = vars.getRight()
				vals = vals.getRight()
			}
			env = env.getRight()
		}
		throw Error('Error updating ', variable.getValue(), ': Could not find variable within scope!')
	},

	deleteHelper: function(vars, vals, table) {
		if (vars == null) {
			let retVar = table.getLeft()
			let retVal = table.getRight()
			table.setLeft(retVar.getRight())
			table.setRight(retVal.getRight())

			retVar.setRight(null)
			retVal.setRight(null)

			return retVal.getLeft()
		}

		let retVar = vars.getRight()
		let retVal = vals.getRight()

		vars.setRight(retVar.getRight())
		vals.setRight(retVal.getRight())

		return retVal.getLeft()
	},

	delete: function(env, variable) {
		while (env != null) {
			let table = env.getLeft()
			let vars = table.getLeft()
			let vals = table.getRight()
			let prevVar = null
			let prevVal = null
			while (vars != null) {
				if (this.sameVariable(vars.getLeft(), variable))
					return deleteHelper(prevVar, prevVal, table)
				prevVar = vars
				prevVal = vals
				vars = vars.getRight()
				vals = vals.getRight()
			}
			env = env.getRight()
		}
		throw Error('Error deleting ', variable.getValue(), ': Could not find variable within scope!')
	},

	extend: function(env, vars, vals) {
		return Lexeme.prototype.cons(Type.ENVIRONMENT, Lexeme.prototype.cons(Type.TABLE, vars, vals), env)
	},

	displayEnvironment: function(env) {
		if (env == null) return
		let table = env.getLeft()
		let vars = table.getLeft()
		let vals = table.getRight()
		console.log('The local environment is. . .')
		if (vars == null) {
			console.log('EMPTY!')
		}
		while (vars != null) {
			process.stdout.write("var " + vars.getLeft().getValue() + " = ")
			vals.getLeft().displayLexeme()
			vars = vars.getRight()
			vals = vals.getRight()
		}
		console.log('-------------------------------------------------')
	},

	displayEnvironments: function(env) {
		console.log('--------The environments within scope are--------')
		while (env != null) {
			this.displayEnvironment(env)
			env = env.getRight()
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
			case Type.NUMBER:
				return tree
			case Type.ID:
				return Environment.prototype.lookup(env, tree)
			case Type.PROGRAM:
				return this.evalProgram(tree, env)
			case Type.MINUS:
				return this.evalMinus(tree, env)
			case Type.COLON:
				return this.evalAssign(tree, env)
			case Type.OBJECT:
				return this.evalObject(tree, env)
			case Type.ATTRIBUTE_LIST:
				return this.evalAttributeList(tree, env)
			case Type.MIXIN_LIST:
				return this.evalArray(tree, env)
			default:
				throw Error('Error evaluating type ' + tree.getType() + '!')
		}
	},

	evalProgram: function(tree, env) {
		let ret = null
		while (tree != null) {
			ret = this.eval(tree.getLeft(), env)
			tree = tree.getRight()
		}
		return ret
	},

	evalMinus: function(tree, env) {
		let num = this.eval(tree.getRight(), env)
		return new Lexeme(Type.NUMBER, -1 * num, undefined, undefined, undefined)
	},

	evalAssign: function(tree, env) {
		let id = tree.getLeft()
		let result = this.eval(tree.getRight(), env)
		return Environment.prototype.insert(env, id, result)
	},

	evalObject: function(tree, env) {
		let xenv = Environment.prototype.extend(env, null, null)
		this.eval(tree.getLeft(), xenv)
		return xenv
	},

	evalAttributeList: function(list, env) {
		let ret = null
		while (list != null) {
			ret = this.eval(list.getLeft(), env)
			list = list.getRight()
		}
		return ret
	},

	evalArray: function(list, env) {
		let temp = list
		let count = 0
		while (temp != null) {
			temp = temp.getRight()
			count++
		}
		let arr = []
		while (list != null) {
			arr.push(this.eval(list.getLeft(), env))
			list = list.getRight()
		}
		return new Lexeme(Type.ARRAY, arr, undefined, undefined, undefined)
	}

}

module.exports = {
	Type: Type,
	Lexeme: Lexeme,
	Lexer: Lexer,
	Parser: Parser,
	Pretty: Pretty,
	Environment: Environment,
	Evaluator, Evaluator
}
