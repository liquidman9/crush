/*
 * ServerEntity.hpp
 */
#ifndef SERVERENTITY_H_INCLUDED
#define SERVERENTITY_H_INCLUDED

// Global includes
#include <d3dx9.h>

// Project includes
#include <shared/game/Entity.h>
#include <shared/util/SharedUtils.h>

class ServerEntity : public virtual Entity {
private:
	static int s_id_gen;

protected:
	/* THESE NEED TO BE PROTECTED
	float m_max_velocity;
	float m_max_angular_velocity;

	D3DXVECTOR3 m_velocity;
	D3DXVECTOR3 m_angular_velocity;

	float m_mass;
	D3DXVECTOR3 c_rot_inertia;
	float m_radius;
	*/

public:
	// Effective constants
	// THESE NEED TO BE PROTECTED
	float m_max_velocity;
	float m_max_angular_velocity;

	float m_mass; float m_mass_inverse;
	D3DXVECTOR3 m_rot_inertia, m_rot_inertia_inverse;

	float m_radius;

	// Derived Variables
	D3DXVECTOR3 m_velocity;
	D3DXVECTOR3 m_angular_velocity;
	Quaternion m_orientation_delta;

	// Calculated Values
	D3DXVECTOR3 m_momentum;
	D3DXVECTOR3 m_angular_momentum;

	// Accumulator Values
	D3DXVECTOR3 t_impulse;
	D3DXVECTOR3 t_angular_impulse;

	bool m_immovable;


	ServerEntity();
	ServerEntity(float mass);
	ServerEntity(D3DXVECTOR3 velocity, float max_velocity, float mass);
	ServerEntity(float max_velocity, float max_angular_velocity, float mass, D3DXVECTOR3 rot_inertia);

	// General Methods
	int genId();

	// Physics Methods

	/*
	 * PHYSICS NOTES:
	 *
	 * Impulse is a force applied over time. As we use timesteps and not 
	 * integration methods, we need to have a duration for each force.
	 *
	 */

	// Applies a force to this entity's center of mass for the given amount of time(will not cause rotation)
	virtual void applyLinearImpulse(D3DXVECTOR3 force, float duration);

	// Applies a rotational force to this entity (will not cause linear motion)
	virtual void applyAngularImpulse(D3DXVECTOR3 torque, float duration);

	// Applies a force to this entity at the given point for the given amount of time
	virtual void applyImpulse(D3DXVECTOR3 impulse, D3DXVECTOR3 point, float duration);

	// Applies current acceleration to Entity
	virtual void update(float delta_time);

	// Getters/Setters
	void setVelocity(D3DXVECTOR3 velocity);
	D3DXVECTOR3 getVelocity();

	// Utility Methods
	// Perform angle-axis rotation to this Entity
	virtual void rotate(float angle, D3DXVECTOR3 axis);
	// Adjust the Entity's position by the given vector.
	virtual void move(D3DXVECTOR3 vector);
	// Resets values
	virtual void reset();

	// Forced abstract for rotational inertia calculation
	virtual D3DXVECTOR3 calculateRotationalInertia(float mass) = 0;

	
	virtual void calculate(float);

};


#endif SERVERENTITY_H_INCLUDED