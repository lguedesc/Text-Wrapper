# Set compiler, C Language Standard and name of the program
# To use Intel compiler (icx) in windows, insert at terminal before call make:
# call "C:\Program Files (x86)\Intel\oneAPI\setvars.bat" intel64

# To use intel compiler (icx) in linux, insert at terminal before call
# source /opt/intel/oneapi/setvars.sh

CC=gcc
CSTD=c17
NAME=test
CFLAGS=-std=$(CSTD) -Wall -Werror -Wpedantic 
CFILES= $(NAME).c wrapper.c

# The main call of make
all: 
	@$(CC) $(CFLAGs) -o $(NAME) $(CFILES)

run: 
	@./$(NAME)

clean: 
	@rm -f $(NAME)
	@rm -f *.txt
