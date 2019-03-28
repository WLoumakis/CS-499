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

/* Intents */
var intents = [
	{
		intent: '20Q',
		examples: [
			{
				text: '20q'
			},
			{
				text: '20 q'
			},
			{
				text: '20 questions'
			},
			{
				text: 'I\'d like to play 20 questions'
			},
			{
				text: 'twenty q'
			},
			{
				text: 'twenty questions'
			}
		]
	},
	{
		intent: 'no',
		examples: [
			{
				text: 'false'
			},
			{
				text: 'n'
			},
			{
				text: 'nay'
			},
			{
				text: 'no'
			}
		]
	},
	{
		intent: 'yes',
		examples: [
			{
				text: 'aye'
			},
			{
				text: 'true'
			},
			{
				text: 'y'
			},
			{
				text: 'yes'
			}
		]
	}
];

function createIntents() {
	for (var i = 0; i < intents.length; i++) {
		intents[i]['workspace_id'] = workspace_id;
		assistant.createIntent(intents[i], callback);
	}
}

/* Entities */
var entities = [];

function createEntities() {
	for (var i = 0; i < entities.length; i++) {
		entities[i]['workspace_id'] = workspace_id;
		assistant.createEntity(entities[i], callback);
	}
}

/* Dialog Nodes */
var level0 = [
	{
		node_type: 'standard',
		title: 'Is the thing youre thinking of a person',
		output: {
			text: {
				values: [
					'Is the thing youre thinking of a person?'
				],
				selection_policy: 'sequential'
			}
		},
		metadata: {
			fallback: 'leave'
		},
		conditions: '#20Q',
		digress_in: 'does_not_return',
		dialog_node: 'Is the thing youre thinking of a person',
		digress_out: 'allow_all'
	}
];

var level1 = [
	{
		node_type: 'slot',
		title: 'slot_noPerson',
		output: {},
		parent: 'Is the thing youre thinking of a person',
		metadata: {},
		variable: '$noPerson',
		dialog_node: 'slot_noPerson',
		//previous_sibling: 'slot_yesPerson'
	},
	{
		node_type: 'slot',
		title: 'slot_yesPerson',
		output: {},
		parent: 'Is the thing youre thinking of a person',
		metadata: {},
		variable: '$yesPerson',
		dialog_node: 'slot_yesPerson',
		//previous_sibling: 'Person Intent Getter'
	},
	{
		node_type: 'frame',
		title: 'Person Intent Getter',
		output: {
			text: {
			values: [],
			selection_policy: 'sequential'
			}
		},
		parent: 'Is the thing youre thinking of a person',
		metadata: {
			fallback: 'leave'
		},
		next_step: {
			behavior: 'skip_user_input'
		},
		conditions: '#yes || #no',
		dialog_node: 'Person Intent Getter',
		digress_out: 'allow_all',
		digress_out_slots: 'not_allowed'
	}
];

var level2 = [
	{
		node_type: 'slot',
		output: {},
		parent: 'Person Intent Getter',
		metadata: {},
		variable: '$noPerson',
		dialog_node: 'slot_35_1550099925978',
		//previous_sibling: 'slot_32_1550099913353'
	},
	{
		node_type: 'slot',
		output: {},
		parent: 'Person Intent Getter',
		metadata: {},
		variable: '$yesPerson',
		dialog_node: 'slot_32_1550099913353',
		//previous_sibling: 'Is this thing an animal'
	},
	{
		node_type: 'standard',
		title: 'Is this thing an animal',
		output: {
			text: {
				values: [
					'Is this thing an animal?'
				],
				selection_policy: 'sequential'
			}
		},
		parent: 'Person Intent Getter',
		metadata: {
			fallback: 'leave'
		},
		conditions: '$noPerson',
		dialog_node: 'Is this thing an animal',
		digress_out: 'allow_all',
		//previous_sibling: 'Is this person an actoractress'
	},
	{
		node_type: 'standard',
		title: 'Is this person an actoractress',
		output: {
			text: {
				values: [
					'Is this person an actor/actress?'
				],
				selection_policy: 'sequential'
			}
		},
		parent: 'Person Intent Getter',
		metadata: {
			fallback: 'leave'
		},
		conditions: '$yesPerson',
		dialog_node: 'Is this person an actoractress',
		digress_out: 'allow_all'
	},
	
	{
		node_type: 'event_handler',
		output: {},
		parent: 'slot_yesPerson',
		metadata: {},
		event_name: 'focus',
		dialog_node: 'handler_11_1550098827650',
		//previous_sibling: 'handler_10_1550098827650'
	},
	{
		node_type: 'event_handler',
		output: {},
		parent: 'slot_yesPerson',
		context: {
			'yesPerson': '#yes'
		},
		metadata: {},
		conditions: '#yes',
		event_name: 'input',
		dialog_node: 'handler_10_1550098827650'
	},
	{
		node_type: 'event_handler',
		output: {},
		parent: 'slot_noPerson',
		event_name: 'focus',
		dialog_node: 'handler_14_1550099007064',
		//previous_sibling: 'handler_13_1550099007064'
	},
	{
		node_type: 'event_handler',
		parent: 'slot_noPerson',
		context: {
			'noPerson': '#no'
		},
		conditions: '#no',
		event_name: 'input',
		dialog_node: 'handler_13_1550099007064'
	},
];

