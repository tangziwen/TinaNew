#pragma once
#include "TinaCompiler.h"
namespace tzw
{
class TinaRunTime
{
public:
	void execute(TinaProgram * program);
private:
	size_t pc = 0;
};
}
