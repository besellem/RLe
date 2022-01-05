/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RLe_buffer.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/04 14:57:32 by besellem          #+#    #+#             */
/*   Updated: 2022/01/05 01:25:57 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RLE_BUFFER_H
# define RLE_BUFFER_H


extern int	fd_out;


/* flush buffer */
#define RLE_FlushBuffer(fd) \
	do { \
		write((fd), g_output, g_output_size); \
	} while (0);


/* bufferize or write directly */
#define RLE_WriteToBuffer(buf, nbyte) \
	do { \
		if (g_output_size + (nbyte) >= BUFF_SIZE) { \
			RLE_FlushBuffer(fd_out); \
			write(fd_out, (buf), (nbyte)); \
			g_output_size = 0; \
		} else { \
			memmove(g_output + g_output_size, (buf), (nbyte)); \
			g_output_size += (nbyte); \
		} \
	} while (0);


#endif
