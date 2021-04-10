#include "TinaRunTime.h"

namespace tzw
{
void TinaRunTime::execute(TinaProgram* program)
{
	for (;;)
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
			default:;
		}
	}
}
}
