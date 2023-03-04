typedef unsigned int  u32; //makes it so typing u32 means usigned 32 bit integer

//rest copied from the tutorial's github
typedef long long s64;
typedef unsigned long long u64;
typedef char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;
typedef int s32;

#define global_variable static;
#define internal static;

inline int
clamp(int min, int val, int max) {
	if (val < min) return min;
	if (val > max) return max;
	return val;
}