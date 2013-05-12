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

#ifdef ENABLE_COMPRESSION
#include<shared/network/minilzo.h>
#endif
#define GS_MAX_MSG_SIZE 25

typedef vector<pair<unsigned int, int>> scoreList_t;

template <class E>
class GameState
{
public:

	GameState(void):m_entities(){
		m_meta.size = sizeof(gameStateMeta);
		//#ifdef ENABLE_COMPRESSION
		//		m_cSize = m_meta.size;
		//#endif
		/*m_serverMessages[0] = "New client has connected to the server";
		m_serverMessages[1] = "Client has disconnected from the server";
		m_serverMessages[2] = "Game starts in: ";*/
	};

	GameState(GameState const & g): m_entities(g.m_entities), m_meta(g.m_meta) {
		//#ifdef ENABLE_COMPRESSION
		//	m_cSize = g.m_cSize;
		//#endif
	};

	void push_back(E *e) {
#ifdef ENABLE_DELTA
		m_buffSize += e->deltaSize();
#else
		m_buffSizee += e->size();
#endif

		m_entities.push_back(shared_ptr<E>(e));
	};

	unsigned int size() const {
		return m_entities.size();
	}

	unsigned int sendSize() const {
		//#ifdef ENABLE_COMPRESSION
		//	return m_cSize;
		//#endif
		return m_meta.size;
	}

	shared_ptr<E>& operator[](unsigned int i) {
		return m_entities[i];
	}

