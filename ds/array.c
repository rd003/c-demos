#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  int *data;
  size_t size;
  size_t capacity;
} Array;

Array *array_create(size_t initial_capacity) {
  Array *arr = (Array *)malloc(sizeof(Array));
  if (!arr)
    return NULL;

  arr->data = (int *)malloc(initial_capacity * sizeof(int));
  if (!arr->data) {
    free(arr);
    return NULL;
  }

  arr->size = 0;
  arr->capacity = initial_capacity;
  return arr;
}

void array_destroy(Array *arr) {
  if (arr) {
    free(arr->data);
    free(arr);
  }
}

bool array_resize(Array *arr, size_t new_capacity) {
  int *new_data = (int *)realloc(arr->data, new_capacity * sizeof(int));
  if (!new_data) {
    return false;
  }

  arr->data = new_data;
  arr->capacity = new_capacity;
  return true;
}

bool array_push(Array *arr, int value) {
  if (arr->size >= arr->capacity) {
    if (!array_resize(arr, arr->capacity * 2)) {
      return false;
    }
  }

  arr->data[arr->size++] = value;
  return true;
}

bool array_pop(Array *arr, int *value) {
  if (arr->size == 0)
    return false;

  if (value) {
    *value = arr->data[arr->size - 1];
  }
  arr->size--;

  // if size is 1/4th of the capacity
  if (arr->size <= arr->capacity / 4) {
    array_resize(arr, arr->capacity / 2);
  }

  return true;
}

bool array_insert(Array *arr, size_t index, int value) {
  if (index >= arr->size) {
    return false;
  }
  if (arr->size >= arr->capacity) {
    if (!array_resize(arr, arr->capacity * 2))
      return false;
  }

  for (size_t i = arr->size; i > index; i--) {
    arr->data[i] = arr->data[i - 1];
  }

  arr->data[index] = value;
  arr->size++;

  return true;
}

bool array_remove(Array *arr, size_t index, int *value) {
  if (index >= arr->size) {
    return false;
  }

  if (value) {
    *value = arr->data[index];
  }

  // shift elements to left
  for (size_t i = index; i < arr->size; i++) {
    arr->data[i] = arr->data[i + 1];
  }
  arr->size--;

  // if size is 1/4th of the capacity
  if (arr->size <= arr->capacity / 4) {
    array_resize(arr, arr->capacity / 2);
  }

  return true;
}

bool array_get(Array *arr, size_t index, int *value) {
  if (index >= arr->size) {
    return false;
  }

  *value = arr->data[index];

  return true;
}

bool array_set(Array *arr, size_t index, int value) {
  if (index >= arr->size)
    return false;
  arr->data[index] = value;
  return true;
}

size_t array_size(Array *arr) { return arr->size; }

bool array_is_empty(Array *arr) { return arr->size == 0; }

void array_clear(Array *arr) { arr->size = 0; }

void array_print(Array *arr) {
  printf("[");
  for (size_t i = 0; i < arr->size; i++) {
    printf("%d", arr->data[i]);
    if (i < arr->size - 1) {
      printf(", ");
    }
  }
  printf("]\n");
}

int main() {
  Array *arr = array_create(2);
  if (!arr) {
    printf("Failed to create an array!\n");
    return 1;
  }

  printf("Pushing elements...\n");
  array_push(arr, 4);
  array_push(arr, 6);
  array_push(arr, 7);
  array_push(arr, 106);
  array_push(arr, 434);
  array_push(arr, 90834);
  array_push(arr, 56);
  array_push(arr, 45);
  array_push(arr, 15);

  array_print(arr);

  int popped;
  array_pop(arr, &popped);
  printf("\nPopped %d\n", popped);

  array_print(arr);

  array_insert(arr, 2, 11);
  printf("\n11 inserted at index:2\n");
  array_print(arr);

  int deleted;
  array_remove(arr, 3, &deleted);
  printf("\nDeleted %d\n", deleted);
  array_print(arr);

  array_destroy(arr);
  return 0;
}
