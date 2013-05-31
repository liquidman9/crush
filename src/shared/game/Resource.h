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
	static const unsigned int m_size = Entity::m_size + sizeof(bool);

public:
	// Constructors
	Resource();


	// Methods
	virtual unsigned int encode(char *) const;
	virtual unsigned int decode(const char *);
	virtual const unsigned int size() const { return m_size; };

	friend ostream& operator<<(ostream& os, const Resource&);
	virtual void update(shared_ptr<Entity> source);
};


#endif RESOURCE_H_INCLUDED