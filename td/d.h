#ifndef D_H
#define D_H

#include "obsfusc.h"
#include <assert.h>
#include <limits.h>
#include <stdio.h>

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
 *  - FIGURE OUT HOW TO NOT STATICALLY ALLOCATE THE LINE
 *
 *  - ADD A FUNCTION TO DELETE A TASK
 *
 *  - IM NOT ADDING MODIFICATION OF TASKS, FUCK THAT,
 *    JUST MAKE ANOTHER ONE
 *
 *  - ALSO IMPLEMENT A DISPLAY METHOD FOR THE ITEM TYPE
 *
 */

#define PATH "/home/lvx/.config/doers/todo.txt"
#define ENC_PATH "/usr/local/task/tasks.crypt"
#define ITEM_PRINTING_FORMAT "[%ld]\n%s:\n\t%s\n%s\n"

struct item {
  long id;
  char *header;
  char *body;
  char *date;
} typedef item;

int *analyse_file() {

  FILE *fp = fopen(PATH, "rb");

  assert(fp != NULL);

  char c;
  int lines = 0;

  while ((c = fgetc(fp)) != EOF) {
    if (c == '\n') {
      lines++;
    }
  }

  rewind(fp);
  assert(lines != 0);

  int *line_char_count = (int *)malloc(sizeof(int *) * (lines + +11));
  int ch = 0;
  int i = 0;

  while ((c = fgetc(fp)) != EOF) {
    if (c != '\n') {
      ch++;
    } else {
      line_char_count[i++] = ch + 1;
      ch = 0;
    }
  }
  line_char_count[lines] = 888888;

  assert(line_char_count != NULL);

  return line_char_count;
  free(line_char_count);
}

item *get_lines() {

  int *lines = analyse_file();

  assert(lines != NULL);

  int i = 0;
  int c_lines = 0;

  while (lines[i] != 888888) {
    i++;
    c_lines++;
  }

  FILE *fp = fopen(PATH, "rb");
  assert(fp != NULL);

  item *tasks = (item *)malloc(sizeof(item) * c_lines);

  for (int j = 0; j < c_lines; j++) {
    char *line = (char *)malloc(sizeof(char) * lines[j]);
    fgets(line, lines[j] + 1, fp);

    const char *delim = "%";

    char *token = strtok(line, delim);

    int it = 0;
    int c_tasks = 0;

    while (token != NULL) {
      printf("%s\n", token);
      char *header;
      char *body;
      char *date;
      long id = 0;

      switch (it) {
      case 0:
        token = (char *)malloc(strlen(token) * sizeof(char));
        date = token;
        assert(date != NULL);
        break;
      case 1:
        id = atol(token);
        assert(id != 0);
        break;
      case 2:
        header = (char *)malloc(strlen(token) * sizeof(char));
        header = token;
        assert(header != NULL);
        break;
      case 3:
        body = (char *)malloc(strlen(token) * sizeof(char));
        body = token;
        assert(body != NULL);
        break;
      }
      it++;
      token = strtok(NULL, delim);
      item task = {id, header, body, date};
      tasks[c_tasks] = task;
      c_tasks++;
    }
    it = 0;
    c_tasks = 0;
    free(line);
  }
  fclose(fp);
  return tasks;
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
    char *encpath = (char *)malloc(sizeof(ENC_PATH));
    strcpy(encpath, ENC_PATH);
    decrypt_file(encpath);
    fclose(efp);
    fclose(fp);
    free(encpath);
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
