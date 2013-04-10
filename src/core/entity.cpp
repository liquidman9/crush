/*
 * entity.cpp
 */

Entity::id_gen = 0;

Entity::Entity() {
	id = id_gen++;
	position = new Float3();
	orientation = new Quaternion();
}

Entity::Entity(Float3 position, Quarternion orientation) {
	id = id_gen++;
	this->position = new Float3();
	this->orientation = new Quaternion();
}