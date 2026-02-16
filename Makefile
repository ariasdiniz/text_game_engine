LIBNAME = tge
LIB = (lib$(LIBNAME).so)
SRC = $(shell find . -type f -name "*.c" ! -name "main.c")
OBJS = $(shell echo $(SRC) | sed s/\.c/\.o/g)
