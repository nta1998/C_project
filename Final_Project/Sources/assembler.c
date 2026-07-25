#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../Headers/globals.h"
#include "../Headers/errors.h"
#include "../Headers/pre_assembler.h"

static void change_extension(const char *orig_file, char *new_file, const char *new_ext){
    strcpy(new_file, orig_file);
    *strrchr(new_file, '.') = '\0';
    strcat(new_file,new_ext);
}

int assembler (int argc, char *argv[]) {

    int i, len;
    char *as_file;
    char am_file[256], ob_file[256];
    FILE *orig_file, *curr_file;

    if (argc == 1){
        printf("No files found.");
        return 1; //במידה ולא צורפו שום קבצים, זו לא שגיאה, התכנית פשוט לא רצה ומחזירה את הערך 1, כלומר הכל תקין.
    }

    //אם הגענו לכאן, סימן שיש לפחות קובץ אחד שהתקבל בטרמינל יחד עם השורת פקודה
    // הקובץ הראשון יהיה במיקום argv[1]
    for (i = 1; i < argc; i++){

        //בכל קליטה של קובץ חדש, אנחנו מאפסים את מונה השגיאות ואת מונה המאקרואים
        err_reset();
        mcro_table_reset();

        //בכל ריצה נעבור על קובץ במלואו, נעדכן את הקובץ הנוכחי להיות הקובץ הבא בתור ונעדכן את האורך של השם של הקובץ
        as_file = argv[i];
        len = strlen(as_file);

        if (len < 3){
            printf("Invalid file name.");
            continue;
        }
        
        //נבדוק האם הסיומת של הקובץ תקינה, כלומר מסתיים ב".as"
        //אם לא, נדפיס הודעת אזהרה שהקובץ לא בסיומת נכונה ונמשיך לקובץ הבא מבלי לבדוק אותו כלל
        if (strcmp(as_file+len-3,".as") != 0){
            printf("Invalid file name, '.as' is missing.");
            continue;
        }

        //אם הגענו לכאן, סימן שקיים קובץ וגם הוא בסיומת נכונה, ננסה לפתוח את הקובץ בתצורת קריאה
        orig_file = fopen(as_file, "r");

        //אם לא הצלחנו לפתוח את הקובץ, נדפיס הודעת אזהרה שהקובץ לא נפתח ונמשיך לקובץ הבא.
        if (orig_file == NULL){
            printf("Failed to open file for reading.");
            continue;
        }

        //אם הגענו עד לכאן, סימן שקיים קובץ בסיומת נכונה ושהוא נפתח!
        //לאחר הפרה אסמבלר, אם היו שגיאות הקובץ החדש לא נוצר, במידה והכל היה תקין הקובץ נוצר.
        fclose(orig_file);
        change_extension(as_file, am_file, ".am");

        // אם הוחזר מהפרה אסמבלר - FALSE
        //סימן שנמצאה שגיאה/שגיאות בשלב הפרה אסאמבלר, הקבצים לא ייפלטו החוצה,והבדיקה לא תמשיך לשלבים הבאים
        //כל השגיאות יודפסו כבר בהגדרה של הפרה אסמבאלר ונעבור לבדוק את הקובץ הבא.
        if (!pre_assembler(as_file, am_file)){
            continue;
        }

        //אחרת, לא היו שגיאות בקדם אסמבלר.
        //נעבור לביצוע המעבר הראשון והשני
        change_extension(as_file, ob_file, ".ob");
        first_pass(as_file, am_file, ob_file);
        second_pass(as_file, am_file, ob_file);
        fclose(curr_file);

        //הבדיקה האם נמצאו שגיאות במעבר הראשון/השני מתבצעת בתוך תכנית המעבר השני,
        // אם נמצאו שגיאות הן יודפסו, הקבצים לא יישמרו ונמשיך לקובץ הבא
        // אם הכל היה תקין, הקבצים יישמרו בתיקיית הפלט ונמשיך לקובץ הבא        
    }

    return 1;

}