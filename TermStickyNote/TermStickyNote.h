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
 * - MAKE A MENU THAT RUNS LIKE A TUI
 *
 */

#define PATH "/home/lvx/.config/.tasks/tasks.txt"
#define ENC_PATH "/home/lvx/.config/.tasks/tasks.crypt"
#define KEY_PATH "/home/lvx/.config/.keys/tasks.key"
#define ITEM_PRINTING_FORMAT                                                   \
  "\n\033[1;32m[+] %s:\033[0m\n\n\t%s\n\n%s -> \033[1;32m[%ld]\033[0m\n\n"
#define HEADER_LENGTH 256
#define BODY_LENGTH 801
#define DATE_LENGTH 256

struct item {
  long id;
  char header[HEADER_LENGTH];
  char body[BODY_LENGTH];
  char date[DATE_LENGTH];
} typedef item;

void ensure_existence(char *path) {
  FILE *fp = fopen(path, "a+");
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

  char c, prev;
  int lines = 0;

  while ((c = fgetc(fp)) != EOF) {
    if (c == '\n') {
      lines++;
    }
  }

  fclose(fp);
  return lines;
}

char *format_body_printing(char *body) {

  int len = strlen(body);
  int out_index = 0;
  int cspace = 1;

  char *out = (char *)malloc(sizeof(char) * BODY_LENGTH);
  if (!out) {
    perror("malloc failed");
    return NULL;
  }

  for (int i = 0; i < len; i++) {
    out[out_index++] = body[i];

    if (body[i] == ' ') {
      cspace++;
    }

    if (cspace == 10) {
      out[out_index++] = '\n';
      out[out_index++] = '\t';
      cspace = 0;
    }
  }

  out[out_index] = '\0';
  return out;
}

void display_task(item task) {
  if (task.id != 0) {

    task.date[strcspn(task.date, "\n")] = '\0';
    printf(ITEM_PRINTING_FORMAT, task.header, format_body_printing(task.body),
           task.date, task.id);
  }
}
void display_tasks(item *tasks, int taskc) {
  int i = 0;
  while (i != taskc) {
    display_task(tasks[i]);
    i++;
  }
}

int *analyse_file(int lines) {

  FILE *fp = fopen(PATH, "rb");

  if (lines == 0) {
    return 0;
  }

  int *line_char_count = (int *)malloc(sizeof(int *) * (lines + 1));
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
    char *line = (char *)malloc(sizeof(char) * (lines[j] + 1));
    fgets(line, lines[j] + 1, fp);
    strncat(line, "\0", 1);
    if (line != NULL && strlen(line) > 5 && line[0] != '\n' && line[0] != ' ' &&
        line[0] != '\0' && line[0] != '\t' && line[0] != '\r' &&
        line[0] != '0') {
      const char *delim = "%";

      char *token = strtok(line, delim);

      int it = 0;

      long id = 0;

      while (token != NULL) {
        switch (it) {
        case 0:
          tasks[c_tasks].id = atol(token);
          assert(token != NULL);
          break;
        case 1:
          strcpy(tasks[c_tasks].header, token);
          assert(tasks[c_tasks].header != NULL);
          break;
        case 2:
          strcpy(tasks[c_tasks].body, token);
          assert(tasks[c_tasks].body != NULL);
          break;
        case 3:
          strcpy(tasks[c_tasks].date, token);
          assert(tasks[c_tasks].date != NULL);
          break;
        }
        token = strtok(NULL, delim);
        it++;
      }
      c_tasks++;
      it = 0;
      free(line);
    }
  }

  fclose(fp);
  return tasks;
}

