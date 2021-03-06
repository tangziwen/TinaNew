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
	LOCAL_DECLARE,
	PRINT,
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
	bool m_isLvalue = false;
};

/*
 * <statement>	: blockstatement
 *				| singlestatement
 *
 * <blockstatement>	: <{> { <localDeclare> } | {<PrintStatement>} <}>
 *
 * <localDeclare>	: <local> <identifier> {, <identifier>} ;
 *					| <singlestatement>
 * <PrintStatement>	: <print> <singlestatement> ;
 *					| <singlestatement>
 * <singlestatement>	: ;
 *				| <expr> ;
 *				
 * <expr>		: <assignexpr> {, <assignexpr>}
 * 
 * <assignexpr> : <compexpr> {= <compexpr>}
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
 *               | <factor> {<(> <)>}
 *               | <factor> {<(> <factor> <)>}
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
	TinaASTNode * getRoot();
private:
	TinaASTNode * parseStatement();
	TinaASTNode * parseBlockStatement();
	TinaASTNode * parsePrintStatement();
	TinaASTNode * parseSingleStatement();
	TinaASTNode * parseLocalDeclare();
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
	size_t m_tokenPos = 0;
	TinaASTNode * m_root;
	
};
}
