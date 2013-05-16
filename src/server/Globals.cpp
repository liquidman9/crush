#include <server/Globals.h>

namespace server {
	namespace game {
		int timelimit = 5 * 60 * 1000;
	}

	namespace world {
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
			float scaleToRadius = 2.0f;
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
		}

		namespace mothership {
			float mass = 2000000000.0f;
		}
	}
}