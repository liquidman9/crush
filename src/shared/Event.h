/*
 * Event.h
 */

// Project includes
#include <shared/network/Sendable.h>

#define ENUM_TYPE char

namespace shared {
	namespace events {
		enum Type : char {COLLISION, REMOVAL};
	}
}

class Event : public Sendable {
private:
	struct send_struct 
	{
		ENUM_TYPE type;
	};

protected:
	static const unsigned int m_size = sizeof(send_struct);
	// THIS CONSTRUCTOR SHOULD NEVER BE DIRECTLY CALLED.
	Event();

public:
	shared::events::Type m_type;

	// Constructor
	Event(shared::events::Type type);

	// Destructor
	virtual ~Event();

	virtual unsigned int encode(char *) const;
	virtual unsigned int decode(const char *);
	virtual const unsigned int size() const { return m_size; };
};