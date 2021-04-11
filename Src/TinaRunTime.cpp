#include "TinaRunTime.h"

#include <cassert>

namespace tzw
{
void TinaRunTime::execute(TinaProgram* program)
{
	for (;;pc++)
	{
		auto& cmd = program->cmdList[pc];
		if (cmd.m_type == ILCommandType::HALT)
		{
			break;
		}
		switch (cmd.m_type)
		{
			case ILCommandType::MOV:
			{
				TinaVal val;
				//From
				if(cmd.m_B.m_locSrc == LOCAL_TYPE_ENV)//from env
				{
					val = (*refFromEnv(program->envSymbol[cmd.m_B.m_addr]).m_data.valRef);
				}
				else if (cmd.m_B.m_locSrc == LOCAL_TYPE_CONST)
				{
					val = program->constVal[cmd.m_B.m_addr];
				}

				//To
				if(cmd.m_A.m_locSrc == LOCAL_TYPE_REGISTER)//To ref in specified register.
				{
					m_register[cmd.m_A.m_addr] = val;
				}
			}
			break;
			case ILCommandType::PUSH:
			{
			}
			break;
			case ILCommandType::POP:
			{
			}break;
			case ILCommandType::ADD:
			{
			}break;
			case ILCommandType::SUB:
			{
			}break;
			case ILCommandType::MUL:
			{
			}break;
			case ILCommandType::DIV:
			{
			}break;
			case ILCommandType::JMP:
			{
			}break;
			case ILCommandType::CALL:
			{
			}break;
			case ILCommandType::RETURN:
			{
			}break;
			case ILCommandType::PRINT:
			{
			}break;
			case ILCommandType::HALT:
			{
			}break;
			case ILCommandType::MOVREF:
			{
				TinaVal val;
				//From
				if(cmd.m_B.m_locSrc == LOCAL_TYPE_REGISTER)//from env
				{
					val = m_register[cmd.m_B.m_addr];
				}
				else if (cmd.m_B.m_locSrc == LOCAL_TYPE_CONST)
				{
					val = program->constVal[cmd.m_B.m_addr];
				}

				//To
				if(cmd.m_A.m_locSrc == LOCAL_TYPE_REGISTER)//To ref in specified register.
				{
					TinaVal ref = m_register[cmd.m_A.m_addr];
					assert(ref.m_type == TinaValType::Ref);
					(*ref.m_data.valRef) = val;
				}
			}break;
			case ILCommandType::LEA:
			{
				TinaVal val;
				//From
				if(cmd.m_B.m_locSrc == LOCAL_TYPE_ENV)//from env
				{
					val = refFromEnv(program->envSymbol[cmd.m_B.m_addr]);
				}

				//to
				if(cmd.m_A.m_locSrc == LOCAL_TYPE_REGISTER)
				{
					m_register[cmd.m_A.m_addr] = val;
				}
				
			}
			break;
			default:;
		}
	}
}

TinaVal TinaRunTime::refFromEnv(std::string identifier)
{
	TinaVal resultVal;
	resultVal.m_type = TinaValType::Ref;
	auto result = m_envMap.find(identifier);
	if(result == m_envMap.end())
	{
		TinaVal * val = new TinaVal();
		m_envMap[identifier] = val;
		resultVal.m_data.valRef = val;
		
	}
	else
	{
		resultVal.m_data.valRef = result->second;
	}

	return resultVal;
}
}
