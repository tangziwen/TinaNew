#pragma once
#include "TinaCompiler.h"
#include "TinaValue.h"
namespace tzw
{

class TinaRunTime
{
public:
	void execute(TinaProgram * program);
	TinaVal refFromEnv(std::string identifier);
private:
	size_t pc = 0;
	TinaVal m_register[32];
	std::unordered_map<std::string, TinaVal*> m_envMap;
};
}
