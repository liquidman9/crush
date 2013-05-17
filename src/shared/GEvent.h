/*
 * GEvent.h
 */

// Project includes
#include <shared/network/Sendable.h>


class GEvent : public Sendable {
private:
	struct send_struct 
	{
		ENUM_TYPE type;
	};

protected:
	static const unsigned int m_size = sizeof(send_struct);
	// THIS CONSTRUCTOR SHOULD NEVER BE DIRECTLY CALLED.
	GEvent();

public:
	Type m_type;

	// Constructor
	GEvent(Type type);

	// Destructor
	virtual ~GEvent();

	virtual unsigned int encode(char *) const;
	virtual unsigned int decode(const char *);
	virtual const unsigned int size() const { return m_size; };
};