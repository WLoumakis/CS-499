var {Lexeme} = require('./newWjs')
var {Type} = require('./newWjs')
var {Parser} = require('./newWjs')
var {Environment} = require('./newWjs')
var {Evaluator} = require('./newWjs')
var {WJS} = require('./newWjs')
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

/*
function enqueueHelper(node) {
	let parent = new Lexeme(Type.ID, 'parent', undefined, undefined, undefined)
	let dialog_node = new Lexeme(Type.ID, 'dialog_node', undefined, undefined, undefined)
	let workspace_id = new Lexeme(Type.ID, 'workspace_id', undefined, undefined, undefined)
	let children = new Lexeme(Type.ID, 'children', undefined, undefined, undefined)
	let previous_sibling = new Lexeme(Type.ID, 'previous_sibling', undefined, undefined, undefined)

	let p = Environment.prototype.lookupLocal(node, dialog_node)
	let c = null
	let ps = null

	//FIXME: I don't think I need this because of two reasons
	//       	1.  If creating a workspace, we don't know the id yet
	//          2.  If we aren't creating, we're updating. Since the
	//              updateWorkspace method takes a workspace, the nodes
	//              don't need a workspace_id attribute.
	if (!Environment.prototype.existsLocal(node, workspace_id) &&
		Environment.prototype.exists(node, workspace_id))
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
*/

function buildNodeObject(array, ind) {
	let title = new Lexeme(Type.ID, 'title', undefined, undefined, undefined)
	let dialog_node = new Lexeme(Type.ID, 'dialog_node', undefined, undefined, undefined)
	let conditions = new Lexeme(Type.ID, 'conditions', undefined, undefined, undefined)

	let node = new Environment()
	Environment.prototype.insert(node, dialog_node, array[ind].car())
	Environment.prototype.insert(node, title, array[ind].car())
	ind++
	
	// While not at the end of the array, and while we don't have
	// an explicitly defined "name" attribute. . .
	while (ind < array.length &&
			(array[ind].getType() != Type.STRING ||
				(array[ind].getType() == Type.STRING &&
				array[ind].getValue() != 'name'))) {
					if (array[ind].getType() == Type.EXPLICIT) {
						Environment.prototype.insert(node, array[ind].car(), array[ind].cdr())
					}
					else {

					}
				}
}

/*
function enqueue(nodes) {
	let previous_sibling = new Lexeme(Type.ID, 'previous_sibling', undefined, undefined, undefined)
	let dialog_node = new Lexeme(Type.ID, 'dialog_node', undefined, undefined, undefined)
	
	let ps = null

	let result = []

	for (let i = 0; i < nodes.length; i++) {
		if (nodes[i].getType() == Type.EXPLICIT && nodes[i].car().getValue() == 'name') {
			let result = buildNodeObject(nodes, i)
			let node = result.node
			let ind = result.ind
			if (i != 0)
				Environment.prototype.insert(node, previous_sibling, ps)
			result.push(node)
			let temp = enqueueHelper(node)
			for (let j = 0; j < temp.length; j++)
				result.push(temp[j])
			ps = Environment.prototype.lookupLocal(node, dialog_node)
			i = ind
		}
		else {
			if (i != 0)
				Environment.prototype.insert(nodes[i], previous_sibling, ps)
			result.push(nodes[i])
			let temp = enqueueHelper(nodes[i])
			for (let j = 0; j < temp.length; j++)
				result.push(temp[j])
			ps = Environment.prototype.lookupLocal(nodes[i], dialog_node)
		}
		
	}
	return result
}
*/

function setImplicitNodeAttribute(variable, value) {
	if (variable.getValue() == 'unbound') {
		switch (value.getType()) {
			case Type.STRING:
				return variable.setValue('text')
			case Type.URL:
				return variable.setValue('image')
			case Type.NUMBER:
				return variable.setValue('pause')
			case Type.ENVIRONMENT:
				// If we have an object, then it is an option attribute
				if (value.getValue() == Type.OBJECT)
					return variable.setValue('option')
				// Otherwise we have a children attribute
				return variable.setValue('children')
			default:
				throw Error('Error trying to implicitly define a dialog node attribute with type ', value.getType() + '!')
		}
	}
}

function buildOutputSkeleton() {
	let output_val = new Environment()
	output_val.setValue(Type.OBJECT)

	let generic = new Lexeme(Type.ID, 'generic', undefined, undefined, undefined)
	let generic_val = Environment.prototype.extend(output_val, Type.ARRAY, null, null)
	Environment.prototype.insert(output_val, generic, generic_val)

	return output_val
}

