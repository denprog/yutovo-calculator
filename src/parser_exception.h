#ifndef PARSEREXCEPTION_H
#define PARSEREXCEPTION_H

#include <string>
#include <vector>

namespace yutovo_calculator
{
	typedef unsigned int uint;
	typedef std::vector<uint> ElementId;

	enum ParserExceptionCode
	{
		None = -1, 
		SyntaxError = 1, 
		WrongArgumentsCount = 2, 
		UnknownIdentifier = 3, 
		ExpressionExpected = 4,
		DivisionByZero = 100, 
		Overflow = 101, 
		ArgumentIsOver = 102, 
		ConversionDoesNotFit = 103,
		UnitsAreIncompatible = 201,
		CannotCastToUnit = 202,
		UnknownLanguage = 301
	};
	
	struct ParserException
	{
		ParserException() = default;
		
		ParserException(ElementId _id, ParserExceptionCode _ex_id, int _pos, int _line) :
			id(_id),
			ex_id(_ex_id),
			pos(_pos),
			line(_line)
		{
		}

		ParserException(ElementId _id, ParserExceptionCode _ex_id) :
			id(_id),
			ex_id(_ex_id)
		{
		}

		ParserException(ElementId _id, ParserExceptionCode _ex_id, std::u32string _description, int _pos, int _line) :
			id(_id),
			ex_id(_ex_id),
			description(_description),
			pos(_pos),
			line(_line)
		{
		}

		friend bool operator==(const ParserException& e1, const ParserException& e2)
		{
			return e1.id == e2.id && e1.ex_id == e2.ex_id && e1.pos == e2.pos && e1.line == e2.line;
		}
		
		ElementId id;
		ParserExceptionCode ex_id = None;
		int pos = -1;
		int line = -1;
		std::u32string description;
	};
	
	struct SyntaxException : public ParserException
	{
		SyntaxException(ElementId _id, ParserExceptionCode _ex_id, int _pos = -1, int _line = -1) : 
			ParserException(_id, _ex_id, _pos, _line)
		{
		}

		SyntaxException(ElementId _id, ParserExceptionCode _ex_id, std::u32string _description, int _pos = -1, int _line = -1) : 
			ParserException(_id, _ex_id, _description, _pos, _line)
		{
		}
	};
	
	struct MathException : public ParserException
	{
		MathException(ParserExceptionCode _id) : ParserException(ElementId{}, _id, -1, -1)
		{
		}

		MathException(ElementId _id, ParserExceptionCode _ex_id) : ParserException(_id, _ex_id)
		{
		}

		MathException(ElementId _id, ParserExceptionCode _ex_id, int _pos, int _line) : ParserException(_id, _ex_id, _pos, _line)
		{
		}
	};
}

#endif
