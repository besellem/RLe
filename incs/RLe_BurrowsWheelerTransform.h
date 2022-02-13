/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RLE_BurrowsWheelerTransform.h                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/04 15:16:27 by besellem          #+#    #+#             */
/*   Updated: 2022/01/04 15:23:09 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RLE_BURROWS_WHEELER_TRANSFORM_H
# define RLE_BURROWS_WHEELER_TRANSFORM_H

#include "RLe.h"

typedef struct rotation_s
{
	size_t		size;
	size_t		index;
	uint8_t		*suffix;
}	rotation_t;

#endif
