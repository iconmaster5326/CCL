/*
 * parser.cpp
 *
 *  Created on: Jun 22, 2019
 *      Author: iconmaster
 */

#include "ccl/parser.hpp"
#include "ccl/classes.hpp"

using namespace ccl;
using namespace std;

static void parsePostConst(Context& ctx, Lexer& lexer) {
	Token& t = lexer.peek();
	switch (t.type) {
	case Token::Type::EX_STRING:
	case Token::Type::FLAG:
	case Token::Type::LBRACE:
	case Token::Type::LBRACKET:
	case Token::Type::LPAREN:
	case Token::Type::STRING:
	case Token::Type::VAR:
	case Token::Type::WORD:
		throw Error("Unexpected token after constant: " + t.value);
	}
}

static optional<_ProgramCall::Arg> parseFuncArg(Context& ctx, Lexer& lexer) {
	Token t = lexer.peek();
	switch (t.type) {
	case Token::Type::VAR: {
		lexer.next();
		return optional<_ProgramCall::Arg>(in_place, (Program) ProgramVar(t.value, t.source));
	} break;
	case Token::Type::FLAG: {
		lexer.next();
		switch (lexer.peek().type) {
		case Token::Type::VAR:
		case Token::Type::EX_STRING:
		case Token::Type::STRING:
		case Token::Type::LPAREN:
		case Token::Type::LBRACE:
		case Token::Type::LBRACKET:
		case Token::Type::WORD: {
			auto a = parseFuncArg(ctx, lexer);
			return optional<_ProgramCall::Arg>(in_place, t.value, a->value);
		} break;
		default:
			return optional<_ProgramCall::Arg>(in_place, t.value, ProgramConstant(_ClassBool::TRUE, t.source));
		}
	} break;
	case Token::Type::EX_STRING:
	case Token::Type::STRING:
	case Token::Type::WORD: {
		lexer.next();
		return optional<_ProgramCall::Arg>(in_place, (Program) ProgramConstant(_ClassString::create(t.value), t.source));
	} break;
	default:
		return optional<_ProgramCall::Arg>(nullopt);
	}
}

static optional<Program> parseExprPart(Context& ctx, Lexer& lexer) {
	Token t = lexer.peek();
	switch (t.type) {
	case Token::Type::WORD: {
		ProgramCall call{t.value, t.source};
		auto a = parseFuncArg(ctx, lexer);
		
		while (a) {
			call->args.emplace_back(*a);
			a = parseFuncArg(ctx, lexer);
		}
		
		unordered_set<string> flagsUsed;
		for (const auto& arg : call->args) {
			if (arg.flag) {
				if (flagsUsed.find(arg.key) != flagsUsed.end()) {
					throw Error("Duplicate flag given to function: " + arg.key);
				}
				flagsUsed.insert(arg.key);
			}
			
		}
		
		return optional<Program>((Program) call);
	} break;
	case Token::Type::VAR:
	case Token::Type::EX_STRING:
	case Token::Type::STRING:
	case Token::Type::LPAREN:
	case Token::Type::LBRACE:
	case Token::Type::LBRACKET: {
		auto arg = parseFuncArg(ctx, lexer);
		parsePostConst(ctx, lexer);
		return arg->value;
	} break;
	default:
		return optional<Program>(nullopt);
	}
}

static Program parseExpr(Context& ctx, Lexer& lexer) {
	auto p = parseExprPart(ctx, lexer);
	bool done = false;
	while (!done) {
		Token t = lexer.peek();
		switch (t.type) {
		case Token::Type::PIPE: {
			lexer.next();
			if (!p) p = (Program) ProgramNull{t.source};
			auto rhs = parseExprPart(ctx, lexer);
			if (!rhs) rhs = (Program) ProgramNull{t.source};
			p = (Program) ProgramPipe(*p, *rhs, t.source);
		} break;
		case Token::Type::SEMICOLON: {
			lexer.next();
			if (!p) p = (Program) ProgramNull{t.source};
			auto rhs = parseExprPart(ctx, lexer);
			if (!rhs) rhs = (Program) ProgramNull{t.source};
			p = (Program) ProgramSequence(*p, *rhs, t.source);
		} break;
		case Token::Type::NONE:
		case Token::Type::RPAREN:
		case Token::Type::RBRACE: {
			if (!p) p = (Program) ProgramNull{t.source};
			done = true;
		} break;
		default:
			done = true;
		}
	}
	
	if (p) {
		return *p;
	}
	
	throw Error("Unexpected token in expression: " + lexer.peek().value);
}

Program ccl::parse(Context& ctx, Lexer& lexer) {
	return parseExpr(ctx, lexer);
}
