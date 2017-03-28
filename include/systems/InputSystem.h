#ifndef INPUT_SYSTEM_H
#define INPUT_SYSTEM_H

#include "entityx\System.h"

#include "components\InputControl.h"
#include "components\Player.h"

#include "util\Xbox360Controller.h"

/// <summary>
/// @author Rafael Plugge
/// @version 1.0
/// @brief Will handle game input.
/// 
/// this system will handle xbox 360 controller input,
/// aswell as the keyboard input for each entity with InputControl Component
/// </summary>
class InputSystem : public entityx::System<InputSystem>
{
public:
	InputSystem(std::shared_ptr<Xbox360Controller> controller);

	void update(entityx::EntityManager& entities, entityx::EventManager& events, double dt) override;

private:
	void checkForInput(InputControl::Handle&, const Xbox360Controller&);

	std::shared_ptr<Xbox360Controller> m_controller;

};

#endif // !INPUT_SYSTEM_H