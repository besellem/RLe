/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RLe.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/02 19:15:30 by besellem          #+#    #+#             */
/*   Updated: 2022/01/04 23:30:27 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RLe.h"


/* Globals importation */
// extern uint8_t		*g_output;
// extern size_t		g_output_size;
// extern int			fd_in;
// extern int			fd_out;

void	RLE_EncodeGeneric(int in)
{
	// RLE_RunLengthEncode(in);
	RLE_RunLengthEscapeEncode(in);
}

void	RLE_DecodeGeneric(int in)
{
	// RLE_RunLengthDecode(in);
	RLE_RunLengthEscapeDecode(in);
	// RLE_RunLengthEscapeDecode(in, 0, 0);
}
