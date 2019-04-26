var watson = require('watson-developer-cloud')

var iam_apikey = "K4V31w-LlqPmAalrBF63xQ9Ty0yF7o330TxkpxeesRqB"
var version = "2019-04-25"
var url = "https://gateway.watsonplatform.net/assistant/api"

var assistant = new watson.AssistantV1({
    iam_apikey: iam_apikey,
    version: version,
    url: url
})

var workspace_id = "31e66674-a3cf-4233-be75-15a2c457b105"

assistant.getWorkspace({workspace_id: workspace_id, export: true}, function(err, response) {
    if (err)
        console.error(err)
    else
        console.log(JSON.stringify(response, null, 2))
})
