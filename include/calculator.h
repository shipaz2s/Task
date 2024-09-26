#pragma once

#ifdef _WIN32
#ifdef Calculator_EXPORTS
#define Calculator_API __declspec(dllexport)
#else
#define Calculator_API __declspec(dllimport)
#endif
#else //not WIN32
#define Calculator_API
#endif //if WIN32


#include <stdint.h>
#include <limits.h>

#define ROFFSET 61
int64_t PART_MAX_VAL = (1LLU << 61) - 1;					//0x0001 11111.......
int64_t PART_MIN_VAL = ( (1LLU << 61) - 1 ) | (1LLU << 63);	//0x1001 11111.......
uint64_t NUM_POS_MASK = (0LLU - 1LLU) >> 4;					//0x0001 111111...
uint64_t NUM_MASK = ( (0LLU - 1LLU) >> 4 ) | (1LLU << 63);	//0x1001 111111...
uint64_t NUM_MINUS_MASK = (1LLU << 63);						//0x1000 000000...

#define NUM_BASE	16
#define PRECISION	8
#define NUM_SIZE	32

struct Fixed
{
	int8_t flag;
	int8_t num[NUM_SIZE];
} //Fixed_def = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
Fixed_def = {0, 0};

typedef struct Fixed sFixed;

Calculator_API sFixed str2Fixed(const char*);
Calculator_API char* Fixed2str(sFixed* const, char* buff);
Calculator_API uint8_t FixedLess(sFixed const* v1, sFixed const* v2); // v1 less than v2
Calculator_API uint8_t FixedEqual(sFixed const* v1, sFixed const* v2);
Calculator_API sFixed FixedAbs(sFixed const* v);
Calculator_API sFixed FixedShiftL(sFixed const* v, int n);
Calculator_API sFixed FixedShiftR(sFixed const* v, int n);

Calculator_API sFixed FixedAdd(sFixed const* v1, sFixed const* v2);
Calculator_API sFixed FixedSub(sFixed const* v1, sFixed const* v2);

Calculator_API sFixed FixedMul(sFixed const* v1, sFixed const* v2);
Calculator_API sFixed FixedDiv(sFixed const* v1, sFixed const* v2);
