#!/bin/sh
# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    do_test.sh                                         :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: besellem <besellem@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/01/04 13:49:26 by besellem          #+#    #+#              #
#    Updated: 2022/01/04 14:08:42 by besellem         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# program name
PROG="RLe"

# algorithm
ALGO="-m run-length"
# ALGO="-m run-length-escape"
# ALGO=""

# filenames
FILENAME=".__generated_test_file__"
INPUT_FILE="$FILENAME.txt"
ENCODED_FILE="$FILENAME.rle"
DECODED_FILE="$FILENAME""_2.txt"


# colors
RED="\033[1;31m"
GREEN="\033[1;32m"
CLR_COLOR="\033[0m"


if [ $# -ne 1 ]
then
	NBYTE="131072"
else
	NBYTE="$1"
fi

echo "> Generating ${GREEN}${NBYTE}${CLR_COLOR} random bytes..."
head -c$NBYTE /dev/random > $INPUT_FILE

echo "> Compiling ${GREEN}${PROG}${CLR_COLOR}..."
make > /dev/null

echo "> Encoding..."
./$PROG -e $ALGO -i $INPUT_FILE -o $ENCODED_FILE

echo "> Decoding..."
./$PROG -d $ALGO -i $ENCODED_FILE -o $DECODED_FILE

echo "> Comparing input file and decoded file..."
diff $INPUT_FILE $DECODED_FILE > /dev/null

last=$?
if [ $last -eq 0 ]
then
	echo "${GREEN}OK${CLR_COLOR}"
else
	echo "${RED}KO${CLR_COLOR}"
fi

make fclean > /dev/null
rm $FILENAME*
exit $last
