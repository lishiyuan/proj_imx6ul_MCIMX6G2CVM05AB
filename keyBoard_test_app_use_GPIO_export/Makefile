VERSION = 1.0.0

CROSS_COMPILE = arm-linux-gnueabihf-
CC = gcc

SRC := $(wildcard *.c)                     #变量SRC用于存放：遍历目录下所有的.c文件
OBJ := $(patsubst %.c,%.o,$(SRC))          #变量OBJ用于存放：所有.c文件名后缀更改为.o
APP := app_keyboard                        #最终app

$(APP):$(SRC)
	$(CROSS_COMPILE)$(CC) $^ -o $(APP)

.PHONY:
print:
	@echo "APP = $(APP)"
	@echo "OBJ = $(OBJ)"
	@echo "SRC = $(SRC)"
clean:
	rm -rf $(APP)
