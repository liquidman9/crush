/*
 * Entity.hpp
 */

class Entity {
private:
	static id_gen;

public:
	const long id;
	Float3 position;
	Quaternion orientation;

	Entity();
	Entity(position, orientation);
}