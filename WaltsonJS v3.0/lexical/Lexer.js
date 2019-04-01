let fs = require('fs')

export default class Lexer extends Type {

	constructor(filename) {
		this.file = fs.readFileSync(filename, 'utf-8')
		this.position = 0
		this.line = 1
		return this
	}

	static lex() {
		skipWhiteSpace()
		let ch = readChar()
		console.log('position after reading: ', position, '\tfile.length: ', file.length)
		if (position == file.length)
			return new Lexeme(this.END_OF_INPUT, undefined, line, undefined, undefined)
		switch (ch) {
			case '[':
				return new Lexeme(this.OPEN_BRACKET, undefined, line, undefined, undefined)
			case ']':
				return new Lexeme(this.CLOSE_BRACKET, undefined, line, undefined, undefined)
			case '{':
				return new Lexeme(this.OPEN_BRACE, undefined, line, undefined, undefined)
			case '}':
				return new Lexeme(this.CLOSE_BRACE, undefined, line, undefined, undefined)
			case ':':
				return new Lexeme(this.COLON, undefined, line, undefined, undefined)
			case ',':
				return new Lexeme(this.COMMA, undefined, line, undefined, undefined)
			case '-':
				return new Lexeme(this.MINUS, undefined, line, undefined, undefined)
			case '"':
				putBack(ch)
				return lexString()
			case '\d':
				putBack(ch)
				return lexNumber()
			default:
				if (/\w/.test(ch)) {
					putBack(ch)
					return lexVar()
				}
				else {
					throw Error('Error lexing character ' + ch + '!')
				}
		}
	}

	lexString() {
		let ch = readChar()		// Gets first "
		let ret = ""
		while ((ch = readChar()) != '"') {
			ret += ch
		}
		// Throw away the terminating quote by not pushing back

		return new Lexeme(this.STRING, ret, line, undefined, undefined)
	}

	lexNumber() {
		let ch = readChar()
		let real = 0
		let ret = ""
		while (/\d/.test(ch) || ch == '.') {
			if (real && ch == '.')
				throw Error("Error on line " + line + ": number with two decimals!")
			if (ch == '.')
				real = 1
			ret += ch
		}
		putBack(ch)
		return new Lexeme(this.NUMBER, real ? parseFloat(ret) : parseInt(ret), line, undefined, undefined)
	}

	lexVar() {
		let ch = readChar()
		let ret = ""
		let size = 64
		let index = 0
		while (/\w/.test(ch) || ch == '_') {
			if (index == size)
				throw Error("Error on line " + line + ": variable name is too long!")
			ret += ch
			index++
		}
		putBack(ch)
		return new Lexeme(this.ID, ret, line, undefined, undefined)
	}

	skipWhiteSpace() {
		console.log('skipWhiteSpace')
		let ch = readChar()
		while(/\s/.test(ch)) {
			ch = readChar()
		}
		putBack(ch)
	}

	readChar() {
		console.log('readChar')
		let ch = file[position]
		if (ch == '\n')
			line++
		position++
		return ch
	}

	putBack(ch) {
		console.log('putBack')
		if (ch == '\n')
			line--
		position--
	}

}