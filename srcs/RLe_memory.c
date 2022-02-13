/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RLe_memory.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/13 21:35:46 by besellem          #+#    #+#             */
/*   Updated: 2022/02/13 23:34:51 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RLe_memory.h"

RLE_params_t	*singleton(void)
{
	static RLE_params_t	*p = NULL;

	if (!p)
	{
		p = calloc(1, sizeof(RLE_params_t));
		if (!p)
			return NULL;
	}
	return p;
}

void	free_all(void)
{
	RLE_params_t	*params = singleton();

	fclose(params->input_stream);
	fclose(params->output_stream);
	free(params);
	free(g_output);
}
