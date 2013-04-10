/*
 * Entity.hpp
 */

class Entity {
public:
	const long id;
	float3 position;
	float3 orientation;

	Entity();
	Entity(position, orientation);
}