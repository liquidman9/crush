/*
 * Asteroid.h
 */

#ifndef ASTEROID_H_INCLUDED
#define ASTEROID_H_INCLUDED

// Global includes
#include <memory>

// Project includes
#include <shared/game/Entity.h>
 
// Defines

class Asteroid : public virtual Entity {
private:
	static const unsigned int m_size = Entity::m_size + sizeof(float);

public:
	// Fields
	float m_scale;

	// Constructors
	Asteroid();
	Asteroid(float scale);

	// Methods
	virtual const char* encode() const;
	virtual void decode(const char *);
	virtual const unsigned int size() const { return m_size; };

	//virtual void update(Entity * source);
	friend ostream& operator<<(ostream& os, const Asteroid&);
	virtual void update(shared_ptr<Entity> source);
};


#endif ASTEROID_H_INCLUDED