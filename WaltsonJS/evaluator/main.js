var {Parser} = require('./Evaluator')
var {Environment} = require('./Evaluator')
var {Evaluator} = require('./Evaluator')

function main() {
	let argv = process.argv
	let argc = argv.length
	if (argc != 3)
		throw Error('Usage: nodejs main.js <infile>')
	let parser = new Parser(argv[argc-1])
	let tree = parser.parse()

	let global = new Environment()

	let evaluator = new Evaluator()
	evaluator.eval(tree, global)

	Environment.prototype.displayEnvironments(global)
}

main()