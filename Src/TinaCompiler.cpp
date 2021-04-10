#include "TinaCompiler.h"

#include "TinaParser.h"

namespace tzw
{
ILCmd::ILCmd(ILCommandType type):m_type(type)
{
}

ILCmd::ILCmd(ILCommandType type, OperandLocation A, OperandLocation B):m_type(type),m_A(A), m_B(B)
{
}

ILCmd::ILCmd(ILCommandType type, OperandLocation A, OperandLocation B, OperandLocation C):m_type(type),m_A(A), m_B(B), m_C(C)
{
}

TinaProgram TinaCompiler::gen(TinaASTNode* astRootNode)
{
	TinaProgram program;
	m_stackMap.clear();
	m_constMap.clear();
	
	std::vector<ILCmd> cmdList;
	std::vector<std::string> stackVar;
	std::vector<std::string> envVar;
	std::vector<std::string> constVal;
	traverseAST(astRootNode, program);

	program.cmdList.push_back(ILCmd(ILCommandType::HALT));
	return program;
}

void TinaCompiler::traverseAST(TinaASTNode* ast_node, TinaProgram & program)
{
	for(TinaASTNode * child : ast_node->m_children)
	{
		traverseAST(child, program);
	}
	if(ast_node->m_type == TinaASTNodeType::SEQUENCE)
	{
		//do nothing let the children do the work
	}
	else if(ast_node->m_type == TinaASTNodeType::CALL)
	{
		int argNum = ast_node->m_children.size() - 1;
		program.cmdList.push_back(ILCmd(ILCommandType::CALL,
			OperandLocation(LOCAL_TYPE_REGISTER, m_registerIndex - argNum), OperandLocation(LOCAL_TYPE_IMEEDIATE, argNum), OperandLocation(LOCAL_TYPE_REGISTER, m_registerIndex - argNum + 1)));
		m_registerIndex -= argNum + 1;
	}
	else if(ast_node->m_type == TinaASTNodeType::OPERATOR)
	{
		switch (ast_node->m_op.m_tokenType)
		{
			case TokenType::TOKEN_TYPE_OP_PLUS:
				program.cmdList.push_back(ILCmd(ILCommandType::ADD, 
					OperandLocation(LOCAL_TYPE_REGISTER, m_registerIndex - 1), OperandLocation(LOCAL_TYPE_REGISTER, m_registerIndex), OperandLocation(LOCAL_TYPE_REGISTER, m_registerIndex - 2)));
				m_registerIndex -= 2;
				break;
			case TokenType::TOKEN_TYPE_OP_MINUS:
				program.cmdList.push_back(ILCmd(ILCommandType::SUB, 
					OperandLocation(LOCAL_TYPE_REGISTER, m_registerIndex - 1), OperandLocation(LOCAL_TYPE_REGISTER, m_registerIndex), OperandLocation(LOCAL_TYPE_REGISTER, m_registerIndex - 2)));
				m_registerIndex -= 2;
				break;
			case TokenType::TOKEN_TYPE_OP_MULTIPLY:
				program.cmdList.push_back(ILCmd(ILCommandType::MUL, 
					OperandLocation(LOCAL_TYPE_REGISTER, m_registerIndex - 1), OperandLocation(LOCAL_TYPE_REGISTER, m_registerIndex), OperandLocation(LOCAL_TYPE_REGISTER, m_registerIndex - 2)));
				m_registerIndex -= 2;
				break;
			case TokenType::TOKEN_TYPE_OP_DIVIDE:
				program.cmdList.push_back(ILCmd(ILCommandType::DIV, 
					OperandLocation(LOCAL_TYPE_REGISTER, m_registerIndex - 1), OperandLocation(LOCAL_TYPE_REGISTER, m_registerIndex), OperandLocation(LOCAL_TYPE_REGISTER, m_registerIndex - 2)));
				m_registerIndex -= 2;
				break;
			case TokenType::TOKEN_TYPE_OP_ASSIGN://!!!!左值右值问题
				program.cmdList.push_back(ILCmd(ILCommandType::MOV, 
					OperandLocation(LOCAL_TYPE_REGISTER, m_registerIndex - 1), OperandLocation(LOCAL_TYPE_REGISTER, m_registerIndex)));
				m_registerIndex -= 2;
				break;
		}
	}
	else if(ast_node->m_type == TinaASTNodeType::LEAF)//identifier const up-value
	{
		int addr = -1;
		char src = LOCAL_TYPE_LOCAL;
		if(ast_node->m_op.m_tokenType == TokenType::TOKEN_TYPE_IDENTIFIER)
		{
			auto iter = m_envMap.find(ast_node->m_op.m_tokenValue);
			if(iter == m_envMap.end())
			{
				program.envVar.push_back(ast_node->m_op.m_tokenValue);
				addr = program.envVar.size() - 1;
				m_envMap[ast_node->m_op.m_tokenValue] = addr;
			}
			else
			{
				addr = iter->second;
			}
			src = LOCAL_TYPE_LOCAL;
			if(ast_node->m_isLvalue)
			{
				program.cmdList.push_back(ILCmd(ILCommandType::LEA, 
							OperandLocation(LOCAL_TYPE_REGISTER, m_registerIndex), OperandLocation(src, addr)));
			}
			else
			{
				program.cmdList.push_back(ILCmd(ILCommandType::MOV, 
							OperandLocation(LOCAL_TYPE_REGISTER, m_registerIndex), OperandLocation(src, addr)));
			}
		}
		else if(ast_node->m_op.m_tokenType == TokenType::TOKEN_TYPE_STR || ast_node->m_op.m_tokenType == TokenType::TOKEN_TYPE_NUM)
		{
			auto iter = m_constMap.find(ast_node->m_op.m_tokenValue);
			if(iter == m_constMap.end())
			{
				program.constVal.push_back(ast_node->m_op.m_tokenValue);
				addr = program.constVal.size() - 1;
				m_constMap[ast_node->m_op.m_tokenValue] = addr;
			}
			else
			{
				addr = iter->second;
			}
			src = LOCAL_TYPE_CONST;
			if(ast_node->m_isLvalue)
			{
				abort();//const can not produce lVaule
			}
			else
			{
				program.cmdList.push_back(ILCmd(ILCommandType::MOV, 
							OperandLocation(LOCAL_TYPE_REGISTER, m_registerIndex), OperandLocation(src, addr)));
			}
		}
		

		m_registerIndex += 1;
	}
}
}
