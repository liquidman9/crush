/*
* GameState.h
*/

#pragma once


// Global includes
#include <vector>
#include <memory>
#include <assert.h>

// Project includes
#include <shared/game/Entity.h>
#include <shared/game/Ship.h>
#include <shared/game/TractorBeam.h>
#include <shared/game/Mothership.h>
#include <shared/game/Resource.h>
#include <shared/game/Asteroid.h>

#define GS_MAX_MSG_SIZE 25

typedef vector<pair<unsigned int, int>> scoreList_t;

template <class E>
class GameState
{
public:

	GameState(void):m_entities(){
		m_meta.size = sizeof(gameStateMeta);
		/*m_serverMessages[0] = "New client has connected to the server";
		m_serverMessages[1] = "Client has disconnected from the server";
		m_serverMessages[2] = "Game starts in: ";*/
	};

	GameState(GameState const & g): m_entities(g.m_entities), m_meta(g.m_meta) {
	};

	void push_back(E *e) {
		m_meta.size += e->size();
		m_entities.push_back(shared_ptr<E>(e));
	};

	unsigned int size() const {
		return m_entities.size();
	}

	unsigned int sendSize() const {
		return m_meta.size;
	}

	shared_ptr<E> operator[](unsigned int i) {
		return m_entities[i];
	}

	shared_ptr<const E> operator[](unsigned int i) const {
		return m_entities[i];
	}

	void setTime(long long const &time) {
		m_meta.time = (int) time;
	}

	wstring getRemainingTimeString() const {
		string hours = to_string((long long)(m_meta.time / (1000*60*60)));
		string minutes = to_string((long long)(m_meta.time % (1000*60*60))/(1000*60));
		string seconds = to_string((long long)((m_meta.time % (1000*60*60)) % (1000*60))/1000);
		if((long long)((m_meta.time % (1000*60*60)) % (1000*60))/1000 < 10) {
			seconds = "0" + seconds;
		}
		if(hours != "0"){
			string s(hours + ":" + minutes + ":" + seconds);
			wstring rtn(s.begin(), s.end());
			return rtn;
		} else {
			string s(minutes + ":" + seconds);
			wstring rtn(s.begin(), s.end());
			return rtn;
		}
	}

	long long getRemainingTimeNum() const {
		return m_meta.time;
	}

	void setWinner(int client) {
		m_meta.winner = client;
	}

	int getWinner() {
		return m_meta.winner;
	}

	bool isWinner() {
		return m_meta.winner >= 0;
	}

	bool isServerMessage() {
		return message > 0;
	}

	void setScore(scoreList_t const &list) {
		for(auto it = list.begin(); it != list.end(); it++) {
			m_meta.score[it->first] = it->second;
		}
	}

	scoreList_t getScore() {
		scoreList_t rtn;
		for(unsigned int i = 0; i < 4; i--) {
			if(m_meta.score[i] >= 0) {
				rtn.push_back(pair<unsigned int, int>(i, m_meta.score[i]));
			}
		}
		return rtn;
	}

	/*string getServerMessage() {
		return m_serverMessage[m_meta.message];
	}*/

	bool isGameOver() {
		return m_meta.time == 0;
	}

	void setGameInProgress() {
		m_meta.gameInProgress = true;
	}

	void setGameOver() {
		m_meta.gameInProgress = false;
	}

	void clear() {
		m_entities.clear();
		m_meta.size = sizeof(gameStateMeta);
	}

	virtual ~GameState(void) {
		m_entities.clear();
	};

private:
	int getExpectedSize(const char* head, const unsigned int size) const {
		assert(size >= sizeof(m_meta));
		auto gs_size = getRecvSize(head);
		gameStateMeta gs_meta;
		memcpy((char* ) &gs_meta, head, sizeof(m_meta));
		unsigned int cur_size;
		bool valid_gameState = false;
		for(cur_size = 0; cur_size < size; cur_size += gs_size) {
			if(gs_size == 0) {
				return -1;
			}
			gs_size = getRecvSize(head+cur_size);
			if(cur_size + gs_size <= size) {
				valid_gameState = true;
			}
		}
		if (valid_gameState && cur_size != size) {
			return cur_size - gs_size;
		}
		return cur_size;
	}

	int getLastCompleteGS(const char* &head, const unsigned int size) const {
		//char local_buf[65000];
		assert(size >= sizeof(m_meta));
		//memcpy(local_buf,head,size);
		auto orig_head = head;
		unsigned int total_size = 0;
		auto gs_size = getRecvSize(head);
		unsigned int cur_size;
		for(cur_size = 0; cur_size < size; cur_size += gs_size) {
			gs_size = getRecvSize(orig_head+cur_size);
			assert(gs_size >= sizeof(m_meta));
			auto next_size = cur_size + gs_size;
			if(gs_size == gsMinSize()) {
				head = orig_head + cur_size;
				return size - (next_size);
			} else if(next_size <= size) {
				total_size = next_size;
				head = orig_head + cur_size;
			}
		}
		return size - total_size;
	}

	int decode(const char *head, const unsigned int size) {
		auto orig_head = head;
		auto rtn = getLastCompleteGS(head, size);
		if(rtn < 0) {
			return rtn;
		}
		memcpy((char* ) &m_meta, head, sizeof(m_meta));
		auto gs_size = m_meta.size;
		const char* cur_head = head + sizeof(m_meta);
			Entity *ep = NULL;
			clear();
			//int test = head - orig_head;
			Type a;
			//assert(head - orig_head >= 0);
			for(unsigned int cur_size = 0; cur_size < gs_size - sizeof(m_meta); cur_size += ep->size() ){
				ep = NULL;
				a = *(Type*) (cur_head + cur_size);
				switch(a) {
				case SHIP: 
					ep = new C_Ship();
					ep->decode(cur_head + cur_size);
					this->push_back(ep);
					break;
				case ASTEROID:
					ep = new C_Asteroid();
					ep->decode(cur_head + cur_size);
					this->push_back(ep);
					break;
				case TRACTORBEAM:
					ep = new C_TractorBeam();
					ep->decode(cur_head + cur_size);
					this->push_back(ep);
					break;
				case MOTHERSHIP:
					ep = new C_Mothership();
					ep->decode(cur_head + cur_size);
					this->push_back(ep);
					break;
				case RESOURCE:
					ep = new C_Resource();
					ep->decode(cur_head + cur_size);
					this->push_back(ep);
					break;
				default:
					cerr << "ERROR decoding entity state. Unknown type: " << (int) (*(ENUM_TYPE*)( cur_head + cur_size)) << endl;
					break;
				}
			}
			return rtn;
		//}
	}

	const char * getSendBuff() const {
		char *send_buff = new char[m_meta.size];
		memcpy(send_buff,(char*) &m_meta, sizeof(m_meta));
		int total_size = sizeof(m_meta);
		for(unsigned int i = 0; i < m_entities.size(); i++) {
			const char* tmp = m_entities[i]->encode();
			memcpy(send_buff + total_size, tmp, m_entities[i]->size());
			total_size += m_entities[i]->size();
			delete []tmp;
		}		
		return send_buff;
	}

	unsigned int getRecvSize(const char *a) const {
		return *(unsigned int*) a;
	}

	int gsMinSize() const {
		return sizeof(m_meta);
	}

	struct gameStateMeta {
		unsigned int size;
		int time;
		int score[4];
		char winner;
	} m_meta;

	//static string m_serverMessages[3];

	vector<shared_ptr<E> > m_entities;
	friend class NetworkServer;
	friend class NetworkClient;

};

