#ifndef SERVER_GLOBALS_H_INCLUDED
#define SERVER_GLOBALS_H_INCLUDED

#include <shared/util/SharedUtils.h>
#include <shared/ConfigSettings.h>
#include <string.h>
#include <stdio.h>

namespace server {
	static const string CONFIG_PREFIX = "server_";
	namespace game {
		static const string CONFIG_PREFIX = "game_";

		extern int timelimit;

		// INIT SERVER PROPERTIES
		inline void initFromConfig(string prefix) {
			ConfigSettings::config->getValue(prefix + CONFIG_PREFIX + "timelimit", timelimit);
		}
	}

	namespace world {
		static const string CONFIG_PREFIX = "world_";

		extern float asteroids_num;
		extern float asteroids_pos_start;
		extern float asteroids_pos_range;
		extern float asteroids_scale_start;
		extern float asteroids_scale_range;
		extern float asteroids_vel_range;
		extern float asteroids_rot_vel_range;
		
		// INIT WORLD PROPERTIES
		inline void initFromConfig(string prefix) {
			ConfigSettings::config->getValue(prefix + CONFIG_PREFIX + "asteroids_num", asteroids_num);
			ConfigSettings::config->getValue(prefix + CONFIG_PREFIX + "asteroids_pos_start", asteroids_pos_start);
			ConfigSettings::config->getValue(prefix + CONFIG_PREFIX + "asteroids_pos_range", asteroids_pos_range);
			ConfigSettings::config->getValue(prefix + CONFIG_PREFIX + "asteroids_scale_start", asteroids_scale_start);
			ConfigSettings::config->getValue(prefix + CONFIG_PREFIX + "asteroids_scale_range", asteroids_scale_range);
			ConfigSettings::config->getValue(prefix + CONFIG_PREFIX + "asteroids_vel_range", asteroids_vel_range);
			ConfigSettings::config->getValue(prefix + CONFIG_PREFIX + "asteroids_rot_vel_range", asteroids_rot_vel_range);
		}
	}

	namespace entities {
		static const string CONFIG_PREFIX = "entities_";

		// S_Asteroid
		namespace asteroid {
			static const string CONFIG_PREFIX = "asteroid_";

			extern float startScale;
			extern float rangeScale;
			extern float startPos;
			extern float rangePos;
			extern float numAsteroids;
			
			extern float scaleToRadius;
			extern float density;

			inline void initFromConfig(string prefix) {
				ConfigSettings::config->getValue(prefix + CONFIG_PREFIX + "scaleToRadius", scaleToRadius);
				ConfigSettings::config->getValue(prefix + CONFIG_PREFIX + "density", density);
			}
		}
		// S_Ship
		namespace ship {
			static const std::string CONFIG_PREFIX = "ship_";

			// Property declarations
			extern float mass;
			extern float forward_impulse;
			extern float rotation_impulse;
			extern float braking_impulse;
			extern float max_velocity;
			extern float max_rotation_velocity;

			inline void initFromConfig(string prefix) {
				ConfigSettings::config->getValue(prefix + CONFIG_PREFIX + "mass", mass);
				ConfigSettings::config->getValue(prefix + CONFIG_PREFIX + "forward_impulse", forward_impulse);
				ConfigSettings::config->getValue(prefix + CONFIG_PREFIX + "rotation_impulse", rotation_impulse);
				ConfigSettings::config->getValue(prefix + CONFIG_PREFIX + "braking_impulse", braking_impulse);
				ConfigSettings::config->getValue(prefix + CONFIG_PREFIX + "max_velocity", max_velocity);
				ConfigSettings::config->getValue(prefix + CONFIG_PREFIX + "max_rotation_velocity", max_rotation_velocity);
			}
		}

		// S_TractorBeam
		namespace tractorbeam {
			static const std::string CONFIG_PREFIX = "tractorbeam_";

			// Property declarations
			extern float power;
			extern float length;

			inline void initFromConfig(string prefix) {
				ConfigSettings::config->getValue(prefix + CONFIG_PREFIX + "power", power);
				ConfigSettings::config->getValue(prefix + CONFIG_PREFIX + "length", length);
			}
		}

		// S_Mothership
		namespace mothership {
			static const std::string CONFIG_PREFIX = "mothership_";

			// Property declarations
			extern float mass;

			inline void initFromConfig(string prefix) {
				ConfigSettings::config->getValue(prefix + CONFIG_PREFIX + "mass", mass);
			}
		}

		namespace extractor {
			static const std::string CONFIG_PREFIX = "extractor_";

			// Property declarations
			extern float mass;
			extern float resource_respawn_time;

			inline void initFromConfig(string prefix) {
				ConfigSettings::config->getValue(prefix + CONFIG_PREFIX + "mass", mass);
				ConfigSettings::config->getValue(prefix + CONFIG_PREFIX + "resource_respawn_time", resource_respawn_time);
			}
		}

		// INIT ALL ENTITIES
		inline void initFromConfig(string prefix) {
			asteroid::initFromConfig(prefix + CONFIG_PREFIX);
			ship::initFromConfig(prefix + CONFIG_PREFIX);
			tractorbeam::initFromConfig(prefix + CONFIG_PREFIX);
			mothership::initFromConfig(prefix + CONFIG_PREFIX);
			extractor::initFromConfig(prefix + CONFIG_PREFIX);
		}

	}

	inline void initFromConfig() {
		game::initFromConfig(CONFIG_PREFIX);
		world::initFromConfig(CONFIG_PREFIX);
		entities::initFromConfig(CONFIG_PREFIX);
	}
}

#endif // SERVER_GLOBALS_H_INCLUDED