var level3 = [
	{
		node_type: 'event_handler',
		output: {},
		parent: 'slot_35_1550099925978',
		event_name: 'focus',
		dialog_node: 'handler_37_1550099925979',
		//previous_sibling: 'handler_36_1550099925979'
	},
	{
		node_type: 'event_handler',
		parent: 'slot_35_1550099925978',
		context: {
			'noPerson': '#no'
		},
		conditions: '#no',
		event_name: 'input',
		dialog_node: 'handler_36_1550099925979'
	},
	{
		node_type: 'event_handler',
		output: {},
		parent: 'slot_32_1550099913353',
		event_name: 'focus',
		dialog_node: 'handler_34_1550099913353',
		//previous_sibling: 'handler_33_1550099913353'
	},
	{
		node_type: 'event_handler',
		output: {},
		parent: 'slot_32_1550099913353',
		context: {
			'yesPerson': '#yes'
		},
		metadata: {},
		conditions: '#yes',
		event_name: 'input',
		dialog_node: 'handler_33_1550099913353'
	},
	{
		node_type: 'slot',
		output: {},
		parent: 'Is this thing an animal',
		metadata: {},
		variable: '$noPerson',
		dialog_node: 'slot_28_1550099425340',
		//previous_sibling: 'slot_25_1550099414959'
	},
	{
		node_type: 'slot',
		output: {},
		parent: 'Is this thing an animal',
		metadata: {},
		variable: '$yesPerson',
		dialog_node: 'slot_25_1550099414959',
		//previous_sibling: 'node_1_1550247955644'
	},
	{
		node_type: 'frame',
		title: 'Animal Intent Getter',
		output: {
			text: {
			values: [],
			selection_policy: 'sequential'
			}
		},
		parent: 'Is this thing an animal',
		metadata: {
			fallback: 'leave'
		},
		next_step: {
			behavior: 'skip_user_input'
		},
		conditions: '#yes || #no',
		dialog_node: 'node_1_1550247955644',
		digress_out: 'allow_all',
		digress_out_slots: 'not_allowed'
	},
	{
		node_type: 'slot',
		output: {},
		parent: 'Is this person an actoractress',
		metadata: {},
		variable: '$noPerson',
		dialog_node: 'slot_21_1550099353344',
		//previous_sibling: 'slot_18_1550099336394'
	},
	{
		node_type: 'slot',
		output: {},
		parent: 'Is this person an actoractress',
		metadata: {},
		variable: '$yesPerson',
		dialog_node: 'slot_18_1550099336394',
		//previous_sibling: 'node_1_1550180060092'
	},
	{
		node_type: 'frame',
		title: 'Actor/Actress Intent Getter',
		output: {
			text: {
				values: [],
				selection_policy: 'sequential'
			}
		},
		parent: 'Is this person an actoractress',
		metadata: {
			fallback: 'leave'
		},
		next_step: {
			behavior: 'skip_user_input'
		},
		conditions: '#yes || #no',
		dialog_node: 'node_1_1550180060092',
		digress_out: 'allow_all',
		digress_out_slots: 'not_allowed'
	}
];

