/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RLe_buffer.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/04 14:57:32 by besellem          #+#    #+#             */
/*   Updated: 2022/02/15 21:37:56 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RLE_BUFFER_H
# define RLE_BUFFER_H

#include "RLe_types.h"
#include "RLe_memory.h"
#include "RLe_error.h"


extern buf_type	*g_output;
extern size_t	g_output_idx;
extern size_t	g_output_size;


#define RLE_fwrite(__buf, __size) \
	fwrite((__buf), 1, (__size), RLE_get_params()->output_stream)


/* flush buffer */
#define RLE_FlushBuffer() \
	RLE_fwrite(g_output, g_output_idx)


/* bufferize or write directly */
#define RLE_WriteToBuffer(__buf, __nbyte) \
	do { \
		if (g_output_idx + (__nbyte) >= g_output_size) { \
			RLE_FlushBuffer(); \
			RLE_fwrite(__buf, __nbyte); \
			g_output_idx = 0; \
		} else { \
			memmove(g_output + g_output_idx, (__buf), (__nbyte)); \
			g_output_idx += (__nbyte); \
		} \
	} while (0);


#endif
