iam_apikey = "K4V31w-LlqPmAalrBF63xQ9Ty0yF7o330TxkpxeesRqB"
[
	name = "#20q",
	[
		"20q",
		"20 questions",
		"twenty questions"
	]
]

[
	name = "20 questions",
	cond = "(#20q || #30_questions) && @sys-time",
	"http://imagebank.com",
	"Is the thing you're thinking of a person?",
	answers = [
		"#yes",
		"#no"
	],
	[
		name = "Is the person \"youre\" thinking about an actoractress",
		answer = "#yes",
		"Is the person you're thinking about an actor/actress?",
		answers = [
			"#yes",
			"#no"
		],
		[
			name = "Is this person male",
			conditions = "$yesActorActress",
			dialog_node = "yesActorActress",
			output = [
				text = "Is this person male?"
			],
			{
				conditions: "$noActorActress",
				dialog_node: "noActorActress",
				output: {
					text: "Is the person you're thinking of an author?"
				}
			}
		],
		{
			conditions: "$noPerson",
			dialog_node: "noPerson",
			output: {
				text: "Is the thing you're thinking of an animal?"
			},
			children: [
				{
					conditions: "$yesAnimal",
					dialog_node: "yesAnimal",
					output: {
						text: "Is the animal you're thinking of a dog?"
					}
				},
				{
					conditions: "$noAnimal",
					dialog_node: "noAnimal",
					output: {
						text: "Is this thing a location?"
					}
				}
			]
		}
	]
]
