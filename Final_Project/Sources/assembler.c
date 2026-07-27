#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../Headers/globals.h"
#include "../Headers/errors.h"
#include "../Headers/pre_assembler.h"
#include "../Headers/symbol_table.h"
#include "../Headers/mcro_table.h"
#include "../Headers/external_table.h"
#include "../Headers/memory_image.h"
#include "../Headers/output.h"

static void change_extension(const char *orig_file, char *new_file, const char *new_ext){
    strcpy(new_file, orig_file);
    *strrchr(new_file, '.') = '\0';
    strcat(new_file,new_ext);
}

static void reset_all_tables(void){
    err_reset();
    mcro_table_reset();
    symbol_table_reset();
    external_table_reset();
    memory_image_reset();
}

int main (int argc, char *argv[]) {

    int i, len;
    char *as_file;
    char am_file[256];
    FILE *orig_file;

    if (argc == 1){
        printf("No files found.");
        return 1;
         /* במידה ולא צורפו שום קבצים, זו לא שגיאה, התכנית פשוט לא רצה ומחזירה את הערך 1, כלומר הכל תקין. */
    }

    /*אם הגענו לכאן, סימן שיש לפחות קובץ אחד שהתקבל בטרמינל יחד עם השורת פקודה*/
    /* הקובץ הראשון יהיה במיקום argv[1]*/
    for (i = 1; i < argc; i++){

        /*בכל קליטה של קובץ חדש, אנחנו מאפסים את כל המונים וכל הטבלאות הקיימות מהריצה הקודמת, כדי שהריצה הנוכחית תתחיל מאפס.*/
        reset_all_tables();

        /*בכל ריצה נעבור על קובץ במלואו, נעדכן את הקובץ הנוכחי להיות הקובץ הבא בתור ונעדכן את האורך של השם של הקובץ*/
        as_file = argv[i];
        len = strlen(as_file);

        if (len < 3){
            printf("Invalid file name.");
            continue;
        }
        
        /*נבדוק האם הסיומת של הקובץ תקינה, כלומר מסתיים ב".as"*/
        /*אם לא, נדפיס הודעת אזהרה שהקובץ לא בסיומת נכונה ונמשיך לקובץ הבא מבלי לבדוק אותו כלל*/
        if (strcmp(as_file+len-3,".as") != 0){
            printf("Invalid file name, '.as' is missing.");
            continue;
        }

        /*אם הגענו לכאן, סימן שקיים קובץ וגם הוא בסיומת נכונה, ננסה לפתוח את הקובץ בתצורת קריאה*/
        orig_file = fopen(as_file, "r");

        /*אם לא הצלחנו לפתוח את הקובץ, נדפיס הודעת אזהרה שהקובץ לא נפתח ונמשיך לקובץ הבא.*/
        if (orig_file == NULL){
            printf("Failed to open file for reading.");
            continue;
        }

        /*אם הגענו עד לכאן, סימן שקיים קובץ בסיומת נכונה ושהוא נפתח!*/
        /*לאחר הפרה אסמבלר, אם היו שגיאות הקובץ החדש לא נוצר, במידה והכל היה תקין הקובץ נוצר.*/
        fclose(orig_file);
        change_extension(as_file, am_file, ".am");

        /* אם הוחזר מהפרה אסמבלר - FALSE*/
        /*סימן שנמצאה שגיאה/שגיאות בשלב הפרה אסאמבלר, הקבצים לא ייפלטו החוצה,והבדיקה לא תמשיך לשלבים הבאים*/
        /*כל השגיאות יודפסו כבר בהגדרה של הפרה אסמבאלר ונעבור לבדוק את הקובץ הבא.*/
        if (!pre_assembler(as_file, am_file)){
            continue;
        }

        /**
         *       אם המעבר הראשון והשני הצליחו
         *         נצהיר על שלושה מחרוזות לשמירה על שמות קבצים עתידיים
         *         נקרא לפונקציה שמשנה את הסיומת של הקבצים עבור קבצי הפלט
         *  ונקרא לפונקצייה שיוצרת את הקבצים, היא תיצור את הקובץ ob אוטומטית
         *         ואת הקבצים עם הסיומת ent ext תיצור רק במידת הצורך
         *         לאחר מכן תעבור לקובץ הבא
         */

        if (first_pass(as_file, am_file) && second_pass(as_file, am_file)){

            char ent_file[256], ext_file[256], ob_file[256];
        
            change_extension(as_file, ob_file, ".ob");
            change_extension(as_file, ent_file, ".ent");
            change_extension(as_file, ext_file, ".ext");

            write_output_files(ob_file, ent_file, ext_file);

            continue;
        }

        /*אחרת, נמחק את הקבצים שנוצרו, נסגור את הקובץ שעבדנו עליו ונמשיך לקובץ הבא.*/
        remove(am_file);

        /**
         * הבדיקה האם נמצאו שגיאות במעבר הראשוןהשני מתבצעת בתוך תכנית המעבר השני,
         * אם נמצאו שגיאות הן יודפסו, הקבצים לא יישמרו ונמשיך לקובץ הבא
         * אם הכל היה תקין, הקבצים יישמרו בתיקיית הפלט ונמשיך לקובץ הבא        
         */
    }

    return 1;

}