function enqueueHelper(env) {
	let parent = new Lexeme(Type.ID, 'parent', undefined, undefined, undefined)
	let dialog_node = new Lexeme(Type.ID, 'dialog_node', undefined, undefined, undefined)
	let children = new Lexeme(Type.ID, 'children', undefined, undefined, undefined)
	let previous_sibling = new Lexeme(Type.ID, 'previous_sibling', undefined, undefined, undefined)

	// Get the parent reference
	let parent_val = Environment.prototype.lookupLocal(node, dialog_node)
	let children_val = null
	let previous_sibling_val = null

	if (Environment.prototype.existsLocal(env, children))
		children_val = lookupLocal(env, children)
	
	let result = []
	let table = env.car()
	let vars = table.car()
	let vals = table.cdr()
	while (vars != null) {
		// If we have an implicit definition, it is expected to be output or children.
		if (vars.car().getValue() == 'unbound') {
			setImplicitNodeAttribute(vars.car(), vals.car())

			// If we are inserting something other than children, it is expected to be output.
			if (vals.car().getType() != Type.ENVIRONMENT ||
					(vals.car().getType() == Type.ENVIRONMENT &&
					vals.car().getValue() != Type.ARRAY)) {
						// Check to see if the output attribute is already in the environment
						if (!Environment.prototype.existsLocal(node, out))
							Environment.prototype.insert(node, out, output)
						
						// Insert the new value into the output object
						Environment.prototype.insert(output, vars.car(), vals.car())
			}
			// Otherwise just insert the attribute
			else
				Environment.prototype.insert(node, vars.car(), vals.car())
		}

		// Else if we have an object, then it is expected to be a node object.
		else if (vals.car().getType() == Type.ENVIRONMENT && vals.car().getValue() == Type.OBJECT) {

			// If parent doesn't exist in the child, insert it
			if (Environment.prototype.existsLocal(vals.car(), parent))
				Environment.prototype.insert(vals.car(), parent, parent_val)

			// If previous_sibling doesn't exist in the environment, insert it
			if (vars != table.car() && !Environment.prototype.existsLocal(vals.car(), previous_sibling))
				Environment.prototype.insert(vals.car(), previous_sibling, previous_sibling_val)

			result.push(vals.car())
			let arrayOfChildren = enqueueHelper(vals.car())

			for (let i = 0; i < arrayOfChildren.length; i++)
				result.push(arrayOfChildren[i])
			ps = Environment.prototype.lookupLocal(vals.car(), dialog_node)
		}

		// Otherwise, we have implicit definitions of nodes,
		// so we have to build node objects.
		else {
			let node = new Environment()
			node.setValue(Type.OBJECT)
			let title = new Lexeme(Type.ID, 'title', undefined, undefined, undefined)
			let dialog_node = new Lexeme(Type.ID, 'dialog_node', undefined, undefined, undefined)
			let name = vals.car()

			let out = new Lexeme(Type.ID, 'output', undefined, undefined, undefined)
			let output = buildOutputSkeleton()

			// Insert the most important attributes
			Environment.prototype.insert(node, title, name)
			Environment.prototype.insert(node, dialog_node, name)

			// Skip to the next attribute
			vars = vars.cdr()
			vals = vals.cdr()

			// Iterate until we hit the next implicit node
			while (vars != null && vars.car().getValue() != 'name') {

				// If we have an implicit definition, it is expected to be output or children.
				if (vars.car().getValue() == 'unbound') {
					setImplicitNodeAttribute(vars.car(), vals.car())

					// If we aren't inserting children. . .
					if (vals.car().getType() != Type.ENVIRONMENT ||
							(vals.car().getType() == Type.ENVIRONMENT &&
							vals.car().getValue() != Type.ARRAY)) {
								// Check to see if the output attribute is already in the environment
								if (!Environment.prototype.existsLocal(node, out))
									Environment.prototype.insert(node, out, output)
								
								// Insert the new value into the output object
								Environment.prototype.insert(output, vars.car(), vals.car())
					}
					// Otherwise just insert the attribute
					else
						Environment.prototype.insert(node, vars.car(), vals.car())
				}

				// Otherwise, it's an explicit definition and can be inserted.
				else {
					// If the attribute already exists, simply update it.
					if (Environment.prototype.existsLocal(node, result.id))
						Environment.prototype.update(node, vars.car(), vals.car())

					// Otherwise, just insert.
					else
						Environment.prototype.insert(node, vars.car(), vals.car())
				}

				vars = vars.cdr()
				vals = vals.cdr()
			}

			// So long as we aren't the first node, insert the previous_sibling attribute
			if (vars != table.car())
				Environment.prototype.insert(node, previous_sibling, ps)
			
			result.push(node)
			let arrayOfChildren = enqueueHelper(node)
			for (let i = 0; i < arrayOfChildren.length; i++)
				result.push(arrayOfChildren[i])

			/**
			 * We continue here because there exist two scenarios:
			 * 		1. vars == null
			 * 		2. vars.car().getValue() == 'name'
			 * 	In the first case, we do not wish to advance vars since
			 * that will throw an error.
			 * 	In the second case, we have another implicit node, so we
			 * don't want to skip over the name attribute.
			 */
			continue
		}
		
		vars = vars.cdr()
		vals = vals.cdr()
	}
	return result
}