var level4 = [
	{
		node_type: 'event_handler',
		output: {},
		parent: 'slot_28_1550099425340',
		event_name: 'focus',
		dialog_node: 'handler_30_1550099425340',
		//previous_sibling: 'handler_29_1550099425340'
	},
	{
		node_type: 'event_handler',
		output: {},
		parent: 'slot_28_1550099425340',
		context: {
			'noPerson': '#no'
		},
		metadata: {},
		conditions: '#no',
		event_name: 'input',
		dialog_node: 'handler_29_1550099425340'
	},
	{
		node_type: 'event_handler',
		output: {},
		parent: 'slot_25_1550099414959',
		event_name: 'focus',
		dialog_node: 'handler_27_1550099414959',
		//previous_sibling: 'handler_26_1550099414959'
	},
	{
		node_type: 'event_handler',
		output: {},
		parent: 'slot_25_1550099414959',
		context: {
			'yesPerson': '#yes'
		},
		metadata: {},
		conditions: '#yes',
		event_name: 'input',
		dialog_node: 'handler_26_1550099414959'
	},
	{
		node_type: 'slot',
		output: {},
		parent: 'node_1_1550247955644',
		metadata: {},
		variable: '$noAnimal',
		dialog_node: 'slot_5_1550247981985',
		//previous_sibling: 'slot_2_1550247971685'
	},
	{
		node_type: 'slot',
		output: {},
		parent: 'node_1_1550247955644',
		metadata: {},
		variable: '$yesAnimal',
		dialog_node: 'slot_2_1550247971685',
		//previous_sibling: 'node_11_1550248270002'
	},
	{
		node_type: 'standard',
		title: 'Is this thing a location',
		output: {
			text: {
				values: [
					'Is this thing a location?'
				],
				selection_policy: 'sequential'
			}
		},
		parent: 'node_1_1550247955644',
		metadata: {},
		conditions: '$noAnimal',
		dialog_node: 'node_11_1550248270002',
		//previous_sibling: 'node_10_1550248158702'
	},
	{
		node_type: 'standard',
		title: 'Is this animal a dog',
		output: {
			text: {
				values: [
					'Is this animal a dog?'
				],
				selection_policy: 'sequential'
			}
		},
		parent: 'node_1_1550247955644',
		metadata: {},
		conditions: '$yesAnimal',
		dialog_node: 'node_10_1550248158702'
	},
	{
		node_type: 'event_handler',
		output: {},
		parent: 'slot_18_1550099336394',
		event_name: 'focus',
		dialog_node: 'handler_20_1550099336394',
		//previous_sibling: 'handler_19_1550099336394'
	},
	{
		node_type: 'event_handler',
		output: {},
		parent: 'slot_18_1550099336394',
		context: {
			'yesPerson': '#yes'
		},
		metadata: {},
		conditions: '#yes',
		event_name: 'input',
		dialog_node: 'handler_19_1550099336394'
	},
	{
		node_type: 'slot',
		output: {},
		parent: 'node_1_1550180060092',
		metadata: {},
		variable: '$noActorActress',
		dialog_node: 'slot_5_1550180108963',
		//previous_sibling: 'slot_2_1550180092244'
	},
	{
		node_type: 'slot',
		output: {},
		parent: 'node_1_1550180060092',
		metadata: {},
		variable: '$yesActorActress',
		dialog_node: 'slot_2_1550180092244',
		//previous_sibling: 'node_9_1550248083383'
	},
	{
		node_type: 'standard',
		title: 'Is this person an author',
		output: {
			text: {
			values: [
				'Is this person an author?'
			],
			selection_policy: 'sequential'
			}
		},
		parent: 'node_1_1550180060092',
		metadata: {},
		conditions: '$noActorActress',
		dialog_node: 'node_9_1550248083383',
		//previous_sibling: 'node_8_1550247996787'
	},
	{
		node_type: 'standard',
		title: 'Is this person a man',
		output: {
			text: {
			values: [
				'Is this person a man?'
			],
			selection_policy: 'sequential'
			}
		},
		parent: 'node_1_1550180060092',
		metadata: {},
		conditions: '$yesActorActress',
		dialog_node: 'node_8_1550247996787'
	}
];

