var workspaces = [
	{
		workspace_id: "test-workspace",
		dialog_nodes: [
			{
				conditions: "#20q",
				dialog_node: "20q",
				output: {
					text: "Is the thing you're thinking of a person?"
				},
				children: [
					{
						conditions: "$yesPerson",
						dialog_node: "yesPerson",
						output: {
							text: "Is the person you're thinking about an actor/actress?"
						},
						children: [
							{
								conditions: "$yesActorActress",
								dialog_node: "yesActorActress",
								output: {
									text: "Is this person male?"
								}
							},
							{
								conditions: "$noActorActress",
								dialog_node: "noActorActress",
								output: {
									text: "Is the person you're thinking of an author?"
								}
							}
						]
					},
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
			}
		]
	}
]
