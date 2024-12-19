
#include "keys.h"
#include <raylib.h>

char *UpdateBuffer(char *buffer, char newChar) {
  size_t b_len = strlen(buffer);

  for (size_t i = 0; i < b_len - 1; i++) {
    buffer[i] = buffer[i + 1];
  }

  buffer[b_len - 1] = newChar;
  buffer[b_len] = '\0';
  return buffer;
}

void *startupdater(void *arg) {
  char *map = "..1234567890'¡#qwertyuiop.+.@asdfghjklñ._.zxcvbnm,.-_..\\";

  char *buffer = (char *)arg;
  int fd = open(SOCKET, O_RDONLY);
  struct input_event event;
  while (1) {
    ssize_t n = read(fd, &event, sizeof(event));
    if ((event.type == EV_KEY) && (event.value == 1) &&
        (event.code <= strlen(map + 1))) {
      char c = map[event.code];
      UpdateBuffer(buffer, c);
    }
  }

  return NULL;
}

void Window_Manager(char *buffer) {

  InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Keys");
  SetTargetFPS(FPS);
  int glyphCount = 200 - 32;
  int *codepoints = (int *)malloc(glyphCount * sizeof(int));

  for (int i = 0; i < glyphCount; i++) {
    codepoints[i] = 32 + i;
  }
  Font font = LoadFontEx(FONT, FONT_SIZE, codepoints, glyphCount);

  Vector2 text_position = {10, 10};
  int i = 1;

  pthread_t input_thread;
  pthread_create(&input_thread, NULL, startupdater, (void *)buffer);

  while (!WindowShouldClose()) {
    BeginDrawing();
    // startupdater() -- async method
    ClearBackground(BLACK);
    DrawTextEx(font, buffer, text_position, FONT_SIZE, 0, WHITE);
    EndDrawing();
  }
}
int main() {
  char *buffer = calloc(sizeof(char), BUFFER_SIZE);
  strcpy(buffer, "Hello World");
  Window_Manager(buffer);
  free(buffer);
}