char *get_date() {
  time_t t = time(NULL);
  struct tm tm = *localtime(&t);
  char *date = (char *)malloc(DATE_LENGTH);
  sprintf(date, "%d-%d-%d %d:%d:%d", tm.tm_year + 1900, tm.tm_mon + 1,
          tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
  return date;
}

char *get_header() {
  char *header = (char *)malloc(sizeof(char) * HEADER_LENGTH);
  if (!header) {
    perror("Failed to allocate memory for header");
    exit(EXIT_FAILURE);
  }

  printf("Enter header: ");
  if (fgets(header, HEADER_LENGTH, stdin) == NULL) {
    perror("Error reading header");
    exit(EXIT_FAILURE);
  }
  header[strcspn(header, "\n")] = '\0';
  return header;
}

char *get_body() {
  char *body = (char *)malloc(sizeof(char) * BODY_LENGTH);
  if (!body) {
    perror("Failed to allocate memory for body");
    exit(EXIT_FAILURE);
  }
  printf("Enter body: ");
  if (fgets(body, BODY_LENGTH, stdin) == NULL) {
    perror("Error reading body");
    exit(EXIT_FAILURE);
  }
  body[strcspn(body, "\n")] = '\0';
  return body;
}
long gen_id(char *header, char *body) {
  int l = strlen(header);
  long id = 11;
  for (int i = 0; i < l; i++) {
    id *= (header[i]) * (strlen(body));
  }
  if (id > LONG_MAX || id < 0) {
    id = LONG_MAX - (strlen(header));
  }
  assert(id != 0);
  return id;
}

item get_user_input() {
  char h[HEADER_LENGTH];
  char b[BODY_LENGTH];
  char d[DATE_LENGTH];

  strcpy(h, get_header());
  strcpy(b, get_body());
  strcpy(d, get_date());

  long id = gen_id(h, b);

  item task;
  task.id = id;
  strncpy(task.header, h, HEADER_LENGTH);
  task.header[HEADER_LENGTH - 1] = '\0';
  strncpy(task.body, b, BODY_LENGTH);
  task.body[BODY_LENGTH - 1] = '\0';
  strncpy(task.date, d, DATE_LENGTH);
  task.date[DATE_LENGTH - 1] = '\0';

  return task;
}
void write_task(int task_c, item task) {

  if (task_c == 1) {
    FILE *fp = fopen(PATH, "w");

    assert(fp != NULL);
    assert(task.id != 0);

    task.date[strcspn(task.date, "\n")] = '\0';

    if (task.id != 0) {
      fprintf(fp, "%ld%%%s%%%s%%%s\n", task.id, task.header, task.body,
              task.date);
    }
    fclose(fp);
  } else {
    FILE *fp = fopen(PATH, "a");

    assert(fp != NULL);
    assert(task.id != 0);

    task.date[strcspn(task.date, "\n")] = '\0';

    if (task.id != 0) {
      fprintf(fp, "%ld%%%s%%%s%%%s\n", task.id, task.header, task.body,
              task.date);
    }
    fclose(fp);
  }
}

void write_tasks_exclude_id(int task_c, item *tasks, long id, char *path) {

  remove(PATH);

  ensure_existence(path);

  for (int i = 0; i < task_c; i++) {
    if (tasks[i].id != 0 && tasks[i].id != id) {
      write_task(get_task_count() + 1, tasks[i]);
    }
  }
}

void add_todo() {

  char *path = (char *)malloc(sizeof(PATH) + sizeof(char));
  strcpy(path, PATH);
  char *key_path = (char *)malloc(sizeof(KEY_PATH) + sizeof(char));
  strcpy(key_path, KEY_PATH);
  char *enc_path = (char *)malloc(sizeof(ENC_PATH) + sizeof(char));
  strcpy(enc_path, ENC_PATH);

  ensure_existence(path);

  if (check_existence(enc_path)) {
    decrypt_file(enc_path, key_path, path);
  }

  int task_c = get_task_count();

  item *task_arr = get_lines(task_c);
  item task = get_user_input();
  task_arr[task_c++] = task;

  write_task(task_c, task);

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

long get_delete_id() {
  long id;
  printf("Enter the id of the task you want to delete: ");
  scanf("%ld", &id);
  return id;
}

void delete_task() {

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

  long id = get_delete_id();

  int task_c = get_task_count();

  item *tasks = get_lines(task_c);

  write_tasks_exclude_id(task_c, tasks, id, path);

  gen_encrypted_files(path, key_path, enc_path);
}

void remove_task_files() {
  remove(PATH);
  remove(ENC_PATH);
  remove(KEY_PATH);
}

#endif
