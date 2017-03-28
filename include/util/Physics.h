#ifndef PHYSICS_H
#define PHYSICS_H

/// <summary>
/// @author Sebastian Kruzel
/// @version 1.0
/// @brief Purely static class that contains Physics related variables
/// 
/// Contains Friction and gravity as static global values
/// </summary>

class Physics
{
public:

	static void setGravity(const float& newGrav);

	static void setCoefficientOfFriction(const float& newFriction);

	static float getGravity();

	static float getCoefficientOfFriction();

	static float getFrictionOffroad();

private:

	float static m_gravity;

	float static m_coefficientOfFriction;

	float static m_frictionOffRoad;

};

#endif // !PHYSICS_H
