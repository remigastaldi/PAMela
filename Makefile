# @Author: Remi Gastaldi <gastal_r>
# @Date:   2017-11-22T17:40:25+01:00
# @Last modified by:   gastal_r
# @Last modified time: 2017-11-26T21:13:50+01:00


##
## Makefile for Makefile in /home/gastal_r/rendu/PSU_2016_philu
##
## Made by
## Login   <gastal_r>
##
## Started on  Fri Jan 27 12:28:44 2017
## Last update Sat Feb 11 14:27:40 2017
##

ECHO		=	/bin/echo -e
DEFAULT		=	"\033[00m"
GREEN		=	"\033[0;32m"
TEAL		=	"\033[1;36m"
RED		=	"\033[0;31m"
YELLOW		=	"\033[1;33m"
BLUE		= 	"\033[1;34m"

CC		=       gcc -D_GNU_SOURCE

RM		=       rm -rf

CFLAGS		+=	-Wall -Wextra -W #-Werror
CFLAGS		+=	-fPIC -c -lcryptsetup -fno-stack-protector
CFLAGS		+=	-I./includes
LDFLAGS		+= -shared

NAME		=	mypam.so

PATH_INSTALL 	=	/lib/x86_64-linux-gnu/security/mypam.so

PATH_CONFIG_AUTH = /etc/pam.d/common-auth
PATH_CONFIG_ACCOUNT = /etc/pam.d/common-account
PATH_CONFIG_PASSWORD = /etc/pam.d/common-password
PATH_CONFIG_SESSION = /etc/pam.d/common-session

SRC		=	src/pam.c \
				src/container.c \
				src/folder.c

OBJS		=	$(SRC:.c=.o)

all		:	$(NAME)

$(NAME)		:	$(OBJS)
			$(CC) -o $(NAME) $(OBJS) $(LDFLAGS)

clean		:
			$(RM) $(NAME)
			$(RM) $(OBJS)
			@make clean -C units/

re		:       clean all


install	:	$(NAME)
					@if (grep -q "auth sufficient mypam.so" $(PATH_CONFIG_AUTH)); \
					then echo "AUTH CONFIG EXIST"; \
					else \
					 echo "auth sufficient mypam.so" >> $(PATH_CONFIG_AUTH); \
					fi; \
					if (grep -q "account sufficient mypam.so" $(PATH_CONFIG_ACCOUNT)); \
					then echo "ACCOUNT CONFIG EXIST"; \
					else \
					 echo "account sufficient mypam.so" >> $(PATH_CONFIG_ACCOUNT); \
					fi; \
					if (grep -q "password sufficient mypam.so" $(PATH_CONFIG_PASSWORD)); \
					then echo "PASSWORD CONFIG EXIST"; \
					else \
					 echo "password sufficient mypam.so" >> $(PATH_CONFIG_PASSWORD); \
					fi; \
					if (grep -q "session sufficient mypam.so" $(PATH_CONFIG_SESSION)); \
					then echo "SESSION CONFIG EXIST"; \
					else \
					 echo "session sufficient mypam.so" >> $(PATH_CONFIG_SESSION); \
					fi; \
					ld -x --shared -o $(PATH_INSTALL) $(OBJS) \
							&& $(ECHO) $(GREEN) "INSTALLATION SUCCESSFULL" $(BLUE) $(NAME) $(DEFAULT)  || \
								$(ECHO) $(RED) "INSTALLATION FAILED : are you root ?" $(BLUE) $(NAME) $(DEFAULT)

uninstall	:
					@sed -i "/\b\(auth sufficient mypam.so\)\b/d" $(PATH_CONFIG_AUTH) \
					&& sed -i "/\b\(account sufficient mypam.so\)\b/d" $(PATH_CONFIG_ACCOUNT) \
					&& sed -i "/\b\(password sufficient mypam.so\)\b/d" $(PATH_CONFIG_PASSWORD) \
					&& sed -i "/\b\(session sufficient mypam.so\)\b/d" $(PATH_CONFIG_SESSION) \
					&& rm $(PATH_INSTALL)	&& \
							$(ECHO) $(GREEN) "DELETE SUCCESSFUL" $(BLUE) $(NAME) $(DEFAULT)  || \
							$(ECHO) $(RED) "DELETE FAILED" $(BLUE) $(NAME) $(DEFAULT)

check			:
					@test -f $(PATH_INSTALL) && \
							$(ECHO) $(GREEN) "MODULE IS INSTALLED" $(BLUE) $(NAME) $(DEFAULT) \
							|| $(ECHO) $(RED) "MODULE IS NOT INSTALLED" $(BLUE) $(NAME) $(DEFAULT)

test 			:
					@$(ECHO) $(GREEN) "\nCOMPILE TEST" $(DEFAULT)
					@make -C units/
					@$(ECHO)  $(BLUE) "\nRUN TESTS" $(DEFAULT)
					./units/units

.PHONNY		:       all clean re
