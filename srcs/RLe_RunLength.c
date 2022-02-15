/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RLe_RunLength.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/04 15:12:44 by besellem          #+#    #+#             */
/*   Updated: 2022/02/15 21:15:45 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RLe.h"
#include "RLe_BurrowsWheelerTransform.h"

void	RLE_RunLengthEncode(RLE_params_t *params)
{
	const size_t	_size = BUFF_SIZE;
	buf_type		*buf = RLE_alloc(_size);
	buf_type		*ptr;
	count_type		count;
	size_t			n;

	while ((n = RLE_fread(buf, _size, params->input_stream)) > 0)
	{
		// RLE_BurrowsWheelerTransformEncode(buf, n);
		ptr = buf;
		count = 1;
		for ( ; n > 0; --n)
		{
			if (count < COUNT_MAX && (n - 1) > 0 && *ptr == *(ptr + 1))
			{
				++count;
			}
			else
			{
				RLE_WriteToBuffer(&count, sizeof(count_type)); // number of repetitions
				RLE_WriteToBuffer(ptr, sizeof(buf_type));     // value
				count = 1;
			}
			++ptr;
		}
	}
	free(buf);
}

void	RLE_RunLengthDecode(RLE_params_t *params)
{
	const size_t	_threshold = sizeof(buf_type) + sizeof(count_type);
	const size_t	_size = BUFF_SIZE;
	buf_type		*buf = RLE_alloc(_size);
	buf_type		repeated_buf[COUNT_MAX];
	buf_type		*ptr;
	size_t			n;

	while ((n = RLE_fread(buf, _size, params->input_stream)) > 0)
	{
		ptr = buf;
		for ( ; n > 0; n -= _threshold, ptr += _threshold)
		{
			count_type	counter = (buf_type)*ptr; // implicit cast
			int			c = *((char *)ptr + sizeof(count_type)); // implicit cast
			
			memset(repeated_buf, c, counter);
			RLE_WriteToBuffer(repeated_buf, counter);
		}
	}
	free(buf);
}
