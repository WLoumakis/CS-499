var workspace_id = "67ed01a4-e702-49b7-8af2-b8746d0477f1"

var watson = require('watson-developer-cloud')

var params = {
	iam_apikey: "K4V31w-LlqPmAalrBF63xQ9Ty0yF7o330TxkpxeesRqB",
	version: "2019-01-12",
	url: "https://gateway.watsonplatform.net/assistant/api"
}

var assistant = new watson.AssistantV1(params)

assistant.listIntents({workspace_id: workspace_id}, function (err, response) {
	if (err)
		console.error(err)
	else
		console.log(JSON.stringify(response, null, 2))
})