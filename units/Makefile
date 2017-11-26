# @Author: Remi Gastaldi
# @Date:   2017-10-05T12:58:01+02:00
# @Project: Spider
# @Last modified by:   prost_m
# @Last modified time: 2017-11-26T22:38:53+01:00


SRC_DIR	= $(realpath ../src)

SRC_UT_DIR	= $(realpath .)

SRC_UT	= $(SRC_UT_DIR)/test.c

OBJ		= $(SRC_UT:.c=.o)
GCNO		= $(SRC:.c=.gcno) $(SRC_UT:.c=.gcno)
GCDA		= $(SRC:.c=.gcda) $(SRC_UT:.c=.gcda)

CFLAGS	= -Wall -Wextra --coverage -I../includes
LDFLAGS	= -lcriterion -lgcov -lcrypt -fno-stack-protector


NAME	= units

all	:	$(NAME)

$(NAME)	: $(OBJ)
					cc -o $(NAME) $(OBJ) $(LDFLAGS)

clean	:
	rm -f $(OBJ) $(GCNO) $(GCDA)
	rm -f $(NAME)

re	: fclean all
