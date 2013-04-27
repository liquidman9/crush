/*
 * Resource.h
 */

#ifndef RESOURCE_H_INCLUDED
#define RESOURCE_H_INCLUDED

// Global includes
#include <memory>

// Project includes
#include <shared/game/Entity.h>
 
// Defines

class Resource : public virtual Entity {
private:
	static const unsigned int m_size = Entity::m_size;

public:

	// Constructors
	Resource();

	// Methods
	virtual const char* encode() const;
	virtual void decode(const char *);
	virtual const unsigned int size() const { return m_size; };

	//virtual void update(Entity * source);
	friend ostream& operator<<(ostream& os, const Resource&);
	virtual void update(shared_ptr<Entity> source);
};


#endif RESOURCE_H_INCLUDED