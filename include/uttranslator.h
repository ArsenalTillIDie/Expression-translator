#include <string>
#include "utstack.h"
#include <iostream>

#define MAX_STACK_SIZE 10000

enum termType { NUMBER, OPERATOR, OPENINGBRACKET, CLOSINGBRACKET };

class Term {
protected:
	termType type;
public:
	termType getType() {
		return type;
	}
	virtual void print() = 0;
};

class Number : public Term {
public:
	Number(double number) {
		type = NUMBER;
		value = number;
	}
	double value;
	void print() {
		std::cout << value;
	}
};

class Operator : public Term {
public:
	Operator(char op) {
		type = OPERATOR;
		opType = op;
	}
	int priority() {
		if (opType == '+' || opType == '-') return 0;
		if (opType == '*' || opType == '/') return 1;
	}
	char opType;
	void print() {
		std::cout << opType;
	}
};

class OpeningBracket : public Term {
public:
	OpeningBracket() {
		type = OPENINGBRACKET;
	}
	void print() {
		std::cout << '(';
	}
};

class ClosingBracket : public Term {
public:
	ClosingBracket() {
		type = CLOSINGBRACKET;
	}
	void print() {
		std::cout << ')';
	}
};

class Expression {
protected:
	std::string string;
	Vector<Term*> terms;
	Vector<Term*> postfixNotation;
	double result;
	bool lexicalAnalysis() {
		std::string str = string;
		bool correct = true;
		while (str.length() > 0) {
			if (str[0] == '+' || str[0] == '-' || str[0] == '*' || str[0] == '/') {
				terms.push_back(new Operator(str[0]));
				str.erase(0, 1);
			}
			else if (str[0] == '(') {
				terms.push_back(new OpeningBracket);
				str.erase(0, 1);
			}
			else if (str[0] == ')') {
				terms.push_back(new ClosingBracket);
				str.erase(0, 1);
			}
			else if (str[0] >= '0' && str[0] <= '9') {
				size_t numLength;
				terms.push_back(new Number(std::stod(str, &numLength)));
				str.erase(0, numLength);
			}
			else {
				correct = false;
				break;
			}
		}
		return correct;
	}
	bool syntaxAnalysis() {
		bool correct = true;
		int bracketCheck = 0;
		for (int i = 0; i < terms.getSize(); i++) {
			if (terms[i]->getType() == OPENINGBRACKET)
				bracketCheck++;
			if (terms[i]->getType() == CLOSINGBRACKET)
				bracketCheck--;
			if (bracketCheck < 0) return false;
		}
		if (bracketCheck != 0) {
			return false;
		}
		if (terms[0]->getType() == CLOSINGBRACKET || terms[0]->getType() == OPERATOR) correct = false;
		for (int i = 0; i < terms.getSize() - 1; i++) {
			switch (terms[i]->getType()) {
			case OPENINGBRACKET:
				if (terms[i + 1]->getType() == CLOSINGBRACKET || terms[i + 1]->getType() == OPERATOR) correct = false;
				break;
			case CLOSINGBRACKET:
				if (terms[i + 1]->getType() == OPENINGBRACKET || terms[i + 1]->getType() == NUMBER) correct = false;
				break;
			case NUMBER:
				if (terms[i + 1]->getType() == OPENINGBRACKET || terms[i + 1]->getType() == NUMBER) correct = false;
				break;
			case OPERATOR:
				if (terms[i + 1]->getType() == CLOSINGBRACKET || terms[i + 1]->getType() == OPERATOR) correct = false;
				break;
			}
		}
		if (terms[terms.getSize() - 1]->getType() == OPENINGBRACKET || terms[terms.getSize() - 1]->getType() == OPERATOR) correct = false;
		return correct;
	}
	void translation() {
		TStack<Term*> termStack(MAX_STACK_SIZE);
		for (int i = 0; i < terms.getSize(); i++) {
			switch (terms[i]->getType()) {
			case OPENINGBRACKET:
				termStack.push(terms[i]);
				break;
			case CLOSINGBRACKET:
				while (termStack.top()->getType() != OPENINGBRACKET) {
					postfixNotation.push_back(termStack.top());
					termStack.pop();
				}
				termStack.pop();
				break;
			case NUMBER:
				postfixNotation.push_back(terms[i]);
				break;
			case OPERATOR:
				if (termStack.empty()) termStack.push(terms[i]);
				else if (termStack.top()->getType() != OPERATOR) termStack.push(terms[i]);
				else {
					while (!termStack.empty() && termStack.top()->getType() == OPERATOR && ((Operator*)(termStack.top()))->priority() >= ((Operator*)terms[i])->priority()) {
						postfixNotation.push_back(termStack.top());
						termStack.pop();
					}
					termStack.push(terms[i]);
				}
				break;
			}
		}
		while (!termStack.empty()) {
			postfixNotation.push_back(termStack.top());
			termStack.pop();
		}
	}
	bool calculation() {
		TStack<double> numberStack(MAX_STACK_SIZE);
		for (int i = 0; i < postfixNotation.getSize(); i++) {
			switch (postfixNotation[i]->getType()) {
			case NUMBER:
				numberStack.push(((Number*)(postfixNotation[i]))->value);
				break;
			case OPERATOR:
				double operand2 = numberStack.top();
				numberStack.pop();
				double operand1 = numberStack.top();
				numberStack.pop();
				switch (((Operator*)(postfixNotation[i]))->opType) {
				case '+':
					numberStack.push(operand1 + operand2);
					break;
				case '-':
					numberStack.push(operand1 - operand2);
					break;
				case '*':
					numberStack.push(operand1 * operand2);
					break;
				case '/':
					if (operand2 == 0.0) {
						return false;
					}
					numberStack.push(operand1 / operand2);
					break;
				}
				break;
			}
		}
		result = numberStack.top();
		return true;
	}
public:
	Expression(std::string str) {
		string = str;
	}
	~Expression() {
		for (int i = 0; i < terms.getSize(); i++)
			delete terms[i];
	}
	double solve() {
		if (!lexicalAnalysis())
			throw "Lexical analysis failed";
		/*
		for (int i = 0; i < terms.getSize(); i++) {
			terms[i]->print();
			std::cout << " ";
		}
		std::cout << std::endl;
		*/
		if (!syntaxAnalysis())
			throw "Syntax analysis failed";
		translation();
		/*
		for (int i = 0; i < postfixNotation.getSize(); i++) {
			postfixNotation[i]->print();
			std::cout << " ";
		}
		std::cout << std::endl;
		*/
		if (!calculation())
			throw "Runtime error";
		return result;
	}
};