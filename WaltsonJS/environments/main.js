var {Type} = require('./Environment')
var {Lexeme} = require('./Environment')
var {Environment} = require('./Environment')

function main() {
	let global = new Environment()
	Environment.prototype.displayEnvironment(global)
	
	let variable = new Lexeme(Type.ID, 'x', undefined, undefined, undefined)
	let value = new Lexeme(Type.NUMBER, 42, undefined, undefined, undefined)

	console.log('Inserting x = 42 into the global environment. . .')

	Environment.prototype.insert(global, variable, value)

	Environment.prototype.displayEnvironment(global)

	let newVar = new Lexeme(Type.ID, "y", undefined, undefined, undefined)
	let newVal = new Lexeme(Type.STRING, "Hello World!", undefined, undefined, undefined)

	console.log('Inserting y = "Hello World!" into the global environment. . .')

	Environment.prototype.insert(global, newVar, newVal)

	Environment.prototype.displayEnvironments(global)

	console.log('Extending the global environment. . .')
	let xenv = Environment.prototype.extend(global, null, null)

	console.log('Extended! Now displaying environments accessible from the new environment. . .')
	Environment.prototype.displayEnvironments(xenv)
}

main()