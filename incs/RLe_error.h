/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RLe_error.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/13 21:59:23 by besellem          #+#    #+#             */
/*   Updated: 2022/02/13 22:57:11 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RLE_ERROR_H
# define RLE_ERROR_H

#include "RLe_memory.h"
#include <stdio.h>
#include <stdlib.h>


/*
** DEBUG LEVELS
*/
#define DEBUG_LVL_0  0 // No debug
#define DEBUG_LVL_1  1 // Prints basic info
#define DEBUG_LVL_2  2 // Prints basic info + buffer info + memory info

#ifndef DEBUG
# define DEBUG       DEBUG_LVL_2
#endif


#define USAGE(prog_name) \
	fprintf(stdout, "Usage: %1$s -ed [-m mode] [-i input_file] [-o output_file]\n" \
					"%2$13c -e: Encode mode\n" \
					"%2$13c -d: Decode mode\n" \
					"%2$13c -i: Input file\n" \
					"%2$13c -o: Output file\n" \
					"%2$13c -m: Modes:\n" \
					"%2$16c - run-length\n" \
					"%2$16c - run-length-escape\n", \
					prog_name, 0);

#define main_error(prog_name, msg)  _main_error((msg), (prog_name), __FILE__, __LINE__);
#define syscall_error(msg)          _syscall_error((msg), __FILE__, __LINE__);


void	_main_error(const char *, const char *, const char *, size_t);
void	_syscall_error(const char *, const char *, size_t);


#endif /* !defined(RLE_ERROR_H) */
