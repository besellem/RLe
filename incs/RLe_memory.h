/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RLe_memory.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/13 21:36:52 by besellem          #+#    #+#             */
/*   Updated: 2022/02/15 23:26:48 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RLE_MEMORY_H
# define RLE_MEMORY_H

#include "RLe_types.h"
#include "RLe_parsing.h"
#include "RLe_buffer.h"
#include "RLe_error.h"
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>


/* Buffer size for reading files */
#ifndef BUFF_SIZE
// 32 128 256 512 1024 2048 4096 8192 16384 32768 65536 131072 262144 524288 1048576
# define BUFF_SIZE   65536
#endif

/* Min size of the buffer */
#if BUFF_SIZE < 2
# error "BUFF_SIZE must be at least 2"
#endif


/* modes */
#define RLE_ENCODE                  0
#define RLE_DECODE                  1
#define RLE_DEFAULT_MODE            RLE_ENCODE

/* algorithms */
#define RLE_ALGO_RUN_LENGTH         "run-length"
#define RLE_ALGO_RUN_LENGTH_ESCAPE  "run-length-escape"


typedef	struct	RLE_process_tracking_s
{
	int			input_perc; // % of input file read
	int			algo_efficiency;
	long long	bytes_read;
	long long	bytes_written;
}				RLE_process_tracking_t;

typedef	struct	RLE_params_s
{
	t_options_parsing	parsing_opts;
	uint				mode;  // 0 = encode, 1 = decode
	char				*algo; // name of the algorithm

	// input / output streams & files
	char				*input_file;
	char				*output_file;
	FILE				*input_stream;
	FILE				*output_stream;
	struct stat			in_st; // const
	
	// algorithm function
	void				(*func)(struct RLE_params_s *);
	
}				RLE_params_t;


#define RLE_alloc(__size) \
	_RLE_alloc((__size), __FILE__, __func__, __LINE__)

#define RLE_realloc(__ptr, __size) \
	_RLE_realloc((__ptr), (__size), __FILE__, __func__, __LINE__)

#define RLE_fread(__buf, __nbyte, __stream) \
	_RLE_fread((__buf), (__nbyte), (__stream), __FILE__, __LINE__)


void					*_RLE_alloc(size_t, const char *, const char *, size_t);
void					*_RLE_realloc(void *, size_t, const char *, const char *, size_t);
size_t					_RLE_fread(buf_type *, size_t, FILE *, const char *, size_t);
RLE_params_t			*RLE_get_params(void);
RLE_process_tracking_t	RLE_get_process_tracking(void);
void					RLE_free_all(void);


#endif /* !defined(RLE_MEMORY_H) */
