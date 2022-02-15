/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RLe_types.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/15 16:27:56 by besellem          #+#    #+#             */
/*   Updated: 2022/02/15 21:34:35 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RLE_TYPES_H
# define RLE_TYPES_H

#include <sys/types.h>
#include <stdint.h>
#include <stdbool.h>
#include <limits.h>

/* buffer type */
typedef uint8_t		buf_type;


/* avoid defining COUNT_MAX > UINT16_MAX -> may be too large */
#define COUNT_MAX	 UINT8_MAX
#define SIZEOF_COUNT sizeof(count_type)


/* counter type - used in run-length algorithms */
#if __SIZEOF_LONG_LONG__ == 8 && COUNT_MAX == UINT64_MAX
typedef uint64_t		count_type;
#elif COUNT_MAX == UINT32_MAX
typedef uint32_t		count_type;
#elif COUNT_MAX == UINT16_MAX
typedef uint16_t		count_type;
#else
typedef uint8_t			count_type;
#endif


#endif /* !defined(RLE_TYPES_H) */
