/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RLe.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/04 14:46:12 by besellem          #+#    #+#             */
/*   Updated: 2022/02/14 17:11:13 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RLE_H
# define RLE_H

#include "RLe_parsing.h"
#include "RLe_memory.h"
#include "RLe_buffer.h"
#include "RLe_error.h"

#include <stdio.h>
#include <libgen.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <stdint.h>
#include <sys/types.h>
#include <limits.h>
#include <errno.h>
#include <pthread.h>
#include <assert.h>


/*
** Defines
*/
#define SYSCALL_ERR  (-1)


/* Buffer size for reading files */
#ifndef BUFF_SIZE
// 32 128 256 512 1024 2048 4096 8192 16384 32768 65536 131072 262144 524288 1048576
# define BUFF_SIZE   65536
#endif

/* Min size of the buffer */
#if BUFF_SIZE < 2
# error "BUFF_SIZE must be at least 2"
#endif

#define RLE_ENCODE                  0
#define RLE_DECODE                  1
#define RLE_DEFAULT_MODE            RLE_ENCODE

#define RLE_ALGO_RUN_LENGTH         "run-length"
#define RLE_ALGO_RUN_LENGTH_ESCAPE  "run-length-escape"


/*
** Macro-like functions
*/
#define MAX(a, b)  ((a) > (b) ? (a) : (b))
#define MIN(a, b)  ((a) < (b) ? (a) : (b))
#define DIM(a, b)  ((a) - (b) > 0 ? (a) - (b) : 0)


/*
** Data structures
*/
struct	RLE_modes_s
{
	uint		mode; // 0 = encode, 1 = decode
	char		*name;
	void		(*func)(RLE_params_t *);
};

/*
** Functions
*/

void		RLE_RunGeneric(RLE_params_t *);

void		RLE_RunLengthEncode(RLE_params_t *);
void		RLE_RunLengthDecode(RLE_params_t *);
void		RLE_RunLengthEscapeEncode(RLE_params_t *);
void		RLE_RunLengthEscapeDecode(RLE_params_t *);

void		RLE_BurrowsWheelerTransformEncode(uint8_t *data, size_t size);


#endif
