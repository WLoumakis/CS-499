var workspace_id = "67ed01a4-e702-49b7-8af2-b8746d0477f1";
var dialog_nodes = [
	{
		title: "Welcome",
		conditions: "welcome",
		dialog_node: "welcome",
		workspace_id: workspace_id,
		children: [
			{
				title: "Example",
				conditions: "#General_Greetings",
				dialog_node: "example",
				workspace_id: workspace_id
			}
		]
	},
	{
		title: "Anything else",
		conditions: "anything_else",
		dialog_node: "anything_else",
		previous_sibling: "welcome",
		workspace_id: workspace_id
	}
];
var url = "https://gateway.watsonplatform.net/assistant/api";
var version = "2019-01-12";
var iam_apikey = "K4V31w-LlqPmAalrBF63xQ9Ty0yF7o330TxkpxeesRqB";
var watson = require('watson-developer-cloud');
var assistant = new watson.AssistantV1({
	iam_apikey: iam_apikey,
	version: version,
	url: url
});

for (var i = 0, len = dialog_nodes.length; i < len; i++) {
	assistant.createDialogNode(dialog_nodes[i], function (err, response) {
		if (err)
			console.error(err);
		else
			console.log(JSON.stringify(response, null, 2));
	});
}
