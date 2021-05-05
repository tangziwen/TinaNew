
#include <iostream>
#include "Src/TinaTokenizer.h"
#include "Src/TinaParser.h"
#include "Src/TinaCompiler.h"
#include "Src/TinaRunTime.h"
using namespace  tzw;
int main()
{
	TinaTokenizer *tokenizer = new TinaTokenizer();
	tokenizer->loadStr("{local a,b; b= 2; a = (5 + b) * 2; print a;}");
	std::vector<TokenInfo> result =  tokenizer->getTokenList();
	for(TokenInfo& token : result)
	{
		token.print();
	}
	TinaParser parser;
	parser.parse(result);

	TinaCompiler * compiler = new TinaCompiler();
	TinaProgram program = compiler->gen(parser.getRoot());

	TinaRunTime * runtime = new TinaRunTime();
	runtime->execute(&program);

	return 0;
}

