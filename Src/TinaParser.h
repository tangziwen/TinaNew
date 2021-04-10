#pragma once
#include "TinaTokenizer.h"
namespace tzw
{
enum class TinaASTNodeType
{
	CALL,
	OPERATOR,
	LEAF, //identifier literal
	SEQUENCE,
};
//recursive descent parser
struct TinaASTNode
{
	explicit TinaASTNode(TokenInfo op,TinaASTNodeType type);
	TinaASTNode(TinaASTNodeType type);
	void addChild(TinaASTNode * node);
	std::vector<TinaASTNode *> m_children;
	TokenInfo m_op;
	TinaASTNode * m_parent;
	TinaASTNodeType m_type;
};

/*
 * <expr>		: <assignexpr> {, <assignexpr>}
 * 
 * <assignexpr> : <identifier> = <compexpr>
 *				| <compexpr>
 *				
 * <compexpr> : <arithexpr> [== <arithexpr>]
 *			  | <arithexpr> [!= <arithexpr>]
 *			  | <arithexpr> [> <arithexpr>]
 *			  | <arithexpr> [< <arithexpr>]
 *			  | <arithexpr> [>= <arithexpr>]
 *			  | <arithexpr> [<= <arithexpr>]
 *			  
 * <arithexpr>  : <term> {+ <term>}
 *               | <term> {- <term>}
 *               
 * <term>        : <factor> {* <factor>}
 *               | <factor> {/ <factor>}
 *               
 * <postfixexpr> : <factor> {( <factor> )}
 *               | <factor> {[ <factor> ]}
 *               | <factor> {( )}
 *               | <factor> {( <factor> )}
 *               
 * <factor>      : <identifier>
 *               | <num>
 *               | - <factor>
 *               | ( <expr> )
 */
class TinaParser
{
public:
	void parse(std::vector<TokenInfo> tokenList);
private:
	TinaASTNode * parseExpr();
	TinaASTNode * parseAssign();
	TinaASTNode * parseCompExpr();
	TinaASTNode * parseArithExpr();
	TinaASTNode * parseTerm();
	TinaASTNode * parsePostfixExpr();
	TinaASTNode * parseFactor();
	TokenInfo & currToken();
	TokenInfo& nextToken();
	TokenInfo & tryNextToken();

	std::vector<TokenInfo> m_tokenList;
	int m_tokenPos = 0;
	TinaASTNode * m_root;
	
};
}
