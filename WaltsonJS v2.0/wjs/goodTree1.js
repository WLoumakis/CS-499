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
var dialog_nodes = [
	{
		workspace_id: "67ed01a4-e702-49b7-8af2-b8746d0477f1",
		output: {
			text: "Is the thing you're thinking of a person?"
		},
		dialog_node: "20q",
		conditions: "#20q"
	},
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
		parent: "yesPerson",
		output: {
			text: "Is this person male?"
		},
		dialog_node: "yesActorActress",
		conditions: "$yesActorActress"
	},
	{
		workspace_id: "67ed01a4-e702-49b7-8af2-b8746d0477f1",
		previous_sibling: "yesActorActress",
		parent: "yesPerson",
		output: {
			text: "Is the person you're thinking of an author?"
		},
		dialog_node: "noActorActress",
		conditions: "$noActorActress"
	},
	{
		workspace_id: "67ed01a4-e702-49b7-8af2-b8746d0477f1",
		previous_sibling: "yesPerson",
		parent: "20q",
		output: {
			text: "Is the thing you're thinking of an animal?"
		},
		dialog_node: "noPerson",
		conditions: "$noPerson"
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
for (var i = 0; i < dialog_nodes.length; i++) {
	assistant.createDialogNode(dialog_nodes[i], function (err, response) {
		if (err)
			console.error(err);
		else
			console.log(JSON.stringify(response, null, 2));
	});
}