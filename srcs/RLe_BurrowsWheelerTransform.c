/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RLe_BurrowsWheelerTransform.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/04 15:16:16 by besellem          #+#    #+#             */
/*   Updated: 2022/02/15 16:21:50 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RLe_BurrowsWheelerTransform.h"

int			cmpfunc(const void *x, const void *y)
{
	rotation_t	*rx = (rotation_t*)x;
	rotation_t	*ry = (rotation_t*)y;
	return memcmp(rx->suffix, ry->suffix, MIN(rx->size, ry->size));
}

static ssize_t	*computeSuffixArray(buf_type *input_text, size_t size)
{
	rotation_t	*suff = RLE_alloc(size * sizeof(rotation_t));
	ssize_t		*suffix_arr = (ssize_t *)RLE_alloc(size * sizeof(ssize_t));
	size_t		i;

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

static buf_type	*findLastChar(buf_type *input_text, ssize_t *suffix_arr, size_t n)
{
	buf_type	*bwt_arr = (buf_type *)RLE_alloc(n);
	ssize_t		j;

	for (size_t i = 0; i < n; ++i)
	{
		j = suffix_arr[i] - 1;
		if (j < 0)
			j = j + n;
		bwt_arr[i] = input_text[j];
	}
	return bwt_arr;
}

void	RLE_BurrowsWheelerTransformEncode(buf_type *data, size_t size)
{
	ssize_t		*suffix_arr = computeSuffixArray(data, size);
    buf_type	*bwt_arr = findLastChar(data, suffix_arr, size);

	free(suffix_arr);
	memmove(data, bwt_arr, size);
	free(bwt_arr);
}
