/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RLE_RunLengthEscape.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/04 15:14:42 by besellem          #+#    #+#             */
/*   Updated: 2022/01/04 15:21:21 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RLe.h"
#include "RLe_BurrowsWheelerTransform.h"

/* Globals importation */
extern uint8_t		*g_output;
extern size_t		g_output_size;
extern int			fd_in;
extern int			fd_out;


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
					// if ((n - 1) <= 0)
					// {
					// 	printf("Here\n");
					// 	lseek(in, -count, SEEK_CUR);
					// 	break ;
					// }
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
