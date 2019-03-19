var watson = require('watson-developer-cloud');

var iam_apikey = 'K4V31w-LlqPmAalrBF63xQ9Ty0yF7o330TxkpxeesRqB';
var version = '2019-01-12';
var url = 'https://gateway.watsonplatform.net/assistant/api';

var assistant = new watson.AssistantV1({
	iam_apikey: iam_apikey,
	version: version,
	url: url
});

var workspace = {
	name: 'test-workspace',
	description: 'Test workspace created through API.'
}

assistant.createWorkspace(workspace, function (err, response) {
	if (err)
		console.error(err);
	else
		console.log(JSON.stringify(response, null, 2));
})