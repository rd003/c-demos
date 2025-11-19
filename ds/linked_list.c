#include <stdio.h>
#include <stdlib.h>

struct Node {
  int data;
  struct Node *next;
};

struct Node *create_node(int data) {
  struct Node *new_node = (struct Node *)malloc(sizeof(struct Node));
  if (new_node == NULL) {
    printf("Faliled to create a node.\n");
    exit(1);
  }
  new_node->data = data;
  new_node->next = NULL;
  return new_node;
}

void insert_at_beginning(struct Node **head, int data) {
  struct Node *new_node = create_node(data);
  new_node->next = *head;
  *head = new_node;
  printf("Inserted %d at beginning.\n", data);
}

void insert_at_end(struct Node **head, int data) {
  struct Node *new_node = create_node(data);
  if (*head == NULL) {
    *head = new_node;
    return;
  }

  struct Node *temp = *head;
  while (temp->next != NULL) {
    temp = temp->next;
  }

  temp->next = new_node;
  printf("Inserted %d at end.\n", data);
}

void insert_at_position(struct Node **head, int position, int data) {
  if (position == 0) {
    insert_at_beginning(head, data);
    return;
  }

  struct Node *new_node = create_node(data);
  struct Node *temp = *head;

  for (int i = 0; i < position - 1 && temp != NULL; i++) {
    temp = temp->next;
  }

  if (temp == NULL) {
    printf("Index out of bound.\n");
    free(new_node);
    return;
  }
  new_node->next = temp->next;
  temp->next = new_node;
  printf("Inserted %d at position %d.\n", data, position);
}

void delete_from_beginning(struct Node **head) {
  if (head == NULL) {
    printf("List is empty.\n");
  }

  struct Node *temp = *head;
  *head = (*head)->next;
  printf("Deleted %d from beginning.\n", temp->data);
  free(temp);
}

void delete_from_end(struct Node **head) {
  if (*head == NULL) {
    printf("List is empty.\n");
    return;
  }

  if ((*head)->next == NULL) {
    printf("Deleted %d from end\n", (*head)->data);
    free(*head);
    *head = NULL;
    return;
  }

  struct Node *temp = *head;
  while (temp->next->next != NULL) {
    temp = temp->next;
  }
  printf("Deleted %d from end.\n", temp->next->data);
  free(temp->next);
  temp->next = NULL;
}

int search_value(struct Node **head, int value) {
  struct Node *temp = *head;
  int counter = 0;

  while (temp != NULL) {
    if (temp->data == value) {
      printf("%d found at %d\n", value, counter);
      return counter;
    }
    temp = temp->next;
    counter++;
  }

  printf("value %d not found", value);
  return -1;
}

void display_list(struct Node **head) {
  struct Node *temp = *head;

  if (head == NULL) {
    printf("List is empty.\n");
    return;
  }

  printf("Link list: ");
  while (temp != NULL) {
    printf("%d ->", temp->data);
    temp = temp->next;
  }
  printf("NULL\n");
}

int get_length(struct Node **head) {
  int count = 0;
  struct Node *temp = *head;
  while (temp != NULL) {
    count++;
    temp = temp->next;
  }
  return count;
}

void free_list(struct Node **head) {
  struct Node *temp;
  while (*head != NULL) {
    temp = *head;
    *head = (*head)->next;
    free(temp);
  }
  printf("List memory freed.\n");
}

int main() {
  struct Node *head = NULL;

  insert_at_end(&head, 2);
  insert_at_end(&head, 3);
  insert_at_end(&head, 5);
  insert_at_end(&head, 6);
  insert_at_beginning(&head, 1);
  insert_at_position(&head, 3, 4);
  display_list(&head);
  
  int len = get_length(&head);
  printf("\n=== len: %d ===\n", len);

  search_value(&head,2);
  printf("\n");

  delete_from_end(&head);
  display_list(&head);
   
  printf("\n=== len: %d ===\n", get_length(&head));
 
  printf("\n");	
  
  delete_from_beginning(&head);
  display_list(&head);
  
  printf("\n=== len: %d ===\n", get_length(&head));

  free_list(&head);
  return 0;
}
