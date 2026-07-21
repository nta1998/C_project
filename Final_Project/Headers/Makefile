# Makefile - בונה את קובץ ההרצה 'assembler' מכל קבצי המקור ב-src/sources,
# עם קבצי הכותרת ב-src/Headers.
#
# הדגלים -Wall -ansi -pedantic נדרשים במפורש בהגדרת הפרויקט: הקוד חייב
# להתקמפל תחת תקן ANSI C (C89) בלי אף אזהרה או הערה מהמהדר.

CC = gcc
HEADERS_DIR = src/Headers
SOURCES_DIR = src/sources
CFLAGS = -Wall -ansi -pedantic -g -I$(HEADERS_DIR)
SOURCES = $(wildcard $(SOURCES_DIR)/*.c)
OBJECTS = $(SOURCES:.c=.o)
TARGET = assembler

.PHONY: all clean test

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS)

$(SOURCES_DIR)/%.o: $(SOURCES_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)
	rm -f tests/*.am tests/*.ob tests/*.ent tests/*.ext

test: $(TARGET)
	./$(TARGET) tests/valid1.as tests/valid2.as tests/valid3.as \
	            tests/error1.as tests/error2.as tests/error3.as
