#ifndef CONSTANTS_H
#define CONSTANTS_H

// ============================================
// STALE DLA EWALUATORA
// ============================================

#define WRONG_FITNESS_VALUE -1.0
#define DEFAULT_DEPOT_INDEX 1

// ============================================
// STALE DLA ALGORYTMU GENETYCZNEGO
// ============================================

#define DEFAULT_POP_SIZE 50
#define DEFAULT_CROSS_PROB 0.7
#define DEFAULT_MUT_PROB 0.1
#define DEFAULT_ITERATIONS 100
#define DEFAULT_NUM_GROUPS 5

// ============================================
// SLOWA KLUCZOWE W PLIKU LCVRP
// ============================================

#define KEYWORD_NAME "NAME"
#define KEYWORD_DIMENSION "DIMENSION"
#define KEYWORD_CAPACITY "CAPACITY"
#define KEYWORD_PERMUTATION "PERMUTATION"
#define KEYWORD_NODE_COORD "NODE_COORD_SECTION"
#define KEYWORD_DEMAND "DEMAND_SECTION"
#define KEYWORD_DEPOT "DEPOT_SECTION"
#define KEYWORD_EOF "EOF"
#define FILE_SEPARATOR ':'

// ============================================
// KOMUNIKATY BLEDOW
// ============================================

#define ERROR_MSG_FILE_NOT_FOUND "Nie mozna otworzyc pliku"
#define ERROR_MSG_INVALID_FORMAT "Nieprawidlowy format pliku"

#endif
