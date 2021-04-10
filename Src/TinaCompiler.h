#pragma once
#include <string>
#include <vector>
#include <unordered_map>
namespace tzw
{
struct TinaASTNode;

enum class ILCommandType
{
	MOV,
	PUSH,
	POP,
	ADD,
	SUB,
	MUL,
	DIV,
	JMP,
	CALL,
	RETURN,
	PRINT,
	HALT,

};

#define LOCAL_TYPE_REGISTER (0)
#define LOCAL_TYPE_LOCAL (1)
#define LOCAL_TYPE_CONST (2)
#define LOCAL_TYPE_IMEEDIATE (3)

struct OperandLocation
{
	char m_locSrc = LOCAL_TYPE_REGISTER;
	unsigned char m_addr = 0;
	OperandLocation(char src, unsigned char addr): m_locSrc(src),m_addr(addr)
	{
		
	}
	OperandLocation()
	{
		
	}
};
//3-addresses code: C = A + B
struct ILCmd
{
	ILCommandType m_type;
	OperandLocation m_A;
	OperandLocation m_B;
	OperandLocation m_C;
	explicit ILCmd(ILCommandType type);
	ILCmd(ILCommandType type, OperandLocation A, OperandLocation B);
	ILCmd(ILCommandType type, OperandLocation A, OperandLocation B, OperandLocation C);
};
class TinaCompiler
{
public:
	std::vector<ILCmd> gen(TinaASTNode * astRootNode);
private:
	void traverseAST(TinaASTNode * ast_node, std::vector<ILCmd> & cmdList, std::vector<std::string>& stackVar, std::vector<std::string>& constVal);
	unsigned char m_registerIndex = 0;
	std::unordered_map<std::string, int> m_stackMap;
	std::unordered_map<std::string, int> m_constMap;
	
};

}
