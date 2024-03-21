# Set compiler, C Language Standard and name of the program
# To use Intel compiler (icx) in windows, insert at terminal before call make:
# call "C:\Program Files (x86)\Intel\oneAPI\setvars.bat" intel64

# To use intel compiler (icx) in linux, insert at terminal before call
# source /opt/intel/oneapi/setvars.sh

CC=clang
CSTD=c17
NAME=wrapper
CFLAGS=-std=$(CSTD) -Wall -Werror -Wpedantic 

# The main call of make
all: 
	@$(CC) $(CFLAGs) -o $(NAME) $(NAME).c

run: 
	@./$(NAME)

clean: 
	@rm -f $(NAME)