	shared_ptr<const E>& operator[](unsigned int i) const {
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


	bool empty() const {
		return m_entities.empty();
	}

	virtual ~GameState(void) {
		m_entities.clear();
	};

private:
   	void push_back_size(Entity * e, unsigned int size) {
		m_meta.size += size;
		m_entities.push_back(shared_ptr<E>(e));
	}

	bool getIsEmpty(const char* head) const {
#ifdef ENABLE_COMPRESSION
		return *(bool*)(head + sizeof(unsigned int));
#else
		return getRecvSize(head) == gsMinSize();
#endif
	}

#ifdef ENABLE_COMPRESSION
	void setIsEmpty(const char* head, bool isEmpty) const {
		*(bool*)(head + sizeof(unsigned int)) = isEmpty;
	}
#endif

	unsigned int getExpectedSize(char* head, const unsigned int size) const {
		assert(size >= gsMinSize());
		return getRecvSize(head);
		//auto gs_size = getRecvSize(head);
		//gameStateMeta gs_meta;
		//memcpy((char* ) &gs_meta, head, gsMinSize());
		//unsigned int cur_size;
		//bool valid_gameState = false;
		/*for(cur_size = 0; cur_size < size; cur_size += gs_size) {
		gs_size = getRecvSize(head+cur_size);
		if(gs_size == 0) {
		return -1;
		}			
		if(cur_size + gs_size <= size) {
		valid_gameState = true;
		}
		}
		if (valid_gameState && cur_size != size) {
		return cur_size - gs_size;
		}
		return cur_size;*/
	}

	int getLastCompleteGS(char* &head, const unsigned int size, unsigned int &dropped) const {
		assert(size >= gsMinSize());
		auto gs_size = getRecvSize(head);

#ifdef ENABLE_DELTA
		return size - gs_size;
#else
		auto orig_head = head;
		unsigned int total_size = 0;
		unsigned int cur_size;
		for(cur_size = 0; cur_size < size; cur_size += gs_size) {

#ifndef ENABLE_COMPRESSION			
			assert(gs_size >= gsMinSize());
#endif
			gs_size = getRecvSize(orig_head+cur_size);
			auto next_size = cur_size + gs_size;
			if(getIsEmpty(orig_head+cur_size)) {
				head = orig_head + cur_size;
				dropped = 0;
				return size - (next_size);
			} else if(next_size <= size) {
				dropped++;
				total_size = next_size;
				head = orig_head + cur_size;
			}
		}

		return size - total_size;
#endif
	}

	int decode(char *head, unsigned int size, unsigned int &dropped) {
		auto orig_head = head;
		auto rtn = getLastCompleteGS(head, size, dropped);
		if(rtn < 0) {
			return rtn;
		}
#ifdef ENABLE_COMPRESSION
		static bool init = false;
		if(!init){
			lzo_init();
			init = true;
		}
		auto c_size = getRecvSize(head);
		unsigned int c_len = (unsigned int) c_size - sizeof(unsigned int);
		unsigned long d_len;
		unsigned char* d_out = new unsigned char[size*4];
		//skip over stored compressed_size
		unsigned char* c_in = (unsigned char*) head + sizeof(unsigned int) + sizeof(bool);
		auto r = lzo1x_decompress(c_in,c_len,d_out,&d_len,NULL);
		if(r != LZO_E_OK) {
			cerr << "OH GOD WE GUNNA CRASH (decompress failed)" << endl;
		}
		size = d_len;

		head = (char*) d_out;
#endif

#ifdef ENABLE_DELTA
		decodeDeltaState(head);
#else
		decodeNew(head);
#endif

#ifdef ENABLE_COMPRESSION
		delete []d_out;
#endif
		return rtn;
	}

	char * getSendBuff() {
		char *send_buff = new char[10000];
		int total_size = sizeof(m_meta);
		for(unsigned int i = 0; i < m_entities.size(); i++) {			
			total_size += m_entities[i]->encode(send_buff+total_size);
		}
		//assert(total_size <= m_meta.size);
		m_meta.size = total_size;
		memcpy(send_buff,(char*) &m_meta, sizeof(m_meta));
#ifdef ENABLE_COMPRESSION
		static bool init = false;
		if(!init){
			lzo_init();
			init = true;
		}
		char *out_tmp = new char[2*m_meta.size];
		unsigned char scratch [LZO1X_1_MEM_COMPRESS];
		unsigned char* in = (unsigned char *) send_buff;
		unsigned char* out = (unsigned char*) (out_tmp + sizeof(unsigned int) + sizeof(bool));
		lzo_uint in_len = total_size;
		lzo_uint out_len;
		lzo1x_1_compress(in,in_len,out,&out_len,scratch);
		*(lzo_uint*) out_tmp =  out_len + sizeof(unsigned int) + sizeof(bool);
		setIsEmpty(out_tmp, empty());
		delete []send_buff;
		return out_tmp;
#else
		return send_buff;
#endif
	}

	void decodeNew(char *head) {
		memcpy((char* ) &m_meta, head, sizeof(m_meta));
		auto gs_size = m_meta.size;
		char* cur_head = head + sizeof(m_meta);
		Entity *ep = NULL;
		clear();
		//int test = head - orig_head;
		//Type a;
		//assert(head - orig_head >= 0);
		for(unsigned int cur_size = 0; cur_size < gs_size - sizeof(m_meta);){
			Entity* ep = NULL;
			ep = decodeNewEntity(cur_size+cur_head, cur_size, false);
		}
	}

	Entity* decodeNewEntity(char *head, unsigned int &cur_size, bool isDelta) {
		Entity* ep = NULL;
		Type a = *(Type*) (head);
#ifdef ENABLE_DELTA
		if(isDelta) {
			auto tmp = Sendable::skipDeltaInfo(head);
			a = *(Type*) (head+tmp);
		}
#endif
		switch(a) {
		case SHIP: 
			ep = new Ship();
			cur_size += ep->decode(head);
			return ep;
		case ASTEROID:
			ep = new Asteroid();
			cur_size += ep->decode(head);
			return ep;
		case TRACTORBEAM:
			ep = new TractorBeam();
			cur_size += ep->decode(head);
			return ep;
		case MOTHERSHIP:
			ep = new Mothership();
			cur_size += ep->decode(head);
			return ep;
		case RESOURCE:
			ep = new Resource();
			cur_size += ep->decode(head);
			return ep;
		case EXTRACTOR:
			ep = new Extractor();
			cur_size += ep->decode(head);
			return ep;
		default:
			cerr << "ERROR decoding entity state. Unknown type: " << (int) (*(ENUM_TYPE*)(head)) << endl;
			assert(NULL);
			return NULL;
		}
	}

	void decodeDeltaState(char *head) {
		memcpy((char* ) &m_meta, head, sizeof(m_meta));
		auto gs_size = m_meta.size;
		char* cur_head = head + sizeof(m_meta);
		for(unsigned int cur_size = 0, i = 0; cur_size < gs_size - sizeof(m_meta); i++){
			if(Sendable::isNewObject(cur_head + cur_size)) {
				auto tmp = cur_size;
				push_back_size(decodeNewEntity(cur_head + cur_size, cur_size, true), cur_size - tmp);
				continue;
			}
				cur_size += m_entities[i]->decode(cur_head + cur_size);
		}
	}

#ifdef ENABLE_DELTA
	void resetDeltas() {
		for(unsigned int i = 0; i < m_entities.size(); i++) {
			delete []m_entities[i]->m_oldState;
			m_entities[i]->m_oldState = NULL;
		}
	}
#endif

	unsigned int getRecvSize(const char *a) const {
		return *(unsigned int*) a;
	}

	unsigned gsMinSize() const {
		return sizeof(m_meta);
	}

	struct gameStateMeta {
		unsigned int size;
		int time;
		int score[4];
		char winner;
	} m_meta;

	unsigned int m_buffSize;
	//static string m_serverMessages[3];

	vector<shared_ptr<E> > m_entities;
	friend class NetworkServer;
	friend class NetworkClient;
	friend class Server;

};
