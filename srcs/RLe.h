/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RLe.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/04 14:46:12 by besellem          #+#    #+#             */
/*   Updated: 2022/01/04 22:27:03 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RLE_H
# define RLE_H

#include "RLe_buffer.h"
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


/*
** Macro-like functions
*/
#define error(msg) \
	{ fprintf(stderr, "%s:%d: %s\n", __FILE__, __LINE__, (msg)); exit(EXIT_FAILURE); }

#define MAX(a, b)  ((a) > (b) ? (a) : (b))
#define MIN(a, b)  ((a) < (b) ? (a) : (b))
#define DIM(a, b)  ((a) - (b) > 0 ? (a) - (b) : 0)


/*
** Data structures
*/
enum
{
	MODE_ENCODE,
	MODE_DECODE
};


/*
** Functions
*/
void		RLE_EncodeGeneric(int in);
void		RLE_DecodeGeneric(int in);

void		RLE_RunLengthEncode(int in);
void		RLE_RunLengthDecode(int in);
void		RLE_RunLengthEscapeEncode(int in);
void		RLE_RunLengthEscapeDecode(int in);

void		RLE_BurrowsWheelerTransformEncode(uint8_t *data, size_t size);


#endif
