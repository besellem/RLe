/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RLe_RunLengthEscape.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/04 15:14:42 by besellem          #+#    #+#             */
/*   Updated: 2022/02/15 21:19:48 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RLe.h"
#include "RLe_BurrowsWheelerTransform.h"

#define REPEAT_BUFF  4096

void	RLE_RunLengthEscapeEncode(RLE_params_t *params)
{
	buf_type	buf;
	buf_type	buf_prev = 0;
	count_type	count = 1;
	size_t		n;

	RLE_fread(&buf_prev, sizeof(buf_type), params->input_stream);
	while ((n = RLE_fread(&buf, sizeof(buf_type), params->input_stream)))
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
				RLE_WriteToBuffer(&buf_prev, sizeof(buf_type));
			}
			else
			{
				RLE_WriteToBuffer(&buf_prev, sizeof(buf_type)); // value added 2 times
				RLE_WriteToBuffer(&buf_prev, sizeof(buf_type));
				RLE_WriteToBuffer(&count, sizeof(count));  // number of repetitions
				count = 1;
			}
		}
		buf_prev = buf;
	}
}


// void	RLE_RunLengthEscapeEncode(int in)
// {
// 	buf_type	buf[BUFF_SIZE];
// 	buf_type	*ptr;
// 	buf_type	count;
// 	size_t		n;

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
// }

void	RLE_RunLengthEscapeDecode(RLE_params_t *params)
{
	size_t		bufsize = BUFF_SIZE;
	buf_type	*buf = RLE_alloc(bufsize);
	buf_type	*c_repeat = RLE_alloc(COUNT_MAX);
	buf_type	*ptr;
	ssize_t		n;
	ssize_t		n_cpy;
	count_type	count;

	while ((n = RLE_fread(buf, bufsize, params->input_stream)))
	{
		if (n == 1)
		{
			RLE_WriteToBuffer(buf, sizeof(buf_type));
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
					
					buf = RLE_realloc(buf, (bufsize += __nbyte));
					RLE_fread(buf + n_cpy, __nbyte, params->input_stream);
					ptr = buf + __diff;
					n += __nbyte; // not necessary
				}

				memmove(&count, ptr + 2, sizeof(count));
				
				memset(c_repeat, ptr[0], COUNT_MAX);
				// memset(c_repeat, ptr[0], MIN(COUNT_MAX, count));
				
				count_type	__count = count;
				while (__count >= COUNT_MAX)
				{
					RLE_WriteToBuffer(c_repeat, COUNT_MAX);
					__count -= COUNT_MAX;
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
	free(buf);
	free(c_repeat);
}

/* recursive */
// void	RLE_RunLengthEscapeDecode(int in, buf_type last_val, int running)
// {
// 	buf_type	buf[2];
// 	buf_type	c_repeat[UCHAR_MAX];
// 	buf_type	count;
// 	ssize_t		n = 0;

// 	if ((n = read(in, buf, sizeof(buf))) > 0)
// 	{
// 		if (running > 0 && n == 2)
// 		{
// 			if (buf[0] == last_val)
// 			{
// 				count = (buf_type)buf[1];
// 				memset(c_repeat, buf[0], count);
// 				RLE_WriteToBuffer(c_repeat, count);
// 				RLE_RunLengthEscapeDecode(in, 0, 0);
// 				return ;
// 			}
// 		}

// 		if (n == 1)
// 		{
// 			RLE_WriteToBuffer(buf, sizeof(buf_type));
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
// 				RLE_WriteToBuffer(buf, sizeof(buf_type));
// 				RLE_RunLengthEscapeDecode(in, buf[1], 1);
// 			}
// 		}
// 	}
// }


// void	RLE_RunLengthEscapeDecode(int in)
// {
// 	size_t		size = BUFF_SIZE;
// 	buf_type	*buf = RLE_alloc(size);
// 	size_t		buf_idx = 0;
// 	buf_type	count;
// 	buf_type	c_repeat[UCHAR_MAX];
// 	ssize_t		n;

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
// 					RLE_realloc(buf, (size += BUFF_SIZE));
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
