# @Author: Remi Gastaldi
# @Date:   2017-10-05T12:58:01+02:00
# @Project: Spider
# @Last modified by:   gastal_r
# @Last modified time: 2017-11-22T19:34:38+01:00


SRC_DIR	= $(realpath ../src)

SRC	= 	$(SRC_DIR)/pam.c

SRC_UT_DIR	= $(realpath .)

SRC_UT	= $(SRC_UT_DIR)/test.c

OBJ		= $(SRC:.c=.o) $(SRC_UT:.c=.o)
GCNO		= $(SRC:.c=.gcno) $(SRC_UT:.c=.gcno)
GCDA		= $(SRC:.c=.gcda) $(SRC_UT:.c=.gcda)

CFLAGS	= -Wall -Wextra --coverage
LDFLAGS	= -lcriterion -lgcov

NAME	= units

all	:	$(NAME)

$(NAME)	: $(OBJ)
					cc -o $(NAME) $(OBJ) $(LDFLAGS)

clean	:
	rm -f $(OBJ) $(GCNO) $(GCDA)
	rm -f $(NAME)

re	: fclean all
