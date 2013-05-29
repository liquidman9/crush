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
#include <shared/game/Extractor.h>
#include <shared/game/Powerup.h>

#define GS_MAX_MSG_SIZE 25

typedef vector<pair<unsigned int, int>> scoreList_t;

template <class E>
class GameState
{
public:

	GameState(void):m_entities(){
		m_size = gsMinSize();
		memset(&m_meta,-1,sizeof(m_meta));
	};

	GameState(GameState const & g): m_entities(g.m_entities), m_meta(g.m_meta) {
		m_size = g.m_size;	
	};

	void push_back(E *e) {
		if (getEntityById(e->m_id) == NULL) {
			m_size += e->size();
			m_entities.push_back(shared_ptr<E>(e));
		}
	};

	unsigned int size() const {
		return m_entities.size();
	}

	unsigned int sendSize() const {
		return m_size;
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

	void setServerTime(long long const &time) {
		m_meta.serverTime = time;
	}

	long long getServerTime() const {
		return m_meta.serverTime;
	}

	shared_ptr<E> getEntityById(int id) {
		for (unsigned int i = 0; i < size(); i++) {
			if (m_entities[i]->m_id == id) return m_entities[i];
		}

		return NULL;
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
		for(unsigned int i = 0; i < 4; i++) {
			if(m_meta.score[i] >= 0) {
				rtn.push_back(pair<unsigned int, int>(i, m_meta.score[i]));
			}
		}
		return rtn;
	}

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
		m_size = gsMinSize();
	}

	bool empty() const {
		return m_entities.empty();
	}

	virtual ~GameState(void) {
		m_entities.clear();
	}

	static unsigned int  minSize() {
		//size of m_meta and size of m_size
		return sizeof(gameStateMeta) + sizeof(unsigned int);
	}

private:

	void decode(const char *head, unsigned int size) {
		auto orig_head = head;	
		m_size = *(unsigned int *) head;
		head += sizeof(m_size);
		memcpy((char* ) &m_meta, head, sizeof(m_meta));
		auto gs_size = m_size - sizeof(m_size) - sizeof(m_meta);
		const char* cur_head = head + sizeof(m_meta);
		Entity *ep = NULL;
		clear();		
		Type a;
		for(unsigned int cur_size = 0; cur_size < gs_size; cur_size += ep->size() ){
			ep = NULL;
			a = *(Type*) (cur_head + cur_size);
			switch(a) {
			case SHIP: 
				ep = new Ship();
				ep->decode(cur_head + cur_size);
				this->push_back(ep);
				break;
			case ASTEROID:
				ep = new Asteroid();
				ep->decode(cur_head + cur_size);
				this->push_back(ep);
				break;
			case TRACTORBEAM:
				ep = new TractorBeam();
				ep->decode(cur_head + cur_size);
				this->push_back(ep);
				break;
			case MOTHERSHIP:
				ep = new Mothership();
				ep->decode(cur_head + cur_size);
				this->push_back(ep);
				break;
			case RESOURCE:
				ep = new Resource();
				ep->decode(cur_head + cur_size);
				this->push_back(ep);
				break;
			case EXTRACTOR:
				ep = new Extractor();
				ep->decode(cur_head + cur_size);
				this->push_back(ep);
				break;
			case POWERUP:
				ep = new Powerup();
				ep->decode(cur_head + cur_size);
				this->push_back(ep);
				break;
			default:
				cerr << "ERROR decoding entity state. Unknown type: " << (int) (*(ENUM_TYPE*)( cur_head + cur_size)) << endl;
				break;
			}
		}
	}

	const char * getSendBuff() const {
		char *send_buff = new char[m_size];
		 *(unsigned int*) send_buff = m_size;
		memcpy(send_buff+sizeof(m_size),(char*) &m_meta, sizeof(m_meta));
		int total_size = sizeof(m_meta) + sizeof(m_size);
		for(unsigned int i = 0; i < m_entities.size(); i++) {
			total_size += m_entities[i]->encode(send_buff+total_size);
		}
		return send_buff;
	}

	unsigned int getRecvSize(const char *a) const {
		return *(unsigned int*) a;
	}

	unsigned gsMinSize() const {
		return minSize();
	}

	struct gameStateMeta {	
		int time;
		long long serverTime;
		int score[4];
		char winner;
	} m_meta;

	unsigned int m_size;
	//static string m_serverMessages[3];

	vector<shared_ptr<E> > m_entities;
	friend class NetworkServer;
	friend class NetworkClient;
	friend class Server;

};

