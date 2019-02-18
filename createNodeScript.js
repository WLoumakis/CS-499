var lineReader = require('readline').createInterface({
	input: require('fs').createReadStream('example 20q')
});

var workspace_id = '31e66674-a3cf-4233-be75-15a2c457b105';

/*
var watson = require('watson-developer-cloud');
var assistant = watson.AssistantV1({
	iam_apikey: 'K4V31w-LlqPmAalrBF63xQ9Ty0yF7o330TxkpxeesRqB',
	version: '2019-01-12',
	url: 'https://gateway.watsonplatform.net/assistant/api'
});
*/

const bintree = require('bintree');

var tree = new bintree.NodeTree();
/*
function buildConditions(pos) {
	if (pos == 0)
		return '#yes';
	else if (pos == 1)
		return '#no';
	else
		throw Error("pos \'" + pos + "\' not understood.");
}

function buildParent(parent) {
	var params = parent.getWatsonParams();
	if (params.hasOwnProperty("dialog_node"))
		return params.dialog_node;
	else
		throw Error("Couldn't find dialog_node parameter of ${parent.getVal()}");
}

function buildDialogNode(depth, pos, val) {
	var ret = '(' + depth + ',' + pos + '): ' + val;
	return ret;
}

function buildParams(node) {
	var conditions = buildConditions(node.getPosition());
	var text = node.getVal();
	var parent = buildParent(node.getParent());
	var dialog_node = buildDialogNode(node.getDepth(), node.getPosition(), node.getVal());

	var params = {
		workspace_id: workspace_id,
		dialog_node: dialog_node,
		conditions: conditions,
		output: {
			text: text
		},
		parent = parent
	};
	return params;
}
*/

function read(callback) {
	lineReader.on('line', function (line) {
		var newNode = bintree.TreeNode(line, {});
		newNode = tree.insert(newNode);
		//var params = buildParams(newNode);
		//newNode.setWatsonParams(params); //left off here
		//var parent = newNode.getParent();
		//if (parent)
		//	parent.setWatsonParams(buildParentParams(parent, newNode));
	});
	lineReader.on('close', callback);
}

function display(callback) {
	callback();
}

read(function() {
	display(function () {
		tree.displayInorder();
	});
});