var level5 = [
	{
		node_type: 'event_handler',
		output: {},
		parent: 'slot_5_1550247981985',
		event_name: 'focus',
		dialog_node: 'handler_7_1550247981985',
		//previous_sibling: 'handler_6_1550247981985'
	},
	{
		node_type: 'event_handler',
		output: {},
		parent: 'slot_5_1550247981985',
		context: {
			'noAnimal': '#no'
		},
		metadata: {},
		conditions: '#no',
		event_name: 'input',
		dialog_node: 'handler_6_1550247981985'
	},
	{
		node_type: 'event_handler',
		output: {},
		parent: 'slot_2_1550180092244',
		event_name: 'focus',
		dialog_node: 'handler_4_1550180092244',
		//previous_sibling: 'handler_3_1550180092244'
	},
	{
		node_type: 'event_handler',
		parent: 'slot_2_1550180092244',
		context: {
			'yesActorActress': '#yes'
		},
		conditions: '#yes',
		event_name: 'input',
		dialog_node: 'handler_3_1550180092244'
	},
	{
		node_type: 'event_handler',
		output: {},
		parent: 'slot_5_1550180108963',
		event_name: 'focus',
		dialog_node: 'handler_7_1550180108963',
		//previous_sibling: 'handler_6_1550180108963'
	},
	{
		node_type: 'event_handler',
		parent: 'slot_5_1550180108963',
		context: {
			'noActorActress': '#no'
		},
		conditions: '#no',
		event_name: 'input',
		dialog_node: 'handler_6_1550180108963'
	},
	{
		node_type: 'event_handler',
		output: {},
		parent: 'slot_21_1550099353344',
		event_name: 'focus',
		dialog_node: 'handler_23_1550099353344',
		//previous_sibling: 'handler_22_1550099353344'
	},
	{
		node_type: 'event_handler',
		output: {},
		parent: 'slot_21_1550099353344',
		context: {
			'noPerson': '#no'
		},
		metadata: {},
		conditions: '#no',
		event_name: 'input',
		dialog_node: 'handler_22_1550099353344'
	},
	{
		node_type: 'event_handler',
		output: {},
		parent: 'slot_2_1550247971685',
		event_name: 'focus',
		dialog_node: 'handler_4_1550247971685',
		//previous_sibling: 'handler_3_1550247971685'
	},
	{
		node_type: 'event_handler',
		output: {},
		parent: 'slot_2_1550247971685',
		context: {
			'yesAnimal': '#yes'
		},
		metadata: {},
		conditions: '#yes',
		event_name: 'input',
		dialog_node: 'handler_3_1550247971685'
	}
];

function callback(err, response) {
	if (err)
		console.error(err);
	else
		console.log(JSON.stringify(response, null, 2));
}

var dialog_nodes = [
	level0,
	level1,
	level2,
	level3,
	level4,
	level5
];

/*
async function testFunc() {
	try {
		level0[0]['workspace_id'] = workspace_id;
		await assistant.createDialogNode(level0[0], callback);
		level1[0]['workspace_id'] = workspace_id;
		await assistant.createDialogNode(level1[0], callback);
		level1[1]['workspace_id'] = workspace_id;
		await assistant.createDialogNode(level1[1], callback);
		level1[2]['workspace_id'] = workspace_id;
		await assistant.createDialogNode(level1[2], callback);
	} catch(error) {
		console.error(error);
	}
}
testFunc();
*/


async function createNodesLevel(level) {
	var nodes = dialog_nodes[level];
	try {
		for (var i = 0; i < nodes.length; i++) {
			nodes[i]['workspace_id'] = workspace_id;
			await assistant.createDialogNode(nodes[i], callback);
		}
	} catch (error) {
		console.error(error);
	}
}

createNodesLevel(0);


/*
async function createDialogNodes() {
	try {
		for (var i = 0; i < dialog_nodes.length; i++) {
			await createNodesLevel(i);
		}
	} catch (error) {
		console.error(error);
	}
}

async function createSkill() {
	try {
		await createIntents();
		await createEntities();
		await createDialogNodes();
	} catch (error) {
		console.error(error);
	}
}

createSkill();
*/

