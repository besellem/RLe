/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RLe_memory.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/13 21:36:52 by besellem          #+#    #+#             */
/*   Updated: 2022/02/13 23:36:57 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RLE_MEMORY_H
# define RLE_MEMORY_H

# include <stdlib.h>
# include <stdio.h>
# include <stdbool.h>
# include "RLe_buffer.h"


extern uint8_t	*g_output;
extern size_t	g_output_size;


typedef	struct	RLE_params_s
{
	bool		mode;  // true = encode, false = decode
	char		*algo; // name of the algorithm

	// input / output streams & files
	char		*input_file;
	char		*output_file;
	FILE		*input_stream;
	FILE		*output_stream;
	
	// algorithm function
	void		(*func)(struct RLE_params_s *);
	
}				RLE_params_t;


RLE_params_t	*singleton(void);
void			free_all(void);


#endif /* !defined(RLE_MEMORY_H) */
