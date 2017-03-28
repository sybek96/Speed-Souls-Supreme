#include "components\Player.h"


/// <summary>
///  @brief Default Constructor.S
/// 
///  Default Constructor which is creating a Player component
/// </summary>
Player::Player(
	const int& currency
	, const int& engine
	, const int& handling
	, const int& wheel)

	: m_currency(currency)
	, m_engine(engine)
	, m_handling(handling)
	, m_wheel(wheel)
{
}