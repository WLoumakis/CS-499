var workspace_id = "67ed01a4-e702-49b7-8af2-b8746d0477f1";
var iam_apikey = "K4V31w-LlqPmAalrBF63xQ9Ty0yF7o330TxkpxeesRqB";
var version = "2019-01-12";
var url = "https://gateway.watsonplatform.net/assistant/api";
var dialog_nodes = [
	{
		conditions: "#20q",
		dialog_node: "20q",
		output: {
			text: "Is the thing you're thinking of a person?"
		}
	},
	{
		conditions: "$yesPerson",
		dialog_node: "yesPerson",
		output: {
			text: "Is the person you're thinking about an actor/actress?"
		},
		parent: "20q"
	},
	{
		conditions: "$yesActorActress",
		dialog_node: "yesActorActress",
		output: {
			text: "Is this person male?"
		},
		parent: "yesPerson"
	},
	{
		conditions: "$noActorActress",
		dialog_node: "noActorActress",
		output: {
			text: "Is the person you're thinking of an author?"
		},
		parent: "yesPerson",
		previous_sibling: "yesActorActress"
	},
	{
		conditions: "$noPerson",
		dialog_node: "noPerson",
		output: {
			text: "Is the thing you're thinking of an animal?"
		},
		parent: "20q",
		previous_sibling: "yesPerson"
	},
	{
		conditions: "$yesAnimal",
		dialog_node: "yesAnimal",
		output: {
			text: "Is the animal you're thinking of a dog?"
		},
		parent: "noPerson"
	},
	{
		conditions: "$noAnimal",
		dialog_node: "noAnimal",
		output: {
			text: "Is this thing a location?"
		},
		parent: "noPerson",
		previous_sibling: "yesAnimal"
	}
];
