CROSS_COMPILE = arm-linux-gnueabihf-
CC = gcc
RM = rm

SRC := $(wildcard *.c)
APP := $(patsubst %.c,% ,$(wildcard *.c))

$(APP) : $(SRC)
	$(CROSS_COMPILE)$(CC) $< -o $@

.PHONY:
print:
	@echo "APP = $(APP)"
	@echo "SRC = $(SRC)"
clean:
	$(RM) $(APP) -rf
