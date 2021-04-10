#include "TinaParser.h"

namespace tzw
{
TinaASTNode::TinaASTNode(TokenInfo op,TinaASTNodeType type):m_op(op),m_parent(nullptr),m_type(type)
{
}

TinaASTNode::TinaASTNode(TinaASTNodeType type):m_parent(nullptr),m_type(type)
{
}

void TinaASTNode::addChild(TinaASTNode* node)
{
	m_children.push_back(node);
	node->m_parent = this;
}

void TinaParser::parse(std::vector<TokenInfo> tokenList)
{
	m_tokenList = tokenList;
	m_tokenPos = 0;
	m_root = new TinaASTNode(TinaASTNodeType::SEQUENCE);
	auto exprNode = parseExpr();
	m_root->addChild(exprNode);
}

TinaASTNode* TinaParser::parseExpr()
{
	TinaASTNode * leftNode = parseAssign();
	if(currToken().m_tokenType == TokenType::TOKEN_TYPE_OP_COMMA)
	{
		TinaASTNode * opNode = nullptr;
		while(currToken().m_tokenType == TokenType::TOKEN_TYPE_OP_COMMA)
		{
			opNode = new TinaASTNode(currToken(), TinaASTNodeType::OPERATOR);
			nextToken();
			auto rightNode = parseAssign();
			opNode->addChild(leftNode);
			opNode->addChild(rightNode);

			leftNode = opNode;
		}
		return opNode;
	}
	return leftNode;
}

TinaASTNode* TinaParser::parseAssign()
{
	TinaASTNode * node;
	//a = X format
	if(currToken().m_tokenType == TokenType::TOKEN_TYPE_IDENTIFIER && tryNextToken().m_tokenType == TokenType::TOKEN_TYPE_OP_ASSIGN)
	{
		node = new TinaASTNode(tryNextToken(), TinaASTNodeType::OPERATOR);
		TinaASTNode * lValue = new TinaASTNode(currToken(), TinaASTNodeType::OPERATOR);
		//skip a = 
		nextToken();
		nextToken();
		TinaASTNode * rValue = parseCompExpr();

		node->addChild(lValue);
		node->addChild(rValue);
		
	}
	else
	{
		node = parseCompExpr();
	}
	return node;
}

TinaASTNode* TinaParser::parseCompExpr()
{
	TinaASTNode * left = parseArithExpr();
	if(currToken().isLogicCompare())
	{
		TinaASTNode * op = new TinaASTNode(currToken(), TinaASTNodeType::OPERATOR);
		nextToken();
		TinaASTNode * right = parseArithExpr();
		op->addChild(left);
		op->addChild(right);
		return op;
	}
	else
	{
		return left;
	}
}

TinaASTNode * TinaParser::parseArithExpr()
{
	auto leftNode = parseTerm();

	if(currToken().m_tokenType == TokenType::TOKEN_TYPE_OP_PLUS || currToken().m_tokenType == TokenType::TOKEN_TYPE_OP_MINUS)
	{
		TinaASTNode * opNode = nullptr;
		while(currToken().m_tokenType == TokenType::TOKEN_TYPE_OP_PLUS || currToken().m_tokenType == TokenType::TOKEN_TYPE_OP_MINUS)
		{
			opNode = new TinaASTNode(currToken(), TinaASTNodeType::OPERATOR);
			nextToken();
			auto rightNode = parseTerm();
			opNode->addChild(leftNode);
			opNode->addChild(rightNode);

			leftNode = opNode;
		}
		return opNode;
	}
	else
	{
		
		return leftNode;
	}
}

TinaASTNode * TinaParser::parseTerm()
{
	
	auto leftNode = parsePostfixExpr();
	if(currToken().m_tokenType == TokenType::TOKEN_TYPE_OP_MULTIPLY || currToken().m_tokenType == TokenType::TOKEN_TYPE_OP_DIVIDE)
	{
		TinaASTNode * opNode = nullptr;
		while (currToken().m_tokenType == TokenType::TOKEN_TYPE_OP_MULTIPLY || currToken().m_tokenType == TokenType::TOKEN_TYPE_OP_DIVIDE)
		{
			opNode = new TinaASTNode(currToken(), TinaASTNodeType::OPERATOR);
			
			nextToken();
			auto rightNode = parsePostfixExpr();
			opNode->addChild(leftNode);
			opNode->addChild(rightNode);

			leftNode = opNode;
		}
		return opNode;
	}
	else
	{
		return leftNode;
	}

}

TinaASTNode* TinaParser::parsePostfixExpr()
{
	auto leftNode = parseFactor();
	//function call a(xxx,xxx)
	if(currToken().m_tokenType == TokenType::TOKEN_TYPE_LEFT_PARENTHESES)
	{
		TinaASTNode * opNode = nullptr;
		//invoke like a(void)
		if(tryNextToken().m_tokenType == TokenType::TOKEN_TYPE_RIGHT_PARENTHESES)
		{
			opNode = new TinaASTNode(TinaASTNodeType::CALL);
			opNode->addChild(leftNode);
		}
		else
		{
			//invoke like a(xxx) || a(xxx,xxx...) || a(xxx,xxx...)(xxx,xxx...)...
			while(currToken().m_tokenType == TokenType::TOKEN_TYPE_LEFT_PARENTHESES)
			{
				opNode = new TinaASTNode(TinaASTNodeType::CALL);

				nextToken();
				auto rightNode = parseExpr();
				nextToken();//skip right parentheses
				
				opNode->addChild(leftNode);
				opNode->addChild(rightNode);
				
				leftNode = opNode;
			}
		}
		return opNode;
	}
	//array index refrence a[xx] || a[xx][xx]...
	else if(currToken().m_tokenType == TokenType::TOKEN_TYPE_LEFT_SQUARE_BRACKET)
	{
		TinaASTNode * opNode = nullptr;
		while(currToken().m_tokenType == TokenType::TOKEN_TYPE_LEFT_SQUARE_BRACKET)
		{
			nextToken();
			auto rightNode = parseExpr();
			nextToken();//skip right square bracket
			
			opNode->addChild(leftNode);
			opNode->addChild(rightNode);
			
			leftNode = opNode;
		}
		return opNode;
	}
	else
	{
		return leftNode;
	}
}

TinaASTNode * TinaParser::parseFactor()
{
	TinaASTNode * node = nullptr;
	if(currToken().m_tokenType == TokenType::TOKEN_TYPE_IDENTIFIER)
	{
		node = new TinaASTNode(currToken(), TinaASTNodeType::LEAF);
		nextToken();
	}
	else if(currToken().m_tokenType == TokenType::TOKEN_TYPE_STR)
	{
		node = new TinaASTNode(currToken(), TinaASTNodeType::LEAF);
		nextToken();
	}
	else if(currToken().m_tokenType == TokenType::TOKEN_TYPE_NUM)
	{
		node = new TinaASTNode(currToken(), TinaASTNodeType::LEAF);
		nextToken();
	}
	else if(currToken().m_tokenType == TokenType::TOKEN_TYPE_LEFT_PARENTHESES)
	{
		nextToken();
		node = parseArithExpr();
		nextToken();
	}
	
	return node;
}

TokenInfo& TinaParser::currToken()
{
	return m_tokenList[m_tokenPos];
}

TokenInfo& TinaParser::nextToken()
{
	if(m_tokenPos < m_tokenList.size() -1 )
	{
		m_tokenPos ++;
	}
	return m_tokenList[m_tokenPos];
}

TokenInfo& TinaParser::tryNextToken()
{
	return m_tokenList[m_tokenPos + 1];
}
}