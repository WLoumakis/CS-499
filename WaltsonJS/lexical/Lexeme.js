import Type from './Type'

export default class Lexeme extends Type {

	constructor(type, val, line, left, right) {
		this.type = type;
		this.val = val;
		this.line = line;
		this.left = left;
		this.right = right;
		return this;
	}

	get type() {
		return this.type;
	}

	get val() {
		return this.val;
	}

	get line() {
		return this.line;
	}

	get left() {
		return this.left;
	}

	get right() {
		return this.right;
	}

	set type(type) {
		let temp = this.type;
		this.type = type;
		return temp;
	}

	set val(val) {
		let temp = this.val;
		this.val = val;
		return temp;
	}

	set line(line) {
		let temp = this.line;
		this.line = line;
		return temp;
	}

	set left(left) {
		let temp = this.left;
		this.left = left;
		return temp;
	}

	set right(right) {
		let temp = this.right;
		this.right = right;
		return temp;
	}

	static cons(type, left, right) {
		return new Lexeme(type, undefined, undefined, left, right);
	}

}