
#include <iostream>
#include "Src/TinaTokenizer.h"
#include "Src/TinaParser.h"
#include "Src/TinaCompiler.h"
#include "Src/TinaRunTime.h"
using namespace  tzw;
int main()
{
	TinaTokenizer *tokenizer = new TinaTokenizer();
	tokenizer->loadStr("{a = 5;}");
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

