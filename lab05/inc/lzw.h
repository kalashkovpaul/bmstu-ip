#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <sys/types.h>

#define LZW_MIN_CODE_WIDTH 9
#ifndef LZW_MAX_CODE_WIDTH
#define LZW_MAX_CODE_WIDTH 16
#endif
#define LZW_MAX_CODES (1UL << LZW_MAX_CODE_WIDTH)

enum lzw_errors {
	LZW_NOERROR = 0,
	LZW_DESTINATION_TOO_SMALL = -1,
	LZW_INVALID_CODE_STREAM = -2,
	LZW_STRING_TABLE_FULL = -3,
};

#if LZW_MAX_CODE_WIDTH > 12
typedef uint64_t lzw_node;
#else
typedef uint32_t lzw_node;
#endif
typedef uint32_t bitres_t;
typedef uint16_t code_t;
typedef uint8_t sym_t;

struct lzw_string_table {
	uint32_t code_width;
	code_t next_code;
	code_t prev_code;
	lzw_node node[LZW_MAX_CODES];
};

struct lzw_state {
	struct lzw_string_table tree;
	bool	 was_init;
	bool	 must_reset;
	size_t rptr;
	size_t wptr;
	bitres_t bitres;
	uint32_t bitres_len;
	size_t longest_prefix;
	size_t longest_prefix_allowed;
};

const char *lzw_strerror(enum lzw_errors errnum);

ssize_t lzw_decompress(struct lzw_state *state, uint8_t *src, size_t slen, uint8_t *dest, size_t dlen);

ssize_t lzw_compress(struct lzw_state *state, uint8_t *src, size_t slen, uint8_t *dest, size_t dlen);

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>
#include <stdbool.h>
