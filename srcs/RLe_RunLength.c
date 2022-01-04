/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RLE_RunLength.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/04 15:12:44 by besellem          #+#    #+#             */
/*   Updated: 2022/01/04 15:21:27 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RLe.h"
#include "RLe_BurrowsWheelerTransform.h"

/* Globals importation */
extern uint8_t		*g_output;
extern size_t		g_output_size;
extern int			fd_in;
extern int			fd_out;


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
