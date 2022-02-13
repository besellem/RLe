/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RLe_RunLengthEscape.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/04 15:14:42 by besellem          #+#    #+#             */
/*   Updated: 2022/02/13 23:04:24 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RLe.h"
#include "RLe_BurrowsWheelerTransform.h"

/* Globals importation */
extern uint8_t		*g_output;
extern size_t		g_output_size;


typedef uint64_t	 count_type;
#define COUNT_MAX	 UINT64_MAX
#define REPEAT_BUFF  4096


void	RLE_RunLengthEscapeEncode(RLE_params_t *params)
{
	uint8_t		buf;
	uint8_t		buf_prev = 0;
	count_type	count = 1;
	ssize_t		n;

	assert( fread(&buf_prev, sizeof(buf_prev), sizeof(uint8_t), params->input_stream) );
	while ((n = fread(&buf, sizeof(buf), sizeof(uint8_t), params->input_stream)) > 0)
	{
		if (buf == buf_prev)
		{
			++count;
			assert( count < COUNT_MAX );
		}
		else
		{
			if (count == 1)
			{
				RLE_WriteToBuffer(&buf_prev, sizeof(buf));
			}
			else
			{
				RLE_WriteToBuffer(&buf_prev, sizeof(buf)); // value added 2 times
				RLE_WriteToBuffer(&buf_prev, sizeof(buf));
				RLE_WriteToBuffer(&count, sizeof(count));  // number of repetitions
				count = 1;
			}
		}
		buf_prev = buf;
	}
	if (n < 0)
		syscall_error("fread");
}


// void	RLE_RunLengthEscapeEncode(int in)
// {
// 	uint8_t		buf[BUFF_SIZE];
// 	uint8_t		*ptr;
// 	uint8_t		count;
// 	ssize_t		n;

// 	while ((n = read(in, buf, BUFF_SIZE)) > 0)
// 	{
// 		// RLE_BurrowsWheelerTransformEncode(buf, n);
// 		ptr = buf;
// 		count = 1;
// 		for ( ; n > 0; --n)
// 		{
// 			if (count < UCHAR_MAX && (n - 1) > 0 && *ptr == *(ptr + 1))
// 			{
// 				++count;
// 			}
// 			else
// 			{
// 				if (1 == count)
// 				{
// 					RLE_WriteToBuffer(ptr, sizeof(*ptr));
// 				}
// 				else
// 				{
// 					// not working if the buffer is filled with a single char
// 					if ((n - 1) <= 0)
// 					{
// 						// printf("Here\n");
// 						assert ( lseek(in, -count, SEEK_CUR) != SYSCALL_ERR );
// 						break ;
// 					}
// 					RLE_WriteToBuffer(ptr, sizeof(*ptr));     // value added 2 times
// 					RLE_WriteToBuffer(ptr, sizeof(*ptr));
// 					RLE_WriteToBuffer(&count, sizeof(count)); // number of repetitions
// 				}
// 				count = 1;
// 			}
// 			++ptr;
// 		}
// 	}
// 	if (n < 0)
// 		syscall_error("read");
// }

void	RLE_RunLengthEscapeDecode(RLE_params_t *params)
{
	size_t		bufsize = BUFF_SIZE;
	uint8_t		*buf = malloc(bufsize);
	uint8_t		*c_repeat = malloc(REPEAT_BUFF);
	uint8_t		*ptr;
	ssize_t		n;
	ssize_t		n_cpy;
	count_type	count;


	if (!buf || !c_repeat)
	{
		free(buf);
		free(c_repeat);
		syscall_error("malloc");
	}

	while ((n = fread(buf, sizeof(*buf), bufsize, params->input_stream)) > 0)
	{
		if (n == 1)
		{
			RLE_WriteToBuffer(buf, sizeof(uint8_t));
			continue ; // read will return EOF next time
		}
		ptr = buf;
		n_cpy = n;
		while (n > 0)
		{
			if (n > 1 && ptr[0] == ptr[1])
			{
				if ((n - 2 - (ssize_t)sizeof(count)) < 0)
				{
					const size_t	__nbyte = (2 + sizeof(count)) - n;
					size_t			__diff = (size_t)ptr - (size_t)buf;

					// printf("__diff: [%zu], __nbyte: [%zu]\n", __diff, __nbyte);
					
					assert( (buf = reallocf(buf, (bufsize += __nbyte))) != NULL );
					assert( fread(buf + n_cpy, sizeof(*buf), __nbyte, params->input_stream) >= 0 );
					ptr = buf + __diff;
					n += __nbyte; // not necessary
				}

				memmove(&count, ptr + 2, sizeof(count));
				
				memset(c_repeat, ptr[0], REPEAT_BUFF);
				
				count_type	__count = count;
				while (__count >= REPEAT_BUFF)
				{
					RLE_WriteToBuffer(c_repeat, REPEAT_BUFF);
					__count -= REPEAT_BUFF;
				}
				if (__count > 0)
					RLE_WriteToBuffer(c_repeat, __count);
				
				ptr += (2 + sizeof(count));
				n -= (2 + sizeof(count));
			}
			else
			{
				RLE_WriteToBuffer(ptr, sizeof(*ptr));
				++ptr;
				--n;
				if (n == 1)
					assert( fseek(params->input_stream, -1, SEEK_CUR) != SYSCALL_ERR );
			}
		}
	}
	if (n < 0)
		syscall_error("fread");
	free(buf);
	free(c_repeat);
}

/* recursive */
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
// 		syscall_error("malloc");

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
// 		syscall_error("read");
// 	free(buf);
// }
