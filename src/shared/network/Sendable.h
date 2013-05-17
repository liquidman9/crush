#pragma once

#include<vector>
#include<assert.h>



#define ENUM_TYPE char
enum Type : char { ENTITY, SHIP, MOTHERSHIP, ASTEROID, RESOURCE, TRACTORBEAM, EXTRACTOR, POWERUP, COLLISIONEVENT};

class Sendable
{

public :

	virtual const unsigned int size() const = 0;
	virtual unsigned int encode(char *) const = 0;
	virtual unsigned int decode(const char *) = 0;

};

