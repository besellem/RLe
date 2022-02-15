/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RLe_memory.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/13 21:35:46 by besellem          #+#    #+#             */
/*   Updated: 2022/02/15 23:29:44 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RLe_memory.h"

void			*_RLE_alloc(
	size_t size,
	__unused const char *file,
	__unused const char *func,
	__unused size_t line
)
{
	void	*ptr = malloc(size);

#if DEBUG >= DEBUG_LVL_2
	fprintf(stderr, "%s:%s:%zu: Allocating %zu bytes\n", file, func, line, size);
#endif
	
	if (!ptr)
	{
		fprintf(stderr, ERROR_MSG ": Could not allocate %zu bytes\n", size);
		RLE_free_all();
		exit(EXIT_FAILURE);
	}
	return ptr;
}

void			*_RLE_realloc(
	void *ptr,
	size_t size,
	__unused const char *file,
	__unused const char *func,
	__unused size_t line
)
{
	void	*new_ptr = realloc(ptr, size);

#if DEBUG >= DEBUG_LVL_2
	fprintf(stderr, "%s:%s:%zu: Reallocating %zu bytes\n", file, func, line, size);
#endif

	if (!new_ptr)
	{
		fprintf(stderr, ERROR_MSG ": Could not reallocate %zu bytes\n", size);
		free(ptr);
		RLE_free_all();
		exit(EXIT_FAILURE);
	}
	return new_ptr;
}

RLE_process_tracking_t	RLE_get_process_tracking(void)
{
	const RLE_params_t		*params = RLE_get_params();
	RLE_process_tracking_t	track = { 0 };

	track.bytes_read = ftell(params->input_stream);
	track.bytes_written = ftell(params->output_stream) + g_output_idx;
	track.input_perc = (double)track.bytes_read / params->in_st.st_size * 100.;
	track.algo_efficiency = (double)track.bytes_written / track.bytes_read * 100.;
	return track;
}

size_t			_RLE_fread(buf_type *buf, size_t nbyte, FILE *stream, const char *file, size_t line)
{
	const RLE_params_t				*params = RLE_get_params();
	const size_t					n = fread(buf, sizeof(*buf), nbyte, stream);
	const RLE_process_tracking_t	track = RLE_get_process_tracking();

	
	if (is_option_set(params->parsing_opts.opts, OPT_V_MIN))
	{
		fprintf(stderr, CLR_LINE "Processing: %2d%% [%.1f MB written]...",
				track.input_perc,
				track.bytes_written / 1000000.0);
	}

	if (ferror(stream))
	{
		_syscall_error("fread", file, line);
	}
	return (n);
}

/* singleton */
RLE_params_t	*RLE_get_params(void)
{
	static RLE_params_t	*p = NULL;

	if (!p)
	{
		p = calloc(1, sizeof(RLE_params_t));
		if (!p)
		{
			fprintf(stderr, ERROR_MSG ": Could not allocate %zu bytes\n", sizeof(RLE_params_t));
			exit(EXIT_FAILURE);
		}
	}
	return p;
}

static void		print_summary(void)
{
	const RLE_params_t				*params = RLE_get_params();
	const RLE_process_tracking_t	track = RLE_get_process_tracking();

	fprintf(stderr, CLR_LINE "Summary: \n");
	fprintf(stderr, "  %3d%% done\n", track.input_perc);
	
	if (params->mode == RLE_ENCODE)      fprintf(stderr, "  Encoding mode\n");
	else if (params->mode == RLE_DECODE) fprintf(stderr, "  Decoding mode\n");
	
	if (params->input_file)  fprintf(stderr, "  %.1f MB written\n", track.bytes_written / 1000000.0);
	if (params->output_file) fprintf(stderr, "  %.1f MB read\n", track.bytes_read / 1000000.0);
	
	if (params->mode == RLE_ENCODE && params->output_file && params->input_file)
	{
		fprintf(stderr, "  %s%d" CLR_COLOR "%% ratio [%s]\n",
				track.algo_efficiency > 100 ? RED : GREEN,
				track.algo_efficiency,
				params->algo);
	}
}

void			RLE_free_all(void)
{
	RLE_params_t	*params = RLE_get_params();

	if (is_option_set(params->parsing_opts.opts, OPT_V_MIN))
		print_summary();

	if (params && g_output)
		RLE_FlushBuffer();
	
	fclose(params->input_stream);
	fclose(params->output_stream);
	free(params);
	free(g_output);
}
