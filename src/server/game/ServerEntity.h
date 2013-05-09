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

	// Recalculates values based on position/orientation
	void recalculateRelativeValues();

public:
	// Effective constants
	// THESE NEED TO BE PROTECTED

	float m_mass; float m_mass_inverse;
	D3DXMATRIX m_rot_inertia, m_rot_inertia_inverse;

	// Collision
	//float m_radius;
	float m_length;
	float m_elastic;
	//D3DXVECTOR3 m_pFront;
	//D3DXVECTOR3 m_pBack;

	// Derived Variables
	D3DXVECTOR3 m_angular_velocity;
	Quaternion m_orientation_delta;

	// Calculated Values
	D3DXVECTOR3 m_momentum;
	D3DXVECTOR3 m_angular_momentum;

	// Accumulator Values
	D3DXVECTOR3 t_impulse;
	D3DXVECTOR3 t_angular_impulse;

	bool m_immovable;
	int m_resourceSpots;
	bool m_destroy;


	ServerEntity();
	ServerEntity(float mass);

	ServerEntity(float mass, D3DXVECTOR3 rot_inertia);

	ServerEntity(float mass, D3DXVECTOR3 rot_inertia, float length, float elastic);

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
	virtual void applyLinearImpulse(D3DXVECTOR3 force);

	// Applies a rotational force to this entity (will not cause linear motion)
	virtual void applyAngularImpulse(D3DXVECTOR3 torque);

	// Applies a force to this entity at the given point for the given amount of time
	virtual void applyImpulse(D3DXVECTOR3 impulse, D3DXVECTOR3 point);

	// Updates entity's physical variables
	virtual void update(float delta_time);

	// Getters/Setters
	D3DXVECTOR3 getVelocity();

	// Utility Methods
	// Perform angle-axis rotation to this Entity
	virtual void rotate(float angle, D3DXVECTOR3 axis);
	// Adjust the Entity's position by the given vector.
	virtual void move(D3DXVECTOR3 vector);
	// Resets values
	virtual void reset();

	virtual void print();

	// Forced abstract for rotational inertia calculation
	virtual D3DXVECTOR3 calculateRotationalInertia(float mass) = 0;
};


#endif SERVERENTITY_H_INCLUDED