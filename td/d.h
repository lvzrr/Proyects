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
#define ITEM_PRINTING_FORMAT "[%ld]\n%s:\n\t%s\n%s\n"

struct item {
  long id;
  char *header;
  char *body;
  char *date;
} typedef item;

void read_file() {
  FILE *fp = fopen(PATH, "rb");
  assert(fp != NULL);
  char **lines;
  fseek(fp, 0, SEEK_END);
  int size = ftell(fp);
  rewind(fp);
  char contents = fread(fp, size, 1, fp);
  assert(&contents != NULL);
  char *p = &contents;
  char *buffer;
  int ccount = 0;
  int linecount = 0;

  while (*p != EOF) {
    if (*p == '\n') {
      buffer = (char *)malloc(ccount * sizeof(char) + 1);
      assert(buffer != NULL);
      buffer = strncpy(buffer, p - ccount, ccount);
      lines[linecount] = buffer;
      linecount++;
    } else {
      ccount++;
    }
    p++;
  }
}

char *get_date() {
  time_t t = time(NULL);
  struct tm tm = *localtime(&t);
  char *date = (char *)malloc(256);
  sprintf(date, "%d-%d-%d %d:%d:%d", tm.tm_year + 1900, tm.tm_mon + 1,
          tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
  return date;
}

char *get_header() {
  char *header;
  printf("Enter header: ");
  scanf("%s", header);
  return header;
}

char *get_body() {
  char *body;
  printf("Enter body: ");
  scanf("%s", body);
  return body;
}

long gen_id(char *header) {
  int l = strlen(header);
  long id = 10002;
  for (int i = 0; i < l; i++) {
    id *= header[i];
  }
  if (id > LONG_MAX || id < 0) {
    id = LONG_MAX - strlen(header);
  }
  return id;
}

item get_user_input() {
  char *header = get_header();
  char *body = get_body();
  char *date = get_date();
  long id = gen_id(header);
  assert(header != NULL);
  assert(body != NULL);
  assert(date != NULL);
  item task = {id, header, body, date};
  return task;
}

int write_task(item *task) {
  FILE *fp = fopen(PATH, "a");
  assert(fp != NULL);
  assert(task != NULL);
  long id = task->id;
  char *header = task->header;
  char *body = task->body;
  char *date = task->date;
  char *toappend = (char *)malloc(sizeof(header) + sizeof(body) + sizeof(date) +
                                  sizeof(id) + 4);
  sprintf(toappend, "%ld%%%s%%%s%%%s\n", id, header, body, date);
  assert(toappend != NULL);
  fwrite(toappend, sizeof(toappend), 1, fp);
  fclose(fp);
  free(task);
  return 0;
}

int add_todo() {

  FILE *efp = fopen(ENC_PATH, "rb");
  FILE *fp = fopen(PATH, "rb");

  /*
   * CHECK IF THE ENCRYPTED FILE EXISTS,
   * IF SO, DECRYPT IT
   *
   */

  if (efp != NULL && fp == NULL) {
    decrypt_file(ENC_PATH);
    fclose(efp);
    fclose(fp);
  }

  /* CHECK IF ANY FILE EXISTS, IF NOT CREATE IT */

  else if (efp == NULL && fp == NULL) {
    fclose(fp);
    FILE *fp = fopen(PATH, "wb");
    fputc('\0', fp);
    assert(fp != NULL);
    fclose(fp);
    fclose(efp);
  }

  item task = get_user_input();
  write_task(&task);

  return gen_encrypted_files(PATH);
}

#endif
