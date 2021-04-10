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
	LEA,
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

struct TinaProgram
{
	TinaProgram()
	{
		
	};
	std::vector<std::string> envVar;
	std::vector<std::string> stackVar;
	std::vector<std::string> constVal;
	std::vector<ILCmd> cmdList;
};

class TinaCompiler
{
public:
	TinaProgram gen(TinaASTNode * astRootNode);
private:
	void traverseAST(TinaASTNode * ast_node, TinaProgram & program);
	unsigned char m_registerIndex = 0;
	std::unordered_map<std::string, int> m_stackMap;
	std::unordered_map<std::string, int> m_envMap;
	std::unordered_map<std::string, int> m_constMap;
	
};

}
