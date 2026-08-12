# Makefile - בונה את קובץ ההרצה 'assembler' מכל קבצי המקור ב-src/sources,
# עם קבצי הכותרת ב-src/Headers.
#
# הדגלים -Wall -ansi -pedantic נדרשים במפורש בהגדרת הפרויקט.

CC      = gcc
CFLAGS  = -Wall -ansi -pedantic -g

SRC_DIR = ./Sources
OBJ_DIR = build
TARGET  = assembler

# כל קבצי ה-.c שנמצאים ב-src/sources.
# assembler.c הוא ה-main היחיד בפרויקט.
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
	rm -f $(OBJ_DIR)/*.o $(TARGET)
	rm -f $(OBJ_DIR)/test_parser $(OBJ_DIR)/fp_driver
	rm -f $(OBJ_DIR)/parser_driver $(OBJ_DIR)/sp_driver
	$(MAKE) clean-output

# מוחק רק את פלט האסמבלר, בלי לגעת בקבצי ההרצה. הרצה ידנית של
# ./assembler כותבת את הפלט ליד קובץ הקלט, ולכן הוא מצטבר בכל תיקייה
# שיש בה קובץ מקור. היעד הזה מנקה את כולן בפקודה אחת.
#
# תיקיות הבסיס מוחרגות: שם יושבים הקבצים שאליהם הבדיקות משוות.
# מחיקתם לא הייתה נראית כשגיאה - הבדיקות פשוט היו מדווחות
# 'not verified' ועוברות, כלומר הופכות לירוקות בלי לבדוק דבר.
#
# ההחרגה תופסת גם 'expected' וגם 'Exepted' (שגיאת כתיב שקיימת בפרויקט),
# כי התאמה לפי שם מדויק הייתה מחמיצה את השנייה ומוחקת קבצים אמיתיים.
#
# הערה על find: אי אפשר להשתמש כאן ב--delete, כי הוא מפעיל -depth
# אוטומטית ואז -prune מפסיק לעבוד - כלומר קבצי הבסיס היו נמחקים.
# -exec rm אינו מפעיל -depth, ולכן ה--prune נשמר.
#
# ההחרגה היא רשת ביטחון שנייה. הרשת הראשונה: כל קובץ שנמצא במעקב git
# מדולג במפורש, כי פלט זמני לעולם אינו במעקב. גם אם ייווצר בעתיד ספריית
# בסיס בשם אחר, קבציה לא יימחקו.
clean-output:
	@find Tests \( -type d \( -iname 'expected' -o -iname 'exepted' \) -prune \) -o \
		-type f \( -name '*.am' -o -name '*.ob' -o -name '*.ent' -o -name '*.ext' \) \
		-print | while read -r f; do \
			if git ls-files --error-unmatch "$$f" >/dev/null 2>&1; then \
				echo "  kept (tracked by git): $$f"; \
			else \
				echo "  removed: $$f"; rm -f "$$f"; \
			fi; \
		done
	@echo "output files removed (baselines and tracked files kept)"

debug:
	@echo "SRC_DIR  = $(SRC_DIR)"
	@echo "SOURCES  = $(SOURCES)"
	@echo "OBJECTS  = $(OBJECTS)"

test_parser: Tests/test_parser.c Sources/parser.c Sources/instructions.c Sources/errors.c
	gcc -Wall -ansi -pedantic -g $^ -o build/test_parser
	./build/test_parser

# ---------------------------------------------------------------------------
# בדיקות
# ---------------------------------------------------------------------------

# בדיקות קצה-לקצה: מריץ את האסמבלר על קבצי .as שלמים ומשווה את הפלט
# (.am/.ob/.ent/.ext) לקבצים הצפויים. תלוי ב-$(TARGET), כלומר בונה קודם.
# הרצת בדיקה בודדת: make e2e CASE=valid_data
e2e: $(TARGET)
	bash Tests/e2e/run_e2e.sh $(CASE)

# רישום הפלט הנוכחי כבסיס הצפוי. להריץ רק כשהתוכנה עובדת נכון,
# ולעבור על הקבצים שנרשמו ידנית - אחרת מקבעים באג כ'התנהגות צפויה'.
e2e-bless: $(TARGET)
	bash Tests/e2e/run_e2e.sh --bless

# בדיקת אבחון: מריצה קובץ מקור אחד ומדווחת על שני סוגי בעיות נפרדים -
# שגיאות בקובץ ה-.as (עם השורה שגרמה להן), ושגיאות זיכרון בקוד ה-C עצמו
# (תחת valgrind, עם השורה בקובץ ה-.c). קובץ יכול לעבור אסמבלי נקי ועדיין
# לדרוס זיכרון, ולכן שתי הבדיקות תמיד רצות.
#   make check                      כל הקבצים ב-Tests/e2e
#   make check CASE=valid_macro     קובץ בודד לפי שם
#   make check FILE=path/to/x.as    כל קובץ .as בנתיב
#   make check CASE=valid_macro MEM=0    בלי valgrind (מהיר יותר)
check: $(TARGET)
	bash Tests/e2e/run_check.sh $(CASE) $(FILE) $(if $(filter 0,$(MEM)),--no-mem,)

# השוואה שורה-שורה מול קובץ הציפיות. בעוד 'check' עונה "כמה שגיאות יש",
# היעד הזה עונה "איזו שורה שגויה ומה היה אמור לצאת בה" - כלומר מצביע על
# הבדיקה החסרה בקוד במקום לדווח מספר.
#
# הציפיות נקראות ישירות מקובץ ה-.expected.txt, כך שהן קיימות במקום אחד
# בלבד: עריכת קובץ הבסיס משנה את הבדיקה, בלי עותק שני שיכול להתיישן.
#
# כל שורה מסווגת: ok / MISSED (בדיקה חסרה) / wrong code / false positive.
#   make expect                              כל קובץ שיש לו בסיס
#   make expect FILE=Tests/first_pass_errors.as
expect: $(TARGET)
	bash Tests/e2e/run_expect.sh $(FILE)

# דרייבר מבודד ל-first_pass: מריץ אותו לבד ומדפיס את טבלת הסמלים,
# תמונת הקוד ותמונת הנתונים. שימושי לאיתור באגים בלי הפרה-אסמבלר.
#   make fp_driver FILE=Tests/e2e/valid_basic.as
# בלי FILE הוא רק נבנה, ואפשר להריץ ידנית: ./build/fp_driver <file>
FP_SOURCES = Tests/fp_driver.c Sources/first_pass.c Sources/parser.c \
             Sources/instructions.c Sources/errors.c Sources/symbol_table.c \
             Sources/memory_image.c

fp_driver: $(FP_SOURCES) $(HEADERS) | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(FP_SOURCES) -o $(OBJ_DIR)/fp_driver
ifneq ($(FILE),)
	./$(OBJ_DIR)/fp_driver $(FILE)
endif

# דרייבר מבודד ל-parser: מריץ את line_split על כל שורה ומדפיס איך היא
# פורקה - סוג, תווית, שם ואופרנדים. השלב היחיד שבאמת מבודד: הוא אינו
# נוגע בטבלת הסמלים ולא בתמונת הזיכרון.
#   make parser_driver FILE=Tests/e2e/valid_basic.as
PARSER_SOURCES = Tests/parser_driver.c Sources/parser.c \
                 Sources/instructions.c Sources/errors.c

parser_driver: $(PARSER_SOURCES) $(HEADERS) | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(PARSER_SOURCES) -o $(OBJ_DIR)/parser_driver
ifneq ($(FILE),)
	./$(OBJ_DIR)/parser_driver $(FILE)
endif

# דרייבר ל-second_pass: מריץ first_pass ואז second_pass, ומדפיס את
# הטבלאות פעמיים כדי שיהיה אפשר לראות מה המעבר השני שינה. אינו מבודד -
# second_pass קורא את הטבלאות ש-first_pass בנה ולכן חייב לרוץ אחריו.
#   make sp_driver FILE=Tests/e2e/valid_basic.as
SP_SOURCES = Tests/sp_driver.c Sources/first_pass.c Sources/second_pass.c \
             Sources/parser.c Sources/instructions.c Sources/errors.c \
             Sources/symbol_table.c Sources/memory_image.c \
             Sources/external_table.c Sources/output.c

sp_driver: $(SP_SOURCES) $(HEADERS) | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(SP_SOURCES) -o $(OBJ_DIR)/sp_driver
ifneq ($(FILE),)
	./$(OBJ_DIR)/sp_driver $(FILE)
endif

# כל הבדיקות יחד
test: test_parser e2e

.PHONY: clean clean-output debug test e2e e2e-bless check expect test_parser fp_driver parser_driver sp_driver