SRCS:=$(wildcard *.c)

thread_pool:$(SRCS)
	gcc $^ -o $@ -pthread

clean:
	rm -rf thread_pool
