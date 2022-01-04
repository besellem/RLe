/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RLe_main.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/04 14:53:48 by besellem          #+#    #+#             */
/*   Updated: 2022/01/04 15:13:01 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RLe.h"

#define USAGE(av) \
	fprintf(stdout, "Usage: %s <input_file> <output_file>\n" \
					"       %s -d <input_file>\n", av[0], av[0]);


/* Globals */
uint8_t		*g_output = NULL;
size_t		g_output_size = 0;
int			fd_in;
int			fd_out;


int		main(int ac, char **av)
{
	char			*in_file, *out_file;
	int				mode = MODE_ENCODE; // default mode is encode
	struct stat		stat;


	if (ac != 3)
	{
		USAGE(av);
		return (EXIT_FAILURE);
	}


	if (!strcmp(av[1], "-d"))
		mode = MODE_DECODE;

	
	in_file = (mode == MODE_DECODE) ? av[2] : av[1];
	fd_in = open(in_file, O_RDONLY);
	assert( fd_in != SYSCALL_ERR );
	assert( fstat(fd_in, &stat) == 0 );
	
	
	if (mode == MODE_DECODE)
	{
		{
			char *_out_name = strrchr(av[2], '.');
			if (_out_name)
				*_out_name = '\0';
		}
	}
	out_file = av[2];
	fd_out = open(out_file, O_TRUNC | O_CREAT | O_WRONLY, 0644);
	assert( fd_out != SYSCALL_ERR );
	


	// printf("* Allocating %u bytes for input buffer\n", BUFF_SIZE);
	assert ( (g_output = malloc(BUFF_SIZE)) != NULL );

	if (mode == MODE_ENCODE)
	{
		RLE_EncodeGeneric(fd_in);
	}
	else
	{
		RLE_DecodeGeneric(fd_in);
	}


	RLE_FlushBuffer(fd_out);

	close(fd_in);
	close(fd_out);
	
	free(g_output);
	g_output = NULL;
	
	return (EXIT_SUCCESS);
}
