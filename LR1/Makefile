# ===== Compiler & flags =====
CC       ?= gcc
CFLAGS   ?= -std=c99 -Wall -Wextra -O2
INCLUDES  = -Iinclude

# Портативный режим (без __int128) — для MSVC/MinGW-32 и т.п.:
#   make PORTABLE=1 task8
ifeq ($(PORTABLE),1)
CFLAGS += -DNO_INT128
endif

# ===== Source layout =====
# Task 1
task1_SRCS = src/main_task1.c src/task1.c src/common.c
task1_LDFLAGS =

# Task 2 (нужен -lm)
task2_SRCS = src/main_task2.c src/task2.c
task2_LDFLAGS = -lm

# Task 3 (нужен -lm)
task3_SRCS = src/main_task3.c src/task3.c src/float_utils.c src/parse.c src/common.c
task3_LDFLAGS = -lm

# Task 4 (нужен -lm)
task4_SRCS = src/main_task4.c src/task4.c
task4_LDFLAGS = -lm

# Task 5
task5_SRCS = src/main_task5.c src/task5.c
task5_LDFLAGS =

# Task 6 (нужен -lm)
task6_SRCS = src/main_task6.c src/task6.c
task6_LDFLAGS = -lm

# Task 7
task7_SRCS = src/main_task7.c src/task7.c src/base_utils.c
task7_LDFLAGS =

# Task 8
task8_SRCS = src/main_task8.c src/task8.c src/base_utils.c
task8_OBJS = $(task8_SRCS:.c=.o)

# ===== Derive object lists =====
task1_OBJS = $(task1_SRCS:.c=.o)
task2_OBJS = $(task2_SRCS:.c=.o)
task3_OBJS = $(task3_SRCS:.c=.o)
task4_OBJS = $(task4_SRCS:.c=.o)
task5_OBJS = $(task5_SRCS:.c=.o)
task6_OBJS = $(task6_SRCS:.c=.o)
task7_OBJS = $(task7_SRCS:.c=.o)
task8_OBJS = $(task8_SRCS:.c=.o)

BINS = task1 task2 task3 task4 task5 task6 task7 task8

# ===== Default target =====
.PHONY: all
all: $(BINS)

# ===== Pattern rule for objects =====
# (важно: TAB перед командой!)
src/%.o: src/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# ===== Link rules per task =====
task1: $(task1_OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) $(task1_LDFLAGS) -o $@ $^

task2: $(task2_OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^ $(task2_LDFLAGS)

task3: $(task3_OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^ $(task3_LDFLAGS)

task4: $(task4_OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^ $(task4_LDFLAGS)

task5: $(task5_OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) $(task5_LDFLAGS) -o $@ $^

task6: $(task6_OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^ $(task6_LDFLAGS)

task7: $(task7_OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) $(task7_LDFLAGS) -o $@ $^

task8: $(task8_OBJS)
	$(CC) $(CFLAGS) -Iinclude -o $@ $^

# ===== Helpers =====
.PHONY: clean
clean:
	rm -f src/*.o $(BINS)

# Запуск тестов, если у тебя есть скрипт/набор тестов.
.PHONY: test
test:
	@if [ -x ./run_tests.sh ]; then ./run_tests.sh; \
	else echo "Нет run_tests.sh — добавь свои тесты в скрипт и сделай его исполняемым"; fi

# Небольшая подсказка
.PHONY: help
help:
	@echo "Targets:"
	@echo "  make all            — собрать все задания"
	@echo "  make taskN          — собрать конкретное задание (task1..task8)"
	@echo "  make clean          — удалить .o и бинарники"
	@echo "  make PORTABLE=1 ... — портативная сборка без __int128"
