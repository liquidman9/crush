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

		extern float size;

		extern float mothership_distance_from_center;
		extern float ship_spawn_distance_from_center;

		extern float asteroids_num;
		extern float asteroids_pos_start;
		extern float asteroids_pos_range;
		extern float asteroids_scale_start;
		extern float asteroids_scale_range;
		extern float asteroids_vel_range;
		extern float asteroids_rot_vel_range;
		
		// INIT WORLD PROPERTIES
		inline void initFromConfig(string prefix) {
			ConfigSettings::config->getValue(prefix + CONFIG_PREFIX + "world_size", size);

			ConfigSettings::config->getValue(prefix + CONFIG_PREFIX + "mothership_distance_from_center", mothership_distance_from_center);
			ConfigSettings::config->getValue(prefix + CONFIG_PREFIX + "ship_spawn_distance_from_center", ship_spawn_distance_from_center);

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
			extern float linear_impulse;
			extern float rotation_impulse;
			extern float hard_braking_impulse;
			extern float braking_impulse;
			extern float max_velocity;
			extern float max_rotation_velocity;
			extern int mash_number;
			extern float mash_time_limit;

			inline void initFromConfig(string prefix) {
				ConfigSettings::config->getValue(prefix + CONFIG_PREFIX + "mass", mass);
				ConfigSettings::config->getValue(prefix + CONFIG_PREFIX + "linear_impulse", linear_impulse);
				ConfigSettings::config->getValue(prefix + CONFIG_PREFIX + "rotation_impulse", rotation_impulse);
				ConfigSettings::config->getValue(prefix + CONFIG_PREFIX + "hard_braking_impulse", braking_impulse);
				ConfigSettings::config->getValue(prefix + CONFIG_PREFIX + "braking_impulse", braking_impulse);
				ConfigSettings::config->getValue(prefix + CONFIG_PREFIX + "max_velocity", max_velocity);
				ConfigSettings::config->getValue(prefix + CONFIG_PREFIX + "max_rotation_velocity", max_rotation_velocity);
				ConfigSettings::config->getValue(prefix + CONFIG_PREFIX + "mash_number", mash_number);
				ConfigSettings::config->getValue(prefix + CONFIG_PREFIX + "mash_time_limit", mash_time_limit);
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

		namespace resource {
			static const std::string CONFIG_PREFIX = "resource_";

			// Property declarations
			extern float max_travel_time;

			inline void initFromConfig(string prefix) {
				ConfigSettings::config->getValue(prefix + CONFIG_PREFIX + "max_travel_time", max_travel_time);
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

		namespace powerup {
			static const std::string CONFIG_PREFIX = "powerup_";

			// Property declarations
			extern float max_velocity_rate;
			extern float impulse_rate;
			extern float pulse_rate;
			extern float speedup_time;
			extern float shield_time;
			extern float pulse_time;
			extern float pulse_range;

			inline void initFromConfig(string prefix) {
				ConfigSettings::config->getValue(prefix + CONFIG_PREFIX + "max_velocity_rate", max_velocity_rate);
				ConfigSettings::config->getValue(prefix + CONFIG_PREFIX + "impulse_rate", impulse_rate);
				ConfigSettings::config->getValue(prefix + CONFIG_PREFIX + "pulse_rate", pulse_rate);
				ConfigSettings::config->getValue(prefix + CONFIG_PREFIX + "speedup_time", speedup_time);
				ConfigSettings::config->getValue(prefix + CONFIG_PREFIX + "shield_time", shield_time);
				ConfigSettings::config->getValue(prefix + CONFIG_PREFIX + "pulse_time", pulse_time);
				ConfigSettings::config->getValue(prefix + CONFIG_PREFIX + "pulse_range", pulse_range);
			}
		}

		// INIT ALL ENTITIES
		inline void initFromConfig(string prefix) {
			asteroid::initFromConfig(prefix + CONFIG_PREFIX);
			ship::initFromConfig(prefix + CONFIG_PREFIX);
			tractorbeam::initFromConfig(prefix + CONFIG_PREFIX);
			mothership::initFromConfig(prefix + CONFIG_PREFIX);
			extractor::initFromConfig(prefix + CONFIG_PREFIX);
			powerup::initFromConfig(prefix + CONFIG_PREFIX);
			resource::initFromConfig(prefix + CONFIG_PREFIX);
		}

	}

	inline void initFromConfig() {
		game::initFromConfig(CONFIG_PREFIX);
		world::initFromConfig(CONFIG_PREFIX);
		entities::initFromConfig(CONFIG_PREFIX);
	}
}

#endif // SERVER_GLOBALS_H_INCLUDED
