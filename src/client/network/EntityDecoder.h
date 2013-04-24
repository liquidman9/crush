#pragma once

#include <shared/game/GameState.h>
#include <shared/game/Entity.h>
#include <shared/game/Ship.h>
#include <client/graphics/entities/C_Ship.h>
#include <client/graphics/entities/C_Asteroid.h>

namespace client{
	namespace network {
		void decodeGameState(const char *head, unsigned int size, GameState<Entity> &g) {
	
			Entity* ep = NULL;
			const char* cur_head = head;
			for(unsigned int cur_size = 0; cur_size < size; cur_size += ep->size() ){
				ENUM_TYPE a = *(ENUM_TYPE*) head;
				switch(*(ENUM_TYPE*)( cur_head + cur_size)) {
					case SHIP: 
						ep = new C_Ship();
						ep->decode(cur_head + cur_size);
						g.push_back(ep);
						break;
					case ASTEROID:
						ep = new C_Asteroid();
						ep->decode(cur_head + cur_size);
						g.push_back(ep);
						break;
				}
			}
			 
		};
	}
}