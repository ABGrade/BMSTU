#ifndef __MEASURE2_H__
#define __MEASURE2_H__

#define SKEWED 0
#define BALANCED 1
#define BINARY 2
#define CHAIN 3
#define OPEN 4

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
#define NUM_ITERATIONS 15

void measure();

#endif // __MEASURE2_H__