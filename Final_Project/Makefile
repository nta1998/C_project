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
SOURCES = $(wildcard $(SRC_DIR)/*.c)
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

clean:
	rm -f $(OBJ_DIR)/*.o $(TARGET) tests/*.am tests/*.ob tests/*.ent tests/*.ext

.PHONY: clean
debug:
	@echo "SRC_DIR  = $(SRC_DIR)"
	@echo "SOURCES  = $(SOURCES)"
	@echo "OBJECTS  = $(OBJECTS)"