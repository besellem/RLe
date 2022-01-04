/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RLe.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/02 19:15:30 by besellem          #+#    #+#             */
/*   Updated: 2022/01/04 14:09:46 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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


#define PRG_NAME     "RLe"


#define USAGE(av) \
	fprintf(stdout, "Usage: %s <input_file> <output_file>\n" \
					"       %s -d <input_file>\n", av[0], av[0]);


#define error(msg)   {fprintf(stderr, "%s:%d: %s\n", __FILE__, __LINE__, (msg)); exit(EXIT_FAILURE);}


#define SYSCALL_ERR  (-1)
#define MODE_COMP	 1
#define MODE_DECOMP	 2


#ifndef BUFF_SIZE
# define BUFF_SIZE   65536 // 32 128 256 512 1024 2048 4096 8192 16384 32768 65536 131072 262144 524288 1048576 2097152 4194304 8388608 16777216 33554432 67108864 134217728
#endif
#if BUFF_SIZE < 2
# error "BUFF_SIZE must be at least 2"
#endif


#define MAX(a, b)	 ((a) > (b) ? (a) : (b))
#define MIN(a, b)	 ((a) < (b) ? (a) : (b))


/* flush buffer */
#define RLE_FlushBuffer(fd) \
	write((fd), g_output, g_output_size);

/* bufferize or write directly */
#define RLE_WriteToBuffer(buf, nbyte) \
	if (g_output_size + (nbyte) >= BUFF_SIZE) { \
		RLE_FlushBuffer(fd_out); \
		write(fd_out, (buf), (nbyte)); \
		g_output_size = 0; \
	} else { \
		memmove(g_output + g_output_size, (buf), (nbyte)); \
		g_output_size += (nbyte); \
	}


/* Globals */
uint8_t		*g_output = NULL;
size_t		g_output_size = 0;
int			fd_in;
int			fd_out;


typedef struct rotation_s
{
	size_t		size;
	size_t		index;
	uint8_t		*suffix;
}	rotation_t;

int			cmpfunc(const void *x, const void *y)
{
	rotation_t	*rx = (rotation_t*)x;
	rotation_t	*ry = (rotation_t*)y;
	return memcmp(rx->suffix, ry->suffix, MIN(rx->size, ry->size));
}

static ssize_t	*computeSuffixArray(uint8_t *input_text, size_t size)
{
	rotation_t	*suff = malloc(size * sizeof(rotation_t));
	ssize_t		*suffix_arr = (ssize_t *)malloc(size * sizeof(ssize_t));
	size_t		i;

	if (!suff || !suffix_arr)
	{
		free(suff);
		free(suffix_arr);
		error("malloc");
	}

	for (i = 0; i < size; ++i)
	{
		suff[i].size = size - i;
		suff[i].index = i;
		suff[i].suffix = input_text + i;
	}
	qsort(suff, size, sizeof(rotation_t), cmpfunc);

	for (i = 0; i < size; ++i)
		suffix_arr[i] = suff[i].index;

	free(suff);
	return suffix_arr;
}

static uint8_t	*findLastChar(uint8_t *input_text, ssize_t *suffix_arr, size_t n)
{
	uint8_t	*bwt_arr = (uint8_t *)malloc(n);
	ssize_t j;

	for (size_t i = 0; i < n; ++i)
	{
		j = suffix_arr[i] - 1;
		if (j < 0)
			j = j + n;
		bwt_arr[i] = input_text[j];
	}
	return bwt_arr;
}

void	RLE_BurrowsWheelerTransformEncode(uint8_t *data, size_t size)
{
	ssize_t		*suffix_arr = computeSuffixArray(data, size);
    uint8_t		*bwt_arr = findLastChar(data, suffix_arr, size);

	free(suffix_arr);
	memmove(data, bwt_arr, size);
	free(bwt_arr);
}



void	RLE_RunLengthEncode(int in)
{
	uint8_t		buf[BUFF_SIZE];
	uint8_t		*ptr;
	uint8_t		count;
	ssize_t		n;

	while ((n = read(in, buf, BUFF_SIZE)) > 0)
	{
		// RLE_BurrowsWheelerTransformEncode(buf, n);
		ptr = buf;
		count = 1;
		for ( ; n > 0; --n)
		{
			if (count < UCHAR_MAX && (n - 1) > 0 && *ptr == *(ptr + 1))
			{
				++count;
			}
			else
			{
				RLE_WriteToBuffer(&count, sizeof(count)); // number of repetitions
				RLE_WriteToBuffer(ptr, sizeof(*ptr));     // value
				count = 1;
			}
			++ptr;
		}
	}
	if (n < 0)
		error("read");
}

