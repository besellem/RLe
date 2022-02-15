/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RLe_BurrowsWheelerTransform.h                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/04 15:16:27 by besellem          #+#    #+#             */
/*   Updated: 2022/02/15 16:21:24 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RLE_BURROWS_WHEELER_TRANSFORM_H
# define RLE_BURROWS_WHEELER_TRANSFORM_H

#include "RLe.h"

typedef struct rotation_s
{
	size_t		size;
	size_t		index;
	buf_type	*suffix;
}	rotation_t;

#endif
