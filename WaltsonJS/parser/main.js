var {Parser} = require('./Parser')

function main() {
	let argv = process.argv
	let argc = argv.length
	let parser = new Parser(argv[argc-1])
	let tree = parser.parse()
	console.log('legal')
}

main()