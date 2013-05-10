/*
 * Extractor.hpp
 */

#ifndef EXTRACTOR_H_INCLUDED
#define EXTRACTOR_H_INCLUDED

// Global includes
#include <d3dx9.h>

// Project includes
#include <shared/game/Entity.h>

class Extractor : public virtual Entity {
private:
	static const unsigned int m_size = Entity::m_size;
public:

	// Fields

	// Constructors
	Extractor();

	// Methods
	virtual unsigned int encode(char *) const;
	virtual unsigned int decode(const char *);
	virtual const unsigned int size() const { return m_size; };

	//virtual void update(Entity * source);
	friend ostream& operator<<(ostream& os, const Extractor&);
	virtual void update(shared_ptr<Entity> source);
};



#endif EXTRACTOR_H_INCLUDED