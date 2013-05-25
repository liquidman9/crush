#include <server/Globals.h>

namespace server {
	namespace game {
		int timelimit = 5 * 60 * 1000;
	}

	namespace world {
		// World size
		float size = 800.0f;

		// Spawn
		float mothership_distance_from_center = 500.0f;
		float ship_spawn_distance_from_center = 450.0f;

		// Asteroids
		float asteroids_num = 50;
		float asteroids_pos_start = 50.0f;
		float asteroids_pos_range = 150.0f;
		float asteroids_scale_start = 0.5f;
		float asteroids_scale_range = 5.0f;
		float asteroids_vel_range = 5.0f;
		float asteroids_rot_vel_range = PI/4.0f;
	}

	namespace entities {
		namespace asteroid {
			float scaleToRadius = 1.0f;
			float density = 10.0f;
		}

		namespace tractorbeam {
			float power = 5.0f;
			float length = 350.0f;
		}

		namespace ship {
			float mass = 10000.0f;
			float forward_impulse = 5000.0f;
			float rotation_impulse = 3000.0f;
			float braking_impulse = 2500.0f;
			float max_velocity = 50.0f;
			float max_rotation_velocity = 2.5f;
			int mash_number = 15;
			float mash_time_limit = 5000.0f;
		}

		namespace mothership {
			float mass = 20000000.0f;
		}

		namespace extractor {
			float mass = 2000000000.0f;
			float resource_respawn_time = 30.0f;
		}

		namespace powerup {
			float max_velocity_rate = 2.0f;
			float impulse_rate = 1.20f;
			float pulse_rate = 20.0f;
			float speedup_time = 7000;
			float shield_time = 7000;
			float pulse_time = 250;

		}
	}
}