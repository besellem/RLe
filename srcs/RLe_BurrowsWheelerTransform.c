/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RLE_BurrowsWheelerTransform.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/04 15:16:16 by besellem          #+#    #+#             */
/*   Updated: 2022/01/04 15:21:55 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RLe_BurrowsWheelerTransform.h"

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
