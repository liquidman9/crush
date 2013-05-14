#pragma once

#include<vector>
#include<assert.h>





class Sendable
{

public :

	virtual const unsigned int size() const = 0;
	virtual unsigned int encode(char *) const = 0;
	virtual unsigned int decode(const char *) = 0;

};

