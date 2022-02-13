/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RLe_main.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/04 14:53:48 by besellem          #+#    #+#             */
/*   Updated: 2022/02/13 23:35:21 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RLe.h"

/* Globals */
uint8_t		*g_output = NULL;
size_t		g_output_size = 0;

static bool	open_streams(const t_options_parsing *options, RLE_params_t *params)
{
	// check if input and output streams are the same
	if (params->input_file && params->output_file && !strcmp(params->input_file, params->output_file))
	{
		fprintf(stderr, "Error: input and output files cannot be the same\n");
		return false;
	}
	
	// handle input file
	if (!is_option_set(options->opts, OPT_I_MIN))
		params->input_stream = stdin;
	else
		params->input_stream = fopen(params->input_file, "rb");
	if (!params->input_stream)
	{
		fprintf(stderr, "Error: Could not open input file %s\n", params->input_file);
		return false;
	}

	// handle output file
	if (!is_option_set(options->opts, OPT_O_MIN))
		params->output_stream = stdout;
	else
		params->output_stream = fopen(params->output_file, "wb");
	if (!params->output_stream)
	{
		fprintf(stderr, "Error: Could not create output file %s\n", params->output_file);
		return false;
	}
	
	return true;
}

static bool	set_mode(const t_options_parsing *options, RLE_params_t *params)
{
	static struct RLE_modes_s	algo[] = {
		{ RLE_ENCODE, RLE_ALGO_RUN_LENGTH, &RLE_RunLengthEncode },
		{ RLE_DECODE, RLE_ALGO_RUN_LENGTH, &RLE_RunLengthDecode },
		{ RLE_ENCODE, RLE_ALGO_RUN_LENGTH_ESCAPE, &RLE_RunLengthEscapeEncode },
		{ RLE_DECODE, RLE_ALGO_RUN_LENGTH_ESCAPE, &RLE_RunLengthEscapeDecode },
		{ RLE_DECODE, NULL, 0 }
	};


	params->mode = RLE_DEFAULT_MODE;
	if (is_option_set(options->opts, OPT_D_MIN))
		params->mode = RLE_DECODE;
	else if (is_option_set(options->opts, OPT_E_MIN))
		params->mode = RLE_ENCODE;


	// set default algorithm
	if (!is_option_set(options->opts, OPT_M_MIN))
	{
		params->func = algo[params->mode].func;
		return true;
	}

	// search for the algorithm
	for (size_t i = 0; algo[i].name; ++i)
	{
		if (params->mode == algo[i].mode && !strcmp(algo[i].name, params->algo))
		{
			params->func = algo[i].func;
			return true;
		}
	}
	return false;
}

void	print_params(const RLE_params_t *params)
{
	printf("{\n");
	printf("  mode:        %s\n", params->mode ? "encode" : "decode");
	printf("  algorithm:   %s\n", params->algo);
	printf("  input file:  %s\n", params->input_file ? params->input_file : "stdin");
	printf("  output file: %s\n", params->output_file ? params->output_file : "stdout");
	printf("}\n");
}

int		main(int ac, char **av)
{
	RLE_params_t		*params = singleton();
	t_options_parsing	options;


	if (!params)
		syscall_error("singleton allocation failed");

	// parse arguments
	options = parse_args(ac, av, true, "edm:i:o:", &params->algo, &params->input_file, &params->output_file);
	if (is_option_set(options.opts, OPT_ILLEGAL))
		main_error(av[0], "illegal option");

	// open streams
	if (!open_streams(&options, params))
	{
		free_all();
		return EXIT_FAILURE;
	}

#if DEBUG >= DEBUG_LEVEL_1
	fprintf(stderr, "* Allocating %u bytes for input buffer\n", BUFF_SIZE);
#endif
	if (!(g_output = malloc(BUFF_SIZE)))
		syscall_error("malloc failed");
	
	
	// set modes (encode / decode) and algorithm
	if (!set_mode(&options, params))
		main_error(av[0], "invalid algorithm");

	
#if DEBUG >= DEBUG_LEVEL_1
	print_params(params);
#endif

	
	// run the algorithm
	RLE_RunGeneric(params);
	
	
	RLE_FlushBuffer();
	
	free_all();
	return EXIT_SUCCESS;
}