void	RLE_RunLengthEscapeEncode(int in)
{
	uint8_t		buf[BUFF_SIZE];
	uint8_t		*ptr;
	uint8_t		count;
	ssize_t		n;

	while ((n = read(in, buf, BUFF_SIZE)) > 0)
	{
		// RLE_BurrowsWheelerTransformEncode(buf, n);
		ptr = buf;
		count = 1;
		for ( ; n > 0; --n)
		{
			if (count < UCHAR_MAX && (n - 1) > 0 && *ptr == *(ptr + 1))
			{
				++count;
			}
			else
			{
				if (1 == count)
				{
					RLE_WriteToBuffer(ptr, sizeof(*ptr));
				}
				else
				{
					// not working with the buffer is filled with a single char
					if ((n - 1) <= 0)
					{
						// printf("Here\n");
						lseek(in, -count, SEEK_CUR);
						break ;
					}
					RLE_WriteToBuffer(ptr, sizeof(*ptr));     // value added 2 times
					RLE_WriteToBuffer(ptr, sizeof(*ptr));
					RLE_WriteToBuffer(&count, sizeof(count)); // number of repetitions
				}
				count = 1;
			}
			++ptr;
		}
	}
}

void	RLE_RunLengthDecode(int in)
{
	uint8_t		buf[BUFF_SIZE];
	uint8_t		c_repeat[UCHAR_MAX];
	uint8_t		*ptr;
	ssize_t		n;

	while ((n = read(in, buf, BUFF_SIZE)) > 0)
	{
		ptr = buf;
		for ( ; n > 0; n -= 2, ptr += 2)
		{
			memset(c_repeat, *(ptr + 1), (size_t)(uint8_t)*ptr);
			RLE_WriteToBuffer(c_repeat, (uint8_t)*ptr);
		}
	}
	if (n < 0)
		error("read");
}

void	RLE_RunLengthEscapeDecode(int in)
{
	size_t		bufsize = BUFF_SIZE;
	uint8_t		*buf = malloc(bufsize + 1);
	uint8_t		c_repeat[UCHAR_MAX];
	uint8_t		*ptr;
	ssize_t		n;
	ssize_t		n_cpy;

	if (!buf)
		error("malloc");

	while ((n = read(in, buf, bufsize)) > 0)
	{
		if (n == 1)
		{
			RLE_WriteToBuffer(buf, sizeof(uint8_t));
			break ;
		}
		ptr = buf;
		n_cpy = n;
		while (n > 0)
		{
			if (ptr[0] == ptr[1])
			{
				if ((n - 2) < 0)
				{
					size_t	__size = (size_t)buf - (size_t)ptr;
					if ((n - 1) < 0)
					{
						// error("RLE_RunLengthEscapeDecode: n < 0");
						assert( reallocf(buf, ++bufsize) != NULL );
						assert( read(in, buf + n_cpy, 1) >= 0 );
					}
					else
					{
						// error("RLE_RunLengthEscapeDecode: n < 0");
						assert( reallocf(buf, (bufsize += 2)) != NULL );
						assert( read(in, buf + n_cpy, 2) >= 0 );
					}
					ptr = buf + __size;
				}

				uint8_t	count = (uint8_t)ptr[2];
				
				memset(c_repeat, ptr[0], (size_t)count);
				RLE_WriteToBuffer(c_repeat, count);
				ptr += 3;
				n -= 3;
			}
			else
			{
				RLE_WriteToBuffer(ptr, sizeof(*ptr));
				++ptr;
				--n;
			}
		}
	}
	if (n < 0)
		error("read");
	free(buf);
}

// void	RLE_RunLengthEscapeDecode(int in, uint8_t last_val, int running)
// {
// 	uint8_t		buf[2];
// 	uint8_t		c_repeat[UCHAR_MAX];
// 	uint8_t		count;
// 	ssize_t		n = 0;

// 	if ((n = read(in, buf, sizeof(buf))) > 0)
// 	{
// 		if (running > 0 && n == 2)
// 		{
// 			if (buf[0] == last_val)
// 			{
// 				count = (uint8_t)buf[1];
// 				memset(c_repeat, buf[0], count);
// 				RLE_WriteToBuffer(c_repeat, count);
// 				RLE_RunLengthEscapeDecode(in, 0, 0);
// 				return ;
// 			}
// 		}

// 		if (n == 1)
// 		{
// 			RLE_WriteToBuffer(buf, sizeof(uint8_t));
// 		}
// 		else // n == 2
// 		{
// 			if (buf[0] == buf[1])
// 			{
// 				assert( read(in, &count, sizeof(count)) == 1 );
// 				memset(c_repeat, buf[0], count);
// 				RLE_WriteToBuffer(c_repeat, count);
// 				RLE_RunLengthEscapeDecode(in, 0, 0);
// 			}
// 			else
// 			{
// 				RLE_WriteToBuffer(buf, sizeof(uint8_t));
// 				RLE_RunLengthEscapeDecode(in, buf[1], 1);
// 			}
// 		}
// 	}
// }




