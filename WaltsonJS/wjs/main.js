var {Lexeme} = require('./wjs')
var {Type} = require('./wjs')
var {Parser} = require('./wjs')
var {Environment} = require('./wjs')
var {Evaluator} = require('./wjs')
var {wjs} = require('./wjs')
var fs = require('fs')
var rl = require('readline-sync')

function changeExtension(str) {
	let result = ''
	let ind = str.indexOf('.')
	if (ind == -1) {
		result += str
		result += '.js'
		return result
	}
	for (let i = 0; i < ind; i++)
		result += str[i]
	result += '.js'
	return result
}

function warn(outfile) {
	console.error('WARNING: file ' + outfile + ' already exists and will be overwritten.')
	let result = rl.question('Do you wish to continue? [y/n]: ')
	if (result.toLowerCase() == 'y')
		return true
	else if (result.toLowerCase() == 'n')
		return false
	else {
		console.error('Error: token ' + result + ' not understood!')
		return warn(outfile)
	}
}

function enqueueHelper(node) {
	let parent = new Lexeme(Type.ID, 'parent', undefined, undefined, undefined)
	let dialog_node = new Lexeme(Type.ID, 'dialog_node', undefined, undefined, undefined)
	let workspace_id = new Lexeme(Type.ID, 'workspace_id', undefined, undefined, undefined)
	let children = new Lexeme(Type.ID, 'children', undefined, undefined, undefined)
	let previous_sibling = new Lexeme(Type.ID, 'previous_sibling', undefined, undefined, undefined)

	let p = Environment.prototype.lookupLocal(node, dialog_node)
	let c = null
	let ps = null

	if (!Environment.prototype.existsLocal(node, workspace_id))
		Environment.prototype.insert(node, workspace_id, workspace_id)

	if (Environment.prototype.existsLocal(node, children))
		c = Environment.prototype.lookupLocal(node, children).getValue()

	let result = []

	let len = 0
	if (c != null)
		len = c.length
		
	for (let i = 0; i < len; i++) {
		if (p != null)
			Environment.prototype.insert(c[i], parent, p)
		if (i != 0 && !Environment.prototype.existsLocal(c[i], previous_sibling))
			Environment.prototype.insert(c[i], previous_sibling, ps)
		result.push(c[i])
		let temp = enqueueHelper(c[i])
		for (let j = 0; j < temp.length; j++)
			result.push(temp[j])
		ps = Environment.prototype.lookupLocal(c[i], dialog_node)
	}

	if (c != null)
		Environment.prototype.delete(node, children)
	
	return result
}

function enqueue(nodes) {
	let previous_sibling = new Lexeme(Type.ID, 'previous_sibling', undefined, undefined, undefined)
	let dialog_node = new Lexeme(Type.ID, 'dialog_node', undefined, undefined, undefined)
	
	let ps = null

	let result = []

	for (let i = 0; i < nodes.length; i++) {
		if (i != 0)
			Environment.prototype.insert(nodes[i], previous_sibling, ps)
		result.push(nodes[i])
		let temp = enqueueHelper(nodes[i])
		for (let j = 0; j < temp.length; j++)
			result.push(temp[j])
		ps = Environment.prototype.lookupLocal(nodes[i], dialog_node)
	}
	return result
}

function translate(infile, outfile) {
	let parser = new Parser(infile)
	let evaluator = new Evaluator()

	let tree = parser.parse()

	let global = new Environment()
	evaluator.eval(tree, global)

	let dialog_nodes = new Lexeme(Type.ID, 'dialog_nodes', undefined, undefined, undefined)
	let nodes = null
	if (Environment.prototype.exists(global, dialog_nodes))
		nodes = Environment.prototype.lookup(global, dialog_nodes).getValue()
	
	if (nodes != null)
		nodes = enqueue(nodes)
	
	if (Environment.prototype.exists(global, dialog_nodes))
		Environment.prototype.update(global, dialog_nodes, new Lexeme(Type.ARRAY, nodes, undefined, undefined, undefined))

	let translator = new wjs(outfile, global)
	
	translator.addVariables()
}

function main() {
	let argv = process.argv
	let argc = argv.length
	if (argc != 3)
		throw Error('Usage: nodejs main.js <infile>')
	let infile = argv[argc-1]
	let outfile = changeExtension(infile)
	try {
		if (fs.existsSync(outfile)) {
			let accepted = warn(outfile)
			if (accepted)
				translate(infile, outfile)
			else
				process.exit(0)
		}
		else {
			translate(infile, outfile)
		}
	} catch (err) {
		console.error(err)
	}
}

main()