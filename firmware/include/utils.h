#ifndef UTILS_H
#define UTILS_H
// macro for debugging
#define DEBUG
#ifdef DEBUG
#define DPRINTF(...) Serial.print(F(__VA_ARGS__))
#else
#define DPRINTF(...) 
#endif
#endif
