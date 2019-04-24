{
	dialog_nodes: [
		{
			type: "event_handler",
			output: {},
			parent: "slot_yesPerson",
			context: {
				yesPerson: "#yes"
			},
			metadata: {},
			conditions: "#yes",
			event_name: "input",
			dialog_node: "handler_10_1550098827650"
		},
		{
			type: "event_handler",
			output: {},
			parent: "slot_yesPerson",
			metadata: {},
			event_name: "focus",
			dialog_node: "handler_11_1550098827650",
			previous_sibling: "handler_10_1550098827650"
		},
		{
			type: "event_handler",
			parent: "slot_noPerson",
			context: {
				noPerson: "#no"
			},
			conditions: "#no",
			event_name: "input",
			dialog_node: "handler_13_1550099007064"
		},
		{
			type: "event_handler",
			output: {},
			parent: "slot_noPerson",
			event_name: "focus",
			dialog_node: "handler_14_1550099007064",
			previous_sibling: "handler_13_1550099007064"
		},
		{
			type: "event_handler",
			output: {},
			parent: "slot_18_1550099336394",
			context: {
				yesPerson: "#yes"
			},
			metadata: {},
			conditions: "#yes",
			event_name: "input",
			dialog_node: "handler_19_1550099336394"
		},
		{
			type: "event_handler",
			output: {},
			parent: "slot_18_1550099336394",
			event_name: "focus",
			dialog_node: "handler_20_1550099336394",
			previous_sibling: "handler_19_1550099336394"
		},
		{
			type: "event_handler",
			output: {},
			parent: "slot_21_1550099353344",
			context: {
				noPerson: "#no"
			},
			metadata: {},
			conditions: "#no",
			event_name: "input",
			dialog_node: "handler_22_1550099353344"
		},
		{
			type: "event_handler",
			output: {},
			parent: "slot_21_1550099353344",
			event_name: "focus",
			dialog_node: "handler_23_1550099353344",
			previous_sibling: "handler_22_1550099353344"
		},
		{
			type: "event_handler",
			output: {},
			parent: "slot_25_1550099414959",
			context: {
				yesPerson: "#yes"
			},
			metadata: {},
			conditions: "#yes",
			event_name: "input",
			dialog_node: "handler_26_1550099414959"
		},
		{
			type: "event_handler",
			output: {},
			parent: "slot_25_1550099414959",
			event_name: "focus",
			dialog_node: "handler_27_1550099414959",
			previous_sibling: "handler_26_1550099414959"
		},
		{
			type: "event_handler",
			output: {},
			parent: "slot_28_1550099425340",
			context: {
				noPerson: "#no"
			},
			metadata: {},
			conditions: "#no",
			event_name: "input",
			dialog_node: "handler_29_1550099425340"
		},
		{
			type: "event_handler",
			output: {},
			parent: "slot_28_1550099425340",
			event_name: "focus",
			dialog_node: "handler_30_1550099425340",
			previous_sibling: "handler_29_1550099425340"
		},
		{
			type: "event_handler",
			parent: "slot_2_1550180092244",
			context: {
				yesActorActress: "#yes"
			},
			conditions: "#yes",
			event_name: "input",
			dialog_node: "handler_3_1550180092244"
		},
		{
			type: "event_handler",
			output: {},
			parent: "slot_2_1550247971685",
			context: {
				yesAnimal: "#yes"
			},
			metadata: {},
			conditions: "#yes",
			event_name: "input",
			dialog_node: "handler_3_1550247971685"
		},
		{
			type: "event_handler",
			output: {},
			parent: "slot_32_1550099913353",
			context: {
				yesPerson: "#yes"
			},
			metadata: {},
			conditions: "#yes",
			event_name: "input",
			dialog_node: "handler_33_1550099913353"
		},
		{
			type: "event_handler",
			output: {},
			parent: "slot_32_1550099913353",
			event_name: "focus",
			dialog_node: "handler_34_1550099913353",
			previous_sibling: "handler_33_1550099913353"
		},
		{
			type: "event_handler",
			parent: "slot_35_1550099925978",
			context: {
				noPerson: "#no"
			},
			conditions: "#no",
			event_name: "input",
			dialog_node: "handler_36_1550099925979"
		},
		{
			type: "event_handler",
			output: {},
			parent: "slot_35_1550099925978",
			event_name: "focus",
			dialog_node: "handler_37_1550099925979",
			previous_sibling: "handler_36_1550099925979"
		},
		{
			type: "event_handler",
			output: {},
			parent: "slot_2_1550180092244",
			event_name: "focus",
			dialog_node: "handler_4_1550180092244",
			previous_sibling: "handler_3_1550180092244"
		},
		{
			type: "event_handler",
			output: {},
			parent: "slot_2_1550247971685",
			event_name: "focus",
			dialog_node: "handler_4_1550247971685",
			previous_sibling: "handler_3_1550247971685"
		},
		{
			type: "event_handler",
			parent: "slot_5_1550180108963",
			context: {
				noActorActress: "#no"
			},
			conditions: "#no",
			event_name: "input",
			dialog_node: "handler_6_1550180108963"
		},
		{
			type: "event_handler",
			output: {},
			parent: "slot_5_1550247981985",
			context: {
				noAnimal: "#no"
			},
			metadata: {},
			conditions: "#no",
			event_name: "input",
			dialog_node: "handler_6_1550247981985"
		},
		{
			type: "event_handler",
			output: {},
			parent: "slot_5_1550180108963",
			event_name: "focus",
			dialog_node: "handler_7_1550180108963",
			previous_sibling: "handler_6_1550180108963"
		},
		{
			type: "event_handler",
			output: {},
			parent: "slot_5_1550247981985",
			event_name: "focus",
			dialog_node: "handler_7_1550247981985",
			previous_sibling: "handler_6_1550247981985"
		},
		{
			type: "standard",
			title: "Is the thing youre thinking of a person",
			output: {
				text: {
					values: [
						"Is the thing you're thinking of a person?"
					],
					selection_policy: "sequential"
				}
			},
			metadata: {
				fallback: "leave"
			},
			conditions: "#20Q",
			digress_in: "does_not_return",
			dialog_node: "Is the thing youre thinking of a person",
			digress_out: "allow_all"
		},
		{
			type: "standard",
			title: "Is this person an actoractress",
			output: {
				text: {
					values: [
						"Is this person an actor/actress?"
					],
					selection_policy: "sequential"
				}
			},
			parent: "Person Intent Getter",
			metadata: {
				fallback: "leave"
			},
			conditions: "$yesPerson",
			dialog_node: "Is this person an actoractress",
			digress_out: "allow_all"
		},
		{
			type: "standard",
			title: "Is this thing an animal",
			output: {
				text: {
					values: [
						"Is this thing an animal?"
					],
					selection_policy: "sequential"
				}
			},
			parent: "Person Intent Getter",
			metadata: {
				fallback: "leave"
			},
			conditions: "$noPerson",
			dialog_node: "Is this thing an animal",
			digress_out: "allow_all",
			previous_sibling: "Is this person an actoractress"
		},
		{
			type: "standard",
			title: "Is this animal a dog",
			output: {
				text: {
					values: [
						"Is this animal a dog?"
					],
					selection_policy: "sequential"
				}
			},
			parent: "node_1_1550247955644",
			metadata: {},
			conditions: "$yesAnimal",
			dialog_node: "node_10_1550248158702"
		},
		{
			type: "standard",
			title: "Is this thing a location",
			output: {
				text: {
					values: [
						"Is this thing a location?"
					],
					selection_policy: "sequential"
				}
			},
			parent: "node_1_1550247955644",
			metadata: {},
			conditions: "$noAnimal",
			dialog_node: "node_11_1550248270002",
			previous_sibling: "node_10_1550248158702"
		},
		{
			type: "frame",
			title: "Actor/Actress Intent Getter",
			output: {
				text: {
					values: [],
					selection_policy: "sequential"
				}
			},
			parent: "Is this person an actoractress",
			metadata: {
				fallback: "leave"
			},
			next_step: {
				behavior: "skip_user_input"
			},
			conditions: "#yes || #no",
			dialog_node: "node_1_1550180060092",
			digress_out: "allow_all",
			digress_out_slots: "not_allowed"
		},
		{
			type: "frame",
			title: "Animal Intent Getter",
			output: {
				text: {
					values: [],
					selection_policy: "sequential"
				}
			},
			parent: "Is this thing an animal",
			metadata: {
				fallback: "leave"
			},
			next_step: {
				behavior: "skip_user_input"
			},
			conditions: "#yes || #no",
			dialog_node: "node_1_1550247955644",
			digress_out: "allow_all",
			digress_out_slots: "not_allowed"
		},
		{
			type: "standard",
			title: "Is this person a man",
			output: {
				text: {
					values: [
						"Is this person a man?"
					],
					selection_policy: "sequential"
				}
			},
			parent: "node_1_1550180060092",
			metadata: {},
			conditions: "$yesActorActress",
			dialog_node: "node_8_1550247996787"
		},
		{
			type: "standard",
			title: "Is this person an author",
			output: {
				text: {
					values: [
						"Is this person an author?"
					],
					selection_policy: "sequential"
				}
			},
			parent: "node_1_1550180060092",
			metadata: {},
			conditions: "$noActorActress",
			dialog_node: "node_9_1550248083383",
			previous_sibling: "node_8_1550247996787"
		},
		{
			type: "frame",
			title: "Person Intent Getter",
			output: {
				text: {
					values: [],
					selection_policy: "sequential"
				}
			},
			parent: "Is the thing youre thinking of a person",
			metadata: {
				fallback: "leave"
			},
			next_step: {
				behavior: "skip_user_input"
			},
			conditions: "#yes || #no",
			dialog_node: "Person Intent Getter",
			digress_out: "allow_all",
			digress_out_slots: "not_allowed"
		},
		{
			type: "slot",
			output: {},
			parent: "Is this person an actoractress",
			metadata: {},
			variable: "$yesPerson",
			dialog_node: "slot_18_1550099336394",
			previous_sibling: "node_1_1550180060092"
		},
		{
			type: "slot",
			output: {},
			parent: "Is this person an actoractress",
			metadata: {},
			variable: "$noPerson",
			dialog_node: "slot_21_1550099353344",
			previous_sibling: "slot_18_1550099336394"
		},
		{
			type: "slot",
			output: {},
			parent: "node_1_1550180060092",
			metadata: {},
			variable: "$yesActorActress",
			dialog_node: "slot_2_1550180092244",
			previous_sibling: "node_9_1550248083383"
		},
		{
			type: "slot",
			output: {},
			parent: "node_1_1550247955644",
			metadata: {},
			variable: "$yesAnimal",
			dialog_node: "slot_2_1550247971685",
			previous_sibling: "node_11_1550248270002"
		},
		{
			type: "slot",
			output: {},
			parent: "Is this thing an animal",
			metadata: {},
			variable: "$yesPerson",
			dialog_node: "slot_25_1550099414959",
			previous_sibling: "node_1_1550247955644"
		},
		{
			type: "slot",
			output: {},
			parent: "Is this thing an animal",
			metadata: {},
			variable: "$noPerson",
			dialog_node: "slot_28_1550099425340",
			previous_sibling: "slot_25_1550099414959"
		},
		{
			type: "slot",
			output: {},
			parent: "Person Intent Getter",
			metadata: {},
			variable: "$yesPerson",
			dialog_node: "slot_32_1550099913353",
			previous_sibling: "Is this thing an animal"
		},
		{
			type: "slot",
			output: {},
			parent: "Person Intent Getter",
			metadata: {},
			variable: "$noPerson",
			dialog_node: "slot_35_1550099925978",
			previous_sibling: "slot_32_1550099913353"
		},
		{
			type: "slot",
			output: {},
			parent: "node_1_1550180060092",
			metadata: {},
			variable: "$noActorActress",
			dialog_node: "slot_5_1550180108963",
			previous_sibling: "slot_2_1550180092244"
		},
		{
			type: "slot",
			output: {},
			parent: "node_1_1550247955644",
			metadata: {},
			variable: "$noAnimal",
			dialog_node: "slot_5_1550247981985",
			previous_sibling: "slot_2_1550247971685"
		},
		{
			type: "slot",
			title: "slot_noPerson",
			output: {},
			parent: "Is the thing youre thinking of a person",
			metadata: {},
			variable: "$noPerson",
			dialog_node: "slot_noPerson",
			previous_sibling: "slot_yesPerson"
		},
		{
			type: "slot",
			title: "slot_yesPerson",
			output: {},
			parent: "Is the thing youre thinking of a person",
			metadata: {},
			variable: "$yesPerson",
			dialog_node: "slot_yesPerson",
			previous_sibling: "Person Intent Getter"
		}
	],
	pagination: {
		refresh_url: "/v1/workspaces/31e66674-a3cf-4233-be75-15a2c457b105/dialog_nodes?version=2019-01-12"
	}
}
{
	entities: [],
	pagination: {
		refresh_url: "/v1/workspaces/31e66674-a3cf-4233-be75-15a2c457b105/entities?version=2019-01-12"
	}
}
{
	intents: [
		{
			intent: "20Q"
		},
		{
			intent: "no"
		},
		{
			intent: "yes"
		}
	],
	pagination: {
		refresh_url: "/v1/workspaces/31e66674-a3cf-4233-be75-15a2c457b105/intents?version=2019-01-12"
	}
}
