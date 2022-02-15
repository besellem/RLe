/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RLe_error.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/13 21:54:50 by besellem          #+#    #+#             */
/*   Updated: 2022/02/15 23:03:03 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RLe_error.h"

void	_main_error(const char *msg, const char *program_name, const char *file, size_t line)
{
	USAGE(program_name);
	_syscall_error(msg, file, line);
}

void	_syscall_error(const char *msg, __unused const char *file, __unused size_t line)
{
#if DEBUG >= DEBUG_LVL_1
	fprintf(stderr, "%s:%zu: " ERROR_MSG ": %s\n", file, line, msg);
#else
	fprintf(stderr, ERROR_MSG ": %s\n", msg);
#endif
	RLE_free_all();
	exit(EXIT_FAILURE);
}
