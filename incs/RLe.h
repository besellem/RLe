/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RLe.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/04 14:46:12 by besellem          #+#    #+#             */
/*   Updated: 2022/02/15 23:26:39 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RLE_H
# define RLE_H

#include "RLe_types.h"
#include "RLe_parsing.h"
#include "RLe_memory.h"
#include "RLe_buffer.h"
#include "RLe_error.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <libgen.h>
#include <sys/stat.h>
#include <signal.h>
#include <errno.h>

#include <pthread.h>
#include <assert.h>

/*
** Defines
*/
#define SYSCALL_ERR  (-1)

/*
** Macro-like functions
*/
#define MAX(a, b)  ((a) > (b) ? (a) : (b))
#define MIN(a, b)  ((a) < (b) ? (a) : (b))


/*
** Data structures
*/

/*
** Functions
*/

void		RLE_RunGeneric(RLE_params_t *);

void		RLE_RunLengthEncode(RLE_params_t *);
void		RLE_RunLengthDecode(RLE_params_t *);
void		RLE_RunLengthEscapeEncode(RLE_params_t *);
void		RLE_RunLengthEscapeDecode(RLE_params_t *);

void		RLE_BurrowsWheelerTransformEncode(buf_type *data, size_t size);


#endif
