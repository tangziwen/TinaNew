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
	evalR(astRootNode, program);

	program.cmdList.push_back(ILCmd(ILCommandType::HALT));
	return program;
}





OperandLocation TinaCompiler::getLeafAddr(TinaASTNode* ast_node, TinaProgram & program)
{
	if(ast_node->m_type == TinaASTNodeType::LEAF)//identifier const up-value
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
			return OperandLocation(src, addr);
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
			return OperandLocation(src, addr);
		}

	}
	abort();
}

//Eval R vaule for expr, we only process inter-node, return location
OperandLocation TinaCompiler::evalR(TinaASTNode* ast_node, TinaProgram& program)
{

	if(ast_node->m_type == TinaASTNodeType::SEQUENCE)
	{
		OperandLocation lastLocation;
		for(TinaASTNode * child : ast_node->m_children)
		{
			lastLocation = evalR(child, program);
		}
		return lastLocation;
	}
	else if(ast_node->m_type == TinaASTNodeType::CALL)
	{
		if(ast_node->m_children.size() > 1)
		{
			for(int i = 1; i < ast_node->m_children.size(); i++)
			{
				evalR(ast_node->m_children[i], program);
			}
		}
		int argNum = ast_node->m_children.size() - 1;
		program.cmdList.push_back(ILCmd(ILCommandType::CALL,
			getLeafAddr(ast_node->m_children[0], program), OperandLocation(LOCAL_TYPE_IMEEDIATE, argNum), OperandLocation(LOCAL_TYPE_REGISTER, m_registerIndex - argNum + 1)));
		m_registerIndex -= argNum;
		
	}
	else if(ast_node->m_type == TinaASTNodeType::LEAF)//identifier const up-value
	{
		auto tmpAddr = OperandLocation(LOCAL_TYPE_REGISTER, m_registerIndex);//RValue use a temp copy value's addr
		program.cmdList.push_back(ILCmd(ILCommandType::MOV, tmpAddr, getLeafAddr(ast_node, program)));
		m_registerIndex++;
		return tmpAddr;
	}
	else if(ast_node->m_type == TinaASTNodeType::OPERATOR)
	{
		switch (ast_node->m_op.m_tokenType)
		{
			case TokenType::TOKEN_TYPE_OP_PLUS:
				{
					auto locationL = evalR(ast_node->m_children[0], program);
					auto locationR = evalR(ast_node->m_children[1], program);
					auto resultLocation = OperandLocation(LOCAL_TYPE_REGISTER, m_registerIndex);
					program.cmdList.push_back(ILCmd(ILCommandType::ADD, 
						locationL, locationR, resultLocation));
					m_registerIndex ++;
					return resultLocation;
				}
				break;
			case TokenType::TOKEN_TYPE_OP_MINUS:
				{
					auto locationL = evalR(ast_node->m_children[0], program);
					auto locationR = evalR(ast_node->m_children[1], program);
					auto resultLocation = OperandLocation(LOCAL_TYPE_REGISTER, m_registerIndex);
					program.cmdList.push_back(ILCmd(ILCommandType::SUB, 
						locationL, locationR, resultLocation));
					m_registerIndex ++;
					return resultLocation;
				}
				break;
			case TokenType::TOKEN_TYPE_OP_MULTIPLY:
				{
					auto locationL = evalR(ast_node->m_children[0], program);
					auto locationR = evalR(ast_node->m_children[1], program);
					auto resultLocation = OperandLocation(LOCAL_TYPE_REGISTER, m_registerIndex);
					program.cmdList.push_back(ILCmd(ILCommandType::MUL, 
						locationL, locationR, resultLocation));
					m_registerIndex ++;
					return resultLocation;
				}
				break;
			case TokenType::TOKEN_TYPE_OP_DIVIDE:
				{
					auto locationL = evalR(ast_node->m_children[0], program);
					auto locationR = evalR(ast_node->m_children[1], program);
					auto resultLocation = OperandLocation(LOCAL_TYPE_REGISTER, m_registerIndex);
					program.cmdList.push_back(ILCmd(ILCommandType::DIV, 
						locationL, locationR, resultLocation));
					m_registerIndex ++;
					return resultLocation;
				}
				break;
			case TokenType::TOKEN_TYPE_OP_ASSIGN://!!!! L-vaule use it own addr
				{
					auto locationL = getLeafAddr(ast_node->m_children[0], program);
					auto locationR = evalR(ast_node->m_children[1], program);
					auto resultLocation = OperandLocation(LOCAL_TYPE_REGISTER, m_registerIndex);
					m_registerIndex ++;
					//return resultLocation;

					program.cmdList.push_back(ILCmd(ILCommandType::MOV, locationL, locationR));
					return locationL;
				}
				break;
		}
	}
}
}
