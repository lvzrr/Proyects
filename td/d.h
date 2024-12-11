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
 *  - ADD A FUNCTION TO DELETE A TASK
 *
 */

#define PATH "/home/lvx/.tasks/tasks"
#define ENC_PATH "/home/lvx/.tasks/.tasks.crypt"
#define KEY_PATH "/home/lvx/.keys/tasks.key"
#define ITEM_PRINTING_FORMAT "\n[%ld]\n%s:\n\t%s\n%s\n"

struct item {
  long id;
  char *header;
  char *body;
  char *date;
} typedef item;

void ensure_existence(char *path) {

  FILE *fp = fopen(path, "a+");
  fputc(' ', fp);
  fclose(fp);
}

int check_existence(char *path) {
  FILE *fp = fopen(path, "r");
  if (fp == NULL) {
    return 0;
  }
  fclose(fp);
  return 1;
}
int get_task_count() {

  FILE *fp = fopen(PATH, "a+");

  assert(fp != NULL);

  char c;
  int lines = 0;

  while ((c = fgetc(fp)) != EOF) {
    if (c == '\n') {
      lines++;
    }
  }

  fclose(fp);
  return lines;
}

void display_tasks(item *tasks, int taskc) {
  int i = 0;
  while (i < taskc) {
    printf(ITEM_PRINTING_FORMAT, tasks[i].id, tasks[i].header, tasks[i].body,
           tasks[i].date);
    i++;
  }
}

void display_task(item task) {
  printf(ITEM_PRINTING_FORMAT, task.id, task.header, task.body, task.date);
}
int *analyse_file(int lines) {

  FILE *fp = fopen(PATH, "rb");

  if (lines == 0) {
    return 0;
  }

  int *line_char_count = (int *)malloc(sizeof(int *) * (lines));
  int ch = 0;
  int i = 0;
  char c;

  while ((c = fgetc(fp)) != EOF) {
    if (c != '\n') {
      ch++;
    } else {
      line_char_count[i++] = ch + 1;
      ch = 0;
    }
  }

  assert(line_char_count != NULL);
  fclose(fp);
  return line_char_count;
  free(line_char_count);
}

item *get_lines(int c_lines) {

  int *lines = analyse_file(c_lines);

  int i = 0;

  item *tasks = (item *)malloc(sizeof(item) * (c_lines + 1));

  if (c_lines == 0) {
    return tasks;
  }
  FILE *fp = fopen(PATH, "rb");
  assert(fp != NULL);

  int c_tasks = 0;
  for (int j = 0; j < c_lines; j++) {
    char *line = (char *)malloc(sizeof(char) * (lines[j] + 3));
    fgets(line, lines[j] + 1, fp);

    if (line != NULL && strlen(line) > 1 && line[0] != '\n' && line[0] != ' ' &&
        line[0] != '\0' && line[0] != '\t' && line[0] != '\r') {

      const char *delim = "%";

      char *token = strtok(line, delim);

      int it = 0;

      char *header;
      char *body;
      char *date;
      long id = 0;

      while (token != NULL) {
        switch (it) {
        case 0:
          id = atol(token);
          assert(token != NULL);
          break;
        case 1:
          header = (char *)malloc(strlen(token) * sizeof(char) + 1);
          strcpy(header, token);
          assert(header != NULL);
          break;
        case 2:
          body = (char *)malloc(strlen(token) * sizeof(char) + 1);
          strcpy(body, token);
          assert(body != NULL);
          break;
        case 3:
          date = (char *)malloc(strlen(token) * sizeof(char) + 1);
          strcpy(date, token);
          assert(date != NULL);
          break;
        }
        token = strtok(NULL, delim);
        it++;
      }
      item task = {id, header, body, date};
      tasks[c_tasks] = task;
      c_tasks++;
      it = 0;
    }
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
  char *header = (char *)malloc(255);
  if (!header) {
    perror("Failed to allocate memory for header");
    exit(EXIT_FAILURE);
  }
  printf("Enter header: ");
  if (fgets(header, 255, stdin) == NULL) {
    perror("Error reading header");
    exit(EXIT_FAILURE);
  }
  header[strcspn(header, "\n")] = '\0';
  return header;
}

char *get_body() {
  char *body = (char *)malloc(800);
  if (!body) {
    perror("Failed to allocate memory for body");
    exit(EXIT_FAILURE);
  }
  printf("Enter body: ");
  if (fgets(body, 800, stdin) == NULL) {
    perror("Error reading body");
    exit(EXIT_FAILURE);
  }
  body[strcspn(body, "\n")] = '\0';
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

void write_tasks(int task_c, item *tasks, item task) {

  FILE *fp = fopen(PATH, "w");

  assert(fp != NULL);
  assert(task.id != 0);
  printf("Writing task %s to file\n", task.header);
  for (int i = 0; i < task_c; i++) {
    fprintf(fp, "%ld%%%s%%%s%%%s\n", tasks[i].id, tasks[i].header,
            tasks[i].body, tasks[i].date);
  }

  fclose(fp);
}

void add_todo() {

  char *path = (char *)malloc(sizeof(PATH) + sizeof(char) * 2);
  strcpy(path, PATH);
  char *key_path = (char *)malloc(sizeof(KEY_PATH) + sizeof(char) * 2);
  strcpy(key_path, KEY_PATH);
  char *enc_path = (char *)malloc(sizeof(ENC_PATH) + sizeof(char) * 2);
  strcpy(enc_path, ENC_PATH);

  ensure_existence(path);

  if (check_existence(enc_path)) {
    decrypt_file(enc_path, key_path, path);
  }

  int task_c = get_task_count();

  item *task_arr = get_lines(task_c);

  item task = get_user_input();

  task_arr[task_c] = task;

  task_c++;

  printf("trying to display last task\n");

  display_tasks(task_arr, task_c);

  write_tasks(task_c, task_arr, task);

  gen_encrypted_files(path, key_path, enc_path);
}

void list_tasks() {

  char *path = (char *)malloc(sizeof(PATH) + sizeof(char));
  strcpy(path, PATH);
  char *key_path = (char *)malloc(sizeof(KEY_PATH) + sizeof(char));
  strcpy(key_path, KEY_PATH);
  char *enc_path = (char *)malloc(sizeof(ENC_PATH) + sizeof(char));
  strcpy(enc_path, ENC_PATH);

  if (!check_existence(path) && !check_existence(enc_path)) {
    printf("No file %s or %s found\n", path, enc_path);
    exit(EXIT_FAILURE);
  }

  if (check_existence(enc_path)) {
    decrypt_file(enc_path, key_path, path);
  }

  int task_c = get_task_count();

  item *tasks = get_lines(task_c);

  display_tasks(tasks, task_c);

  gen_encrypted_files(path, key_path, enc_path);
}

void remove_task_files() {
  remove(PATH);
  remove(ENC_PATH);
  remove(KEY_PATH);
}

#endif
