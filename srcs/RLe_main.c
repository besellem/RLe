/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RLe_main.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/04 14:53:48 by besellem          #+#    #+#             */
/*   Updated: 2022/02/15 23:18:43 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RLe.h"

/* Globals */
buf_type	*g_output = NULL;
size_t		g_output_idx = 0;
size_t		g_output_size = 0;

static bool	open_streams(const t_options_parsing *options, RLE_params_t *params)
{
	// check if input and output streams are the same
	if (params->input_file && params->output_file && !strcmp(params->input_file, params->output_file))
	{
		fprintf(stderr, ERROR_MSG ": input and output files cannot be the same\n");
		return false;
	}
	
	// handle input file
	if (!is_option_set(options->opts, OPT_I_MIN))
		params->input_stream = stdin;
	else
		params->input_stream = fopen(params->input_file, "rb");
	if (!params->input_stream)
	{
		fprintf(stderr, ERROR_MSG ": Could not open input file %s\n", params->input_file);
		return false;
	}

	// handle output file
	if (!is_option_set(options->opts, OPT_O_MIN))
		params->output_stream = stdout;
	else
		params->output_stream = fopen(params->output_file, "wb");
	if (!params->output_stream)
	{
		fprintf(stderr, ERROR_MSG ": Could not create output file %s\n", params->output_file);
		return false;
	}
	
	return true;
}

struct	RLE_modes_s
{
	uint		mode; // 0 = encode, 1 = decode
	char		*name;
	void		(*func)(RLE_params_t *);
};

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
	struct stat	st = { 0 };
	
	stat(params->input_file, &st);

	fprintf(stderr, "{\n");

	if (params->mode == RLE_ENCODE)      fprintf(stderr, "  mode:        encode\n");
	else if (params->mode == RLE_DECODE) fprintf(stderr, "  mode:        decode\n");
	
	fprintf(stderr, "  algorithm:   %s\n", params->algo);
	fprintf(stderr, "  input file:  %s [%.1f MB]\n",
			params->input_file ? params->input_file : "stdin",
			st.st_size / 1000000.0);
	fprintf(stderr, "  output file: %s\n", params->output_file ? params->output_file : "stdout");
	fprintf(stderr, "}\n");
}

void	sigint_handler(__unused int sig)
{
	fprintf(stderr, "\n");
	syscall_error("interrupted by user");
}

int		main(int ac, char **av)
{
	RLE_params_t		*params = RLE_get_params();
	t_options_parsing	opts;

	// parse arguments
	params->parsing_opts = parse_args(ac, av, true, "edvm:i:o:", &params->algo, &params->input_file, &params->output_file);
	opts = params->parsing_opts;
	if (is_option_set(opts.opts, OPT_ILLEGAL))
		main_error(av[0], "illegal option");

	// open streams
	if (!open_streams(&opts, params))
	{
		RLE_free_all();
		return EXIT_FAILURE;
	}
	
	// set modes (encode / decode) and algorithm
	if (!set_mode(&opts, params))
		main_error(av[0], "invalid algorithm");
	
	if (stat(params->input_file, &params->in_st) == SYSCALL_ERR) // stdin
		g_output_size = BUFF_SIZE;
	else
	{
		if (params->mode == RLE_ENCODE)
			g_output_size = params->in_st.st_size;
		else
			g_output_size = params->in_st.st_size * 3;
	}

	g_output = RLE_alloc(g_output_size);
	
#if DEBUG >= DEBUG_LVL_1
	print_params(params);
#endif

	signal(SIGINT, &sigint_handler);
	
	// run the algorithm
	RLE_RunGeneric(params);
	
	
	RLE_free_all();
	return EXIT_SUCCESS;
}