// void	RLE_RunLengthEscapeDecode(int in)
// {
// 	size_t		size = BUFF_SIZE;
// 	uint8_t		*buf = malloc(size);
// 	size_t		buf_idx = 0;
// 	uint8_t		count;
// 	uint8_t		c_repeat[UCHAR_MAX];
// 	ssize_t		n;

// 	if (!buf)
// 		error("malloc");

// 	while ((n = read(in, buf + buf_idx, 2)) > 0)
// 	{
// 		if (n == 1)
// 		{
// 			RLE_WriteToBuffer(buf, sizeof(*buf));
// 		}
// 		else // n == 2
// 		{
// 			if (*buf == *(buf + 1))
// 			{
// 				assert( read(in, &count, sizeof(count)) == 1 );
// 				memset(c_repeat, *(buf + buf_idx), (size_t)count);
// 				RLE_WriteToBuffer(c_repeat, count);
// 				buf_idx = 0;
// 			}
// 			else
// 			{
// 				RLE_WriteToBuffer(buf + buf_idx, sizeof(*buf));
// 				if (buf_idx + 1 > size)
// 				{
// 					assert( reallocf(buf, (size += BUFF_SIZE)) != NULL );
// 				}
// 				memcpy(buf + buf_idx, buf + buf_idx + 1, sizeof(*buf));
// 				++buf_idx;
// 				// RLE_WriteToBuffer(buf + 1, sizeof(*buf));
// 			}
// 		}
// 	}
// 	if (n < 0)
// 		error("read");
// 	free(buf);
// }

// void	RLE_RunLengthEscapeDecode(int in)
// {
// 	uint8_t		buf1;
// 	uint8_t		buf2;
// 	uint8_t		count;
// 	uint8_t		c_repeat[UCHAR_MAX];
// 	ssize_t		n;

// 	while (1)
// 	{
// 		if (read(in, &buf1, sizeof(buf1)) < 0) error("read");
// 		if (read(in, &buf2, sizeof(buf2)) < 0) error("read");

// 		if (buf1 != buf2)
// 		{
// 			RLE_WriteToBuffer(&buf1, sizeof(buf2));
// 		}


// 		// if (buf1 == buf2)
// 		// {
// 		// 	assert( read(in, &count, sizeof(count)) == 1 );
// 		// 	// assert( count == (size_t)count );
// 		// 	memset(c_repeat, buf1, (size_t)count);
// 		// 	RLE_WriteToBuffer(c_repeat, count);
// 		// }
// 		// else
// 		// {	
// 		// 	RLE_WriteToBuffer(&buf1, sizeof(buf1));
// 		// 	RLE_WriteToBuffer(&buf2, sizeof(buf2));
// 		// }
// 	}
// }



void	RLE_EncodeGeneric(int in)
{
	// RLE_RunLengthEncode(in);
	RLE_RunLengthEscapeEncode(in);
}

void	RLE_DecodeGeneric(int in)
{
	// RLE_RunLengthDecode(in);
	RLE_RunLengthEscapeDecode(in);
	// RLE_RunLengthEscapeDecode(in, 0, 0);
}


int		main(int ac, char **av)
{
	char			*in_file, *out_file;
	// int				fd_in, fd_out;
	int				mode;
	struct stat		stat;


	if (ac != 3)
	{
		USAGE(av);
		return (EXIT_FAILURE);
	}


	if (!strcmp(av[1], "-d"))
		mode = MODE_DECOMP;
	else
		mode = MODE_COMP;

	
	in_file = (mode == MODE_DECOMP) ? av[2] : av[1];
	fd_in = open(in_file, O_RDONLY);
	assert( fd_in != SYSCALL_ERR );
	assert( fstat(fd_in, &stat) == 0 );
	
	
	if (mode == MODE_DECOMP)
	{
		{
			char *_out_name = strrchr(av[2], '.');
			if (_out_name)
				*_out_name = '\0';
		}
	}
	out_file = av[2];
	fd_out = open(out_file, O_TRUNC | O_CREAT | O_WRONLY, 0644);
	assert( fd_out != SYSCALL_ERR );
	


	// printf("* Allocating %u bytes for input buffer\n", BUFF_SIZE);
	assert ( (g_output = malloc(BUFF_SIZE)) != NULL );

	if (mode == MODE_COMP)
	{
		RLE_EncodeGeneric(fd_in);
	}
	else
	{
		RLE_DecodeGeneric(fd_in);
	}

	RLE_FlushBuffer(fd_out);

	close(fd_in);
	close(fd_out);
	
	free(g_output);
	g_output = NULL;
	
	return (EXIT_SUCCESS);
}
