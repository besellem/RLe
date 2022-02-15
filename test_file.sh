#!/bin/sh
# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    test_file.sh                                       :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: besellem <besellem@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/01/04 21:15:13 by besellem          #+#    #+#              #
#    Updated: 2022/01/04 21:24:19 by besellem         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# program name
PROG="RLe"

# algorithm
ALGO="-m run-length"
# ALGO="-m run-length-escape"
# ALGO=""

# colors
RED="\033[1;31m"
GREEN="\033[1;32m"
CLR_COLOR="\033[0m"


if [ $# -ne 1 ]
then
	echo "> Usage: ./"$(basename  $0)" <file_to_encode>"
	exit 1;
else
	INPUT_FILE="$1"
fi

_FILENAME="${INPUT_FILE%.*}"
FILENAME=$(basename $_FILENAME)
ENCODED_FILE="$FILENAME.rle"

if [ "${INPUT_FILE##*.}" != "$INPUT_FILE" ]; then
	DECODED_FILE="$FILENAME.${INPUT_FILE##*.}"
else
	DECODED_FILE="$FILENAME"
fi


echo $FILENAME
echo $ENCODED_FILE
echo $DECODED_FILE

echo "> Compiling ${GREEN}${PROG}${CLR_COLOR}..."
make > /dev/null

echo "> Encoding..."
./$PROG -e $ALGO -i $INPUT_FILE -o $ENCODED_FILE -v

echo "> Decoding..."
./$PROG -d $ALGO -i $ENCODED_FILE -o $DECODED_FILE -v


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
# rm $FILENAME*
exit $last
