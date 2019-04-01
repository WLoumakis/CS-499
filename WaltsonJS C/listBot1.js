var watson = require('watson-developer-cloud');

var iam_apikey = "K4V31w-LlqPmAalrBF63xQ9Ty0yF7o330TxkpxeesRqB";
var version = "2019-01-12";
var url = "https://gateway.watsonplatform.net/assistant/api";

var assistant = new watson.AssistantV1({
	iam_apikey: iam_apikey,
	version: version,
	url: url
});

var workspace_id = 'ad3d7543-0d65-43ae-8ce8-0f81d78ce69b';

var params = {
	workspace_id: workspace_id
};

function callback(err, response) {
	if (err)
		console.error(err);
	else
		console.log(JSON.stringify(response, null, 2));
}

assistant.listIntents(params, callback);

assistant.listEntities(params, callback);

assistant.listDialogNodes(params, callback);