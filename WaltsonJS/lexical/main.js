var {Type} = require('./Lexical')
var {Lexeme} = require('./Lexical')
var {Lexer} = require('./Lexical')

function main() {
	let argv = process.argv
	let argc = argv.length
	let lexer = new Lexer(argv[argc-1])
	let cur = lexer.lex()
	while (cur.getType() != Type.END_OF_INPUT) {
		process.stdout.write(cur.getType())
		if (cur.getType() == Type.NUMBER || cur.getType() == Type.STRING || cur.getType() == Type.ID)
			console.log(" " + cur.getValue())
		else
			console.log()
		cur = lexer.lex()
	}
	console.log(cur.type)
}

main()