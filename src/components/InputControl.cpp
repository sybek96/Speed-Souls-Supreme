#include "components\InputControl.h"

/// <summary>
/// @brief Default Input component.
/// 
/// 
/// </summary>
InputControl::InputControl()
	: m_accelerate(false)
	, m_brake(false)
	, m_handbrake(false)
	, m_pause(false)
	, m_turn(0.0f)
{
}
