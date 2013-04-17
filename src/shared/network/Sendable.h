#pragma once
class Sendable
{
public:
	virtual const unsigned int size() const = 0;
	virtual const char * encode() const = 0;
	virtual void decode(const char *) = 0;

};

