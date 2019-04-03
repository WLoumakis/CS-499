var {Parser} = require('./Pretty')
var {Pretty} = require('./Pretty')
let fs = require('fs')

function main() {
	let argv = process.argv
	let argc = argv.length
	if (argc != 4)
		throw Error('Usage: nodejs main.js <infile> <outfile>')
	let parser = new Parser(argv[argc-2])
	let tree = parser.parse()

	let pretty = new Pretty(argv[argc-1])
	pretty.pp(tree)
}

main()