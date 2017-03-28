#include "components\Ai.h"


/// <summary>
/// @ brief Default Constructor
/// 
/// 
///  Default Constructor which is creating a Ai component
/// </summary>
Ai::Ai()
	: m_nodePath(0)
	, m_steering(0.0f, 0.0f)
	, m_nodeNum(0)
{
	m_nodePath = rand() % 3 + 1;
}



