#ifndef D_H
#define D_H

#include "obsfusc.h"
#include <assert.h>
#include <limits.h>

/* DB FORMAT
 *
 *  ID%HEADER%BODY%DATE\n
 *
 */

/* TODO:
 *
 *  - PARSE A LINE AND APPEND THE RESULT TO A ITEM ARRAY,
 *    RETURN THE ARRAY AND PRINT IT [read_file()]
 *
 *  - ADD A FUNCTION TO DELETE A TASK
 *
 *  - IM NOT ADDING MODIFICATION OF TASKS, FUCK THAT,
 *    JUST MAKE ANOTHER ONE
 *
 *  - ALSO IMPLEMENT A DISPLAY METHOD FOR THE ITEM TYPE
 *
 */

#define PATH "/usr/local/task/tasks"
#define ENC_PATH "/usr/local/task/tasks.crypt"
struct item {
  long id;
  char *header;
  char *body;
  char *date;
} typedef item;

char *get_date(void);

char *get_header(void);

char *get_body(void);

long gen_id(char *header);

void read_file(void);

item get_user_input(void);

int write_task(item *task);

int add_todo(void);

int decrypt_file(const char *path);

int gen_encrypted_files(const char *path);

#endif
