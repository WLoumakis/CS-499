var AssistantV1 = require('watson-developer-cloud/assistant/v1');

var iam_apikey = "K4V31w-LlqPmAalrBF63xQ9Ty0yF7o330TxkpxeesRqB";
var version = "2019-01-12";
var url = "https://gateway.watsonplatform.net/assistant/api";

var service = new AssistantV1({
    version: version,
    iam_apikey: iam_apikey,
    url: url
})

service.listWorkspaces()
.then(res => {
    console.log(JSON.stringify(res, null, 2))
})
.catch(err => {
    console.error(err)
})