var workspace_id = "67ed01a4-e702-49b7-8af2-b8746d0477f1";
var iam_apikey = "K4V31w-LlqPmAalrBF63xQ9Ty0yF7o330TxkpxeesRqB";
var version = "2019-01-12";
var url = "https://gateway.watsonplatform.net/assistant/api";
var watson = require('watson-developer-cloud');
var assistant = new watson.AssistantV1({
	iam_apikey: iam_apikey,
	version: version,
	url: url
});
var dialog_nodes3 = [
	{
		workspace_id: "67ed01a4-e702-49b7-8af2-b8746d0477f1",
		parent: "yesPerson",
		output: {
			text: "Is this person male?"
		},
		dialog_node: "yesActorActress",
		conditions: "$yesActorActress"
	},
	{
		workspace_id: "67ed01a4-e702-49b7-8af2-b8746d0477f1",
		parent: "yesPerson",
		output: {
			text: "Is the person you're thinking of an author?"
		},
		dialog_node: "noActorActress",
		conditions: "$noActorActress"
	},
	{
		workspace_id: "67ed01a4-e702-49b7-8af2-b8746d0477f1",
		parent: "noPerson",
		output: {
			text: "Is the animal you're thinking of a dog?"
		},
		dialog_node: "yesAnimal",
		conditions: "$yesAnimal"
	},
	{
		workspace_id: "67ed01a4-e702-49b7-8af2-b8746d0477f1",
		parent: "noPerson",
		output: {
			text: "Is this thing a location?"
		},
		dialog_node: "noAnimal",
		conditions: "$noAnimal"
	}
];
var dialog_nodes2 = [
	{
		workspace_id: "67ed01a4-e702-49b7-8af2-b8746d0477f1",
		parent: "20q",
		output: {
			text: "Is the person you're thinking about an actor/actress?"
		},
		dialog_node: "yesPerson",
		conditions: "$yesPerson"
	},
	{
		workspace_id: "67ed01a4-e702-49b7-8af2-b8746d0477f1",
		parent: "20q",
		output: {
			text: "Is the thing you're thinking of an animal?"
		},
		dialog_node: "noPerson",
		conditions: "$noPerson"
	}
];
var dialog_nodes1 = [
	{
		workspace_id: "67ed01a4-e702-49b7-8af2-b8746d0477f1",
		output: {
			text: "Is the thing you're thinking of a person?"
		},
		dialog_node: "20q",
		conditions: "#20q"
	}
];
var dialog_nodes = [
	dialog_nodes1,
	dialog_nodes2,
	dialog_nodes3
];
/*
for (var i = 0; i < dialog_nodes.length; i++) {
	assistant.createDialogNode(dialog_nodes[i], async function (err, response) {
		try {
			if (err)
				await console.error(err);
			else
				await console.log(JSON.stringify(response, null, 2));
		} catch(error) {console.error(error);}
	});
}
*/
/*
for (var i = 0; i < dialog_nodes.length; i++) {
	function temp (err, response, pos) {
		if (err) {
			console.error(err);
			assistant.createDialogNode(dialog_nodes[pos], temp, pos);
		}
		else
			console.log(JSON.stringify(response, null, 2));
	}
	assistant.createDialogNode(dialog_nodes[i], temp, i);
}
*/
/*
dialog_nodes.forEach(function(node) {
	assistant.createDialogNode(node, function(err, response) {
		if (err)
			console.error(err);
		else
			console.log(JSON.stringify(response, null, 2));
	});
});
*/
/*
function wait (timeout) {
	return new Promise((resolve) => {
		setTimeout(() => {
			resolve()
		}, timeout)
	})
}

async function insertNode(i) {
	try {
		assistant.createDialogNode(dialog_nodes[i], function () { });
	} catch (err) {
		const timeout = Math.pow(2, i) * 100;
		await wait(timeout);
	}
}

for (var i = 0; i < dialog_nodes.length; i++) {
	insertNode(i);
}
*/
/*
for (var i = 0; i < dialog_nodes.length; i++) {
	for (var j = 0; j < dialog_nodes[i].length; j++) {
		assistant.createDialogNode(dialog_nodes[i][j], function (err, response) {
			if (err)
				console.error(err);
			else
				console.log(JSON.stringify(response, null, 2));
		});
	}
}
*/

async function exists(ref) {

	let params = {
		workspace_id: workspace_id
	}

	function callbackWrapper(res) {
		return (err, response) => {
			if (err)
				console.error(err)
			else {
				res = Object.assign({}, response)
			}
		}
	}

	try {
		let res = null
		await assistant.listDialogNodes(params, callbackWrapper(res))

		if (res) {
			let nodes = res.dialog_nodes
			for (let i = 0; i < nodes.length; i++) {
				if (nodes[i].dialog_node == ref)
					return true
			}
			return false
		}
		else
			return false
	} catch(error) {
		console.log(error)
	}

}

function insertHelper(nodes) {

	function callbackWrapper(count) {
		return (err, response) => {
			if (err) {
				console.log('parent: ', nodes[count].parent, '\ttypeof parent: ', typeof nodes[count].parent)
				if (typeof nodes[count].parent == 'string') {
					while(!exists(nodes[count].parent));
					//if (typeof nodes[count].previous_sibling == 'string') {
					//	while (!exists(nodes[count].previous_sibling));
					//}
					assistant.createDialogNode(nodes[count], function (err, response) {
						if (err)
							console.error(err)
						else
							console.log(JSON.stringify(response, null, 2))
					})
				}
			}
			else
				console.log(JSON.stringify(response, null, 2))
		}
	}

	try {
		for (let i = 0; i < nodes.length; i++) {
			assistant.createDialogNode(nodes[i], callbackWrapper(i))
		}
	} catch (error) {
		console.error(error)
	}

}

for (let i = 0; i < dialog_nodes.length; i++) {
	console.log(JSON.stringify(dialog_nodes[i], null, 2))
	insertHelper(dialog_nodes[i])
}