#include "components\Road.h"

/// <summary>
/// @brief Road component constructor.
/// 
/// 
/// </summary>
/// <param name="type">defines the type of road</param>
Road::Road(
	const Assets::Textures::Track & type
	, const int& roadN)
	 
	: m_type(type)
	, m_roadNum(roadN)
{
}
