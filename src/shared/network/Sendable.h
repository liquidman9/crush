#pragma once
#define ENABLE_DELTA

#ifdef ENABLE_DELTA
//must be a multiple of 2
//#define MAX_SEND_SIZE 128
#endif

class Sendable
{
public:

	virtual const unsigned int size() const = 0;
	virtual unsigned int encode(char *) const = 0;
	virtual unsigned int decode(const char *) = 0;
	
#ifdef ENABLE_DELTA
	/*typedef struct {
		unsigned field : MAX_SEND_SIZE;
	} deltaField;

	static void encodeDelta(char* buff, void *new_data, void *old_data, deltaField *dfield, unsigned int &curr_encode_size, unsigned int added_size) {
		char * np = (char *) new_data;
		char * op = (char *) old_data;
		for(unsigned int encode_count = 0; encode_count < added_size; ++curr_encode_size, ++encode_count) {
			if(*np = *op) {
				dfield->field |= 1 << curr_encode_size;
				buff[curr_encode_size] = old_data;
			} else {
				dfield->field &= ~(1 << curr_encode_size);
			}
		}
	}*/
#endif
};

