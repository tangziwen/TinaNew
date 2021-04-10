
#include <iostream>
#include "Src/TinaTokenizer.h"
#include "Src/TinaParser.h"
#include "Src/TinaCompiler.h"
using namespace  tzw;
int main()
{
	TinaTokenizer *tokenizer = new TinaTokenizer();
	tokenizer->loadStr("{3 = 3; print(3 * a + 5);}");
	std::vector<TokenInfo> result =  tokenizer->getTokenList();
	for(TokenInfo& token : result)
	{
		token.print();
	}
	TinaParser parser;
	parser.parse(result);

	TinaCompiler * compiler = new TinaCompiler();
	compiler->gen(parser.getRoot());

}

