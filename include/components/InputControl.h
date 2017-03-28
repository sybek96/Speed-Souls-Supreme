#ifndef INPUT_CONTROL_H
#define INPUT_CONTROL_H

#include "entityx\Entity.h"

/// <summary>
/// @author Rafael Plugge
/// @version 1.0
/// @brief Input control component.
/// 
/// Contains all games keybinds and their states (on/off)
/// </summary>
struct InputControl : public entityx::Component<InputControl>
{
	InputControl();

	bool m_accelerate;
	bool m_brake;
	bool m_handbrake;
	bool m_pause;
	float m_turn;
};

#endif // !INPUT_CONTROL_H