/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RLe_buffer.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/04 14:57:32 by besellem          #+#    #+#             */
/*   Updated: 2022/02/13 23:36:15 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RLE_BUFFER_H
# define RLE_BUFFER_H

#include "RLe_memory.h"
#include <sys/types.h>


extern uint8_t	*g_output;
extern size_t	g_output_size;


typedef	struct	RLE_buffer_s {
	uint8_t		*buffer;
	size_t		size;
	size_t		pos;
}				RLE_buffer_t;


/* flush buffer */
#define RLE_FlushBuffer() \
	do { \
		fwrite(g_output, 1, g_output_size, singleton()->output_stream); \
	} while (0);


/* bufferize or write directly */
#define RLE_WriteToBuffer(buf, nbyte) \
	do { \
		if (g_output_size + (nbyte) >= BUFF_SIZE) { \
			RLE_FlushBuffer(); \
			fwrite((buf), 1, (nbyte), singleton()->output_stream); \
			g_output_size = 0; \
		} else { \
			memmove(g_output + g_output_size, (buf), (nbyte)); \
			g_output_size += (nbyte); \
		} \
	} while (0);


#endif
