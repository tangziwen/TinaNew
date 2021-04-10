
#include <iostream>
#include "Src/TinaTokenizer.h"
#include "Src/TinaParser.h"
using namespace  tzw;
int main()
{
	TinaTokenizer *tokenizer = new TinaTokenizer();
	tokenizer->loadStr("print(3, 2, 1)");
	std::vector<TokenInfo> result =  tokenizer->getTokenList();
	for(TokenInfo& token : result)
	{
		token.print();
	}
	TinaParser parser;
	parser.parse(result);

}

