/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RLe_parsing.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/13 14:35:34 by besellem          #+#    #+#             */
/*   Updated: 2022/02/13 15:09:35 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RLE_PARSING_H
# define RLE_PARSING_H

# include <stdlib.h>
# include <string.h>
# include <stdarg.h>
# include <stdbool.h>
# include <stdint.h>
# include <stdio.h>

/* options masks */
# define OPT_A       (1ULL <<  0)
# define OPT_B       (1ULL <<  1)
# define OPT_C       (1ULL <<  2)
# define OPT_D       (1ULL <<  3)
# define OPT_E       (1ULL <<  4)
# define OPT_F       (1ULL <<  5)
# define OPT_G       (1ULL <<  6)
# define OPT_H       (1ULL <<  7)
# define OPT_I       (1ULL <<  8)
# define OPT_J       (1ULL <<  9)
# define OPT_K       (1ULL << 10)
# define OPT_L       (1ULL << 11)
# define OPT_M       (1ULL << 12)
# define OPT_N       (1ULL << 13)
# define OPT_O       (1ULL << 14)
# define OPT_P       (1ULL << 15)
# define OPT_Q       (1ULL << 16)
# define OPT_R       (1ULL << 17)
# define OPT_S       (1ULL << 18)
# define OPT_T       (1ULL << 19)
# define OPT_U       (1ULL << 20)
# define OPT_V       (1ULL << 21)
# define OPT_W       (1ULL << 22)
# define OPT_X       (1ULL << 23)
# define OPT_Y       (1ULL << 24)
# define OPT_Z       (1ULL << 25)
# define OPT_A_MIN   (1ULL << 26)
# define OPT_B_MIN   (1ULL << 27)
# define OPT_C_MIN   (1ULL << 28)
# define OPT_D_MIN   (1ULL << 29)
# define OPT_E_MIN   (1ULL << 30)
# define OPT_F_MIN   (1ULL << 31)
# define OPT_G_MIN   (1ULL << 32)
# define OPT_H_MIN   (1ULL << 33)
# define OPT_I_MIN   (1ULL << 34)
# define OPT_J_MIN   (1ULL << 35)
# define OPT_K_MIN   (1ULL << 36)
# define OPT_L_MIN   (1ULL << 37)
# define OPT_M_MIN   (1ULL << 38)
# define OPT_N_MIN   (1ULL << 39)
# define OPT_O_MIN   (1ULL << 40)
# define OPT_P_MIN   (1ULL << 41)
# define OPT_Q_MIN   (1ULL << 42)
# define OPT_R_MIN   (1ULL << 43)
# define OPT_S_MIN   (1ULL << 44)
# define OPT_T_MIN   (1ULL << 45)
# define OPT_U_MIN   (1ULL << 46)
# define OPT_V_MIN   (1ULL << 47)
# define OPT_W_MIN   (1ULL << 48)
# define OPT_X_MIN   (1ULL << 49)
# define OPT_Y_MIN   (1ULL << 50)
# define OPT_Z_MIN   (1ULL << 51)
# define OPT_0       (1ULL << 52)
# define OPT_1       (1ULL << 53)
# define OPT_2       (1ULL << 54)
# define OPT_3       (1ULL << 55)
# define OPT_4       (1ULL << 56)
# define OPT_5       (1ULL << 57)
# define OPT_6       (1ULL << 58)
# define OPT_7       (1ULL << 59)
# define OPT_8       (1ULL << 60)
# define OPT_9       (1ULL << 61)
# define OPT_ILLEGAL (1ULL << 62)


typedef	struct	s_options_parsing
{
	int			end_pos; // position of the last option
	uint64_t	opts;    // options masks
}	t_options_parsing;

typedef	struct	s_options
{
	char		opt;
	uint64_t	mask;
}	t_options;


char				find_option_char(uint64_t);
uint64_t			find_option_mask(char);
uint64_t			is_option_set(uint64_t, uint64_t);
t_options_parsing	parse_args(int, char **, bool, const char *, ...);


#endif /* !defined(RLE_PARSING_H) */
