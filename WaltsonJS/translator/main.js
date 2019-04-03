var {Parser} = require('./Translator')
var {Translator} = require('./Translator')

function main() {
	let argv = process.argv
	let argc = argv.length
	if (argc != 4)
		throw Error('Usage: nodejs main.js <infile> <outfile>')
	let parser = new Parser(argv[argc-2])
	let tree = parser.parse()

	let translator = new Translator(argv[argc-1])
	translator.translate(tree)
}

main()