function enqueue(env) {
	let previous_sibling = new Lexeme(Type.ID, 'previous_sibling', undefined, undefined, undefined)
	let dialog_node = new Lexeme(Type.ID, 'dialog_node', undefined, undefined, undefined)
	
	let previous_sibling_val = null

	let result = []

	let table = env.car()
	let vars = table.car()
	let vals = table.cdr()
	while (vars != null) {
		// If we have an object, then it is expected to be a node object.
		if (vals.car().getType() == Type.ENVIRONMENT && vals.car().getValue() == Type.OBJECT) {
			if (vars != table.car())
				Environment.prototype.insert(vals.car(), previous_sibling, previous_sibling_val)
			result.push(vals.car())
			let arrayOfChildren = enqueueHelper(vals.car())
			for (let i = 0; i < arrayOfChildren.length; i++)
				result.push(arrayOfChildren[i])
			ps = Environment.prototype.lookupLocal(vals.car(), dialog_node)
		}

		// Otherwise, we have implicit definitions of nodes,
		// so we have to build node objects.
		else {
			let node = new Environment()
			node.setValue(Type.OBJECT)
			let title = new Lexeme(Type.ID, 'title', undefined, undefined, undefined)
			let dialog_node = new Lexeme(Type.ID, 'dialog_node', undefined, undefined, undefined)
			let name = vals.car()

			let out = new Lexeme(Type.ID, 'output', undefined, undefined, undefined)
			let output = buildOutputSkeleton()

			// Insert the most important attributes
			Environment.prototype.insert(node, title, name)
			Environment.prototype.insert(node, dialog_node, name)

			// Keep up with the last var and val
			let last_vars = vars
			let last_vals = vals

			// Skip to the next attribute
			vars = vars.cdr()
			vals = vals.cdr()

			// Iterate until we hit the next implicit node
			while (vars != null && vars.car().getValue() != 'name') {

				// If we have an implicit definition, it is expected to be output or children.
				if (vars.car().getValue() == 'unbound') {
					setImplicitNodeAttribute(vars.car(), vals.car())

					// If we aren't inserting children. . .
					if (vals.car().getType() != Type.ENVIRONMENT ||
							(vals.car().getType() == Type.ENVIRONMENT &&
							vals.car().getValue() != Type.ARRAY)) {
								// Check to see if the output attribute is already in the environment
								if (!Environment.prototype.existsLocal(node, out))
									Environment.prototype.insert(node, out, output)
								
								// Insert the new value into the output object
								Environment.prototype.insert(output, vars.car(), vals.car())
					}
					// Otherwise just insert the attribute
					else
						Environment.prototype.insert(node, vars.car(), vals.car())
				}

				// Otherwise, it's an explicit definition and can be inserted.
				else {
					// If the attribute already exists, simply update it.
					if (Environment.prototype.existsLocal(node, result.id))
						Environment.prototype.update(node, vars.car(), vals.car())

					// Otherwise, just insert.
					else
						Environment.prototype.insert(node, vars.car(), vals.car())
				}

				last_vars = vars
				last_vals = vals
				vars = vars.cdr()
				vals = vals.cdr()
			}

			// So long as we aren't the first node, insert the previous_sibling attribute
			if (vars != table.car())
				Environment.prototype.insert(node, previous_sibling, ps)

			result.push(node)
			let arrayOfChildren = enqueueHelper(node)
			for (let i = 0; i < arrayOfChildren.length; i++)
				result.push(arrayOfChildren[i])

			/**
			 * We continue here because there exist two scenarios:
			 * 		1. vars == null
			 * 		2. vars.car().getValue() == 'name'
			 * 	In the first case, we do not wish to advance vars since
			 * that will throw an error.
			 * 	In the second case, we have another implicit node, so we
			 * don't want to skip over the name attribute.
			 */
			continue
		}
		
		vars = vars.cdr()
		vals = vals.cdr()
	}
	return result
}

function translate(infile, outfile) {
	let parser = new Parser(infile)
	let evaluator = new Evaluator()

	let tree = parser.parse()

	let global = new Environment()
	evaluator.eval(tree, global)

	let wjs = new WJS(outfile, global)

	wjs.performChecksAndSets()

	let dialog_nodes = new Lexeme(Type.ID, 'dialog_nodes', undefined, undefined, undefined)
	let nodes = null
	
	if (Environment.prototype.exists(global, dialog_nodes))
		nodes = Environment.prototype.lookup(global, dialog_nodes)
	
	if (nodes != null)
		nodes = enqueue(nodes)
	
	if (Environment.prototype.exists(global, dialog_nodes))
		Environment.prototype.update(global, dialog_nodes, new Lexeme(Type.ARRAY, nodes, undefined, undefined, undefined))
	
	wjs.translate()
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
