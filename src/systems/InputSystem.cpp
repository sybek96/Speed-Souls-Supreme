#include "systems\InputSystem.h"

/// <summary>
/// @brief Input system constructor.
/// 
/// 
/// </summary>
InputSystem::InputSystem(std::shared_ptr<Xbox360Controller> controller)
	: m_controller(controller)
{
}

/// <summary>
/// @brief updates input system.
/// 
/// 
/// </summary>
/// <param name="entities">reference to entity manager</param>
/// <param name="events">reference to events manager</param>
/// <param name="dt">time per update</param>
void InputSystem::update(	entityx::EntityManager & entities
							, entityx::EventManager & events
							, double dt)
{
	InputControl::Handle input;
	Player::Handle player;

	m_controller->update();
	if (m_controller->isConnected())
	{
		for (entityx::Entity entity : entities.entities_with_components(input, player))
		{
			checkForInput(input, *m_controller);
		}
	}
}

/// <summary>
/// @brief Will check for controller input.
/// 
/// 
/// </summary>
/// <param name="input">entity's InputControl component reference</param>
/// <param name="controller">reference to controller</param>
void InputSystem::checkForInput(InputControl::Handle& input, const Xbox360Controller& controller)
{
	input->m_accelerate = (controller.m_currentState.m_rTrigger > 50.0f) || controller.m_currentState.m_A;
	input->m_brake = (controller.m_currentState.m_lTrigger > 50.0f) || controller.m_currentState.m_B;
	input->m_handbrake = controller.m_currentState.m_X;
	input->m_pause = controller.m_currentState.m_start;
	input->m_turn = controller.m_currentState.m_lTS.x;
}
