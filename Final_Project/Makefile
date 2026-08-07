# Makefile - בונה את קובץ ההרצה 'assembler' מכל קבצי המקור ב-src/sources,
# עם קבצי הכותרת ב-src/Headers.
#
# הדגלים -Wall -ansi -pedantic נדרשים במפורש בהגדרת הפרויקט.

CC      = gcc
CFLAGS  = -Wall -ansi -pedantic -g

SRC_DIR = ./Sources
OBJ_DIR = build
TARGET  = assembler

# כל קבצי ה-.c שנמצאים ב-src/sources
# main.c הוא דרייבר בדיקה עצמאי ל-parser עם main משלו, ולכן לא נכנס ל-assembler
SOURCES = $(filter-out $(SRC_DIR)/main.c,$(wildcard $(SRC_DIR)/*.c))
# רשימת ה-.o המקבילה, בתוך build/
OBJECTS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SOURCES))
# כל קבצי הכותרת - כל .o תלוי בכולם
HEADERS = $(wildcard ./Headers/*.h)

# היעד הראשי: מקשר את כל ה-.o לקובץ הרצה אחד
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(TARGET)

# כלל: כל .o נבנה מה-.c בעל אותו שם ותלוי בכל ה-headers
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# יוצר את תיקיית build אם אינה קיימת, לפני שמקמפלים לתוכה
$(OBJECTS): | $(OBJ_DIR)
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# דרייבר בדיקה נפרד ל-parser (Sources/main.c) - נבנה רק ב-'make parser_test'
parser_test: $(OBJ_DIR)/main.o $(OBJ_DIR)/parser.o $(OBJ_DIR)/instructions.o
	$(CC) $(CFLAGS) $^ -o parser_test

$(OBJ_DIR)/main.o: $(SRC_DIR)/main.c $(HEADERS) | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ_DIR)/*.o $(TARGET) parser_test tests/*.am tests/*.ob tests/*.ent tests/*.ext

.PHONY: clean
debug:
	@echo "SRC_DIR  = $(SRC_DIR)"
	@echo "SOURCES  = $(SOURCES)"
	@echo "OBJECTS  = $(OBJECTS)"

test_parser: Tests/test_parser.c Sources/parser.c Sources/instructions.c Sources/errors.c
	gcc -Wall -ansi -pedantic -g $^ -o build/test_parser
	./build/test_parser