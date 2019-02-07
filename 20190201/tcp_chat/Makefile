CC:=gcc
SRCS:=$(wildcard *.c)
TARGETS:=$(SRCS:%.c=%)
LINK:=-pthread

all:$(TARGETS)
%:%.c
	$(CC) $^ -o $@ $(LINK)

clean:
	rm -rf ${TARGETS}
