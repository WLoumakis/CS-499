var AssistantV1 = require('watson-developer-cloud/assistant/v1');

var iam_apikey = "K4V31w-LlqPmAalrBF63xQ9Ty0yF7o330TxkpxeesRqB";
var version = "2019-01-12";
var url = "https://gateway.watsonplatform.net/assistant/api";

var workspace_id = "31e66674-a3cf-4233-be75-15a2c457b105"

var service = new AssistantV1({
    version: version,
    iam_apikey: iam_apikey,
    url: url
})

service.getWorkspace({workspace_id: workspace_id}, function(err, response) {
    if (err)
        console.error(err)
    else
        console.log(JSON.stringify(response, null, 2))
})