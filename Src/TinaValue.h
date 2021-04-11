#pragma once
namespace tzw
{
enum class TinaValType
{
	Int,
	Float,
	Ref,
	Nil,
};
struct TinaVal
{
	union 
	{
		int valI;
		float valF;
		TinaVal* valRef;//ptr
	} m_data;
	TinaValType m_type;
};
}
