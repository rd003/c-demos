#include <stdio.h>
#include <string.h>

#define MAX_PEOPLE 100
#define FILE_NAME "people.txt"

struct Person {
  int id;
  char first_name[30];
  char last_name[30];
};

struct PersonWithIndex {
  struct Person person;
  int index;
};

struct Person people[MAX_PEOPLE];
int count = 0;

void create();
void load_from_file();
void save_to_file();
void menu();
void read_all();
void read_one();
void update();
void delete();

struct PersonWithIndex get_person_by_id(int id);

int main() {
  load_from_file();

  int choice;
  do {
    menu();

    printf("Ener the choice: ");
    scanf("%d", &choice);
    getchar();

    switch (choice) {
    case 1:
      create();
      break;
    case 2:
      read_all();
      break;
    case 3:
      read_one();
      break;
    case 4:
      update();
      break;
    case 5:
      delete();
      break;
    case 6:
      printf("Existing program...\nBye!\n\n");
      break;
    default:
      printf("Invalid choice. Please try again");
    }
  } while (choice != 6);

  return 0;
}

void menu() {
  printf("===Person mgt system==\n");
  printf("1. Create\n");
  printf("2. ReadAll\n");
  printf("3. Read one\n");
  printf("4. Update\n");
  printf("5. Delete\n");
  printf("6. Exit\n");
}

void load_from_file() {
  FILE *file = fopen(FILE_NAME, "r");
  if (file == NULL) {
    printf("File does not exist");
    return;
  }

  count = 0;
  while (fscanf(file, "%d,%29[^,],%29[^\n]", &people[count].id,
                people[count].first_name, people[count].last_name) == 3) {
    count++;
    if (count >= MAX_PEOPLE) {
      printf("Maximum capacity reached!\n");
      break;
    };
  }

  fclose(file);
  printf("Loaded %d records, from a file.\n", count);
}

void save_to_file() {
  FILE *file = fopen(FILE_NAME, "w");
  if (file == NULL) {
    printf("Error: Unable to save to file!\n");
    return;
  }

  for (int i = 0; i < count; i++) {
    fprintf(file, "%d,%s,%s\n", people[i].id, people[i].first_name,
            people[i].last_name);
  }

  fclose(file);
  printf("Data saved successfully\n");
}

void create() {
  if (count > MAX_PEOPLE) {
    printf("Can not add more records!");
    return;
  }

  printf("id = ");
  scanf("%d", &people[count].id);

  printf("first name = ");
  scanf("%29s", people[count].first_name);

  printf("last name = ");
  scanf("%29s", people[count].last_name);

  getchar();

  count++;
  save_to_file();
  printf("Person added successfully!\n");
}
void read_all() {
  if (count == 0) {
    printf("No record to display.\n");
    return;
  }

  printf("\n===All People===\n");
  printf("%-5s %-20s %-20s", "Id", "First Name", "Last Name");
  printf("\n----------------------------------------\n");

  for (int i = 0; i < count; i++) {
    printf("%-5d %-20s %-20s\n", people[i].id, people[i].first_name,
           people[i].last_name);
  }
};
void read_one() {
  if (count == 0) {
    printf("No record to search...\n");
    return;
  }
  printf("\n==== Search Record ====\n");
  printf("Enter first name to search: ");

  char fname[30];
  scanf("%29s", fname);

  int found = 0;
  for (int i = 0; i < count; i++) {
    if (strcmp(fname, people[i].first_name) == 0) {
      found = 1;
      printf("\nRecord found.\n");
      printf("%-5s %-20s %-20s\n", "Id", "First Name", "Last Name");
      printf("%-5d %-20s %-20s\n", people[i].id, people[i].first_name,
             people[i].last_name);
    }
  }

  if (found == 0) {
    printf("No record available for '%s'\n", fname);
  }
};

void update() {
  if (count == 0) {
    printf("File is empty. No record to delete.\n");
    return;
  }

  int id;
  printf("Enter an id to update: ");
  scanf("%d", &id);

  struct PersonWithIndex p = get_person_by_id(id);

  if (p.index == -1) {
    printf("Person not found");
    return;
  }

  printf("\n === Person to update ===\n");

  printf("id: %d, first_name: %s, last_name: %s\n\n", p.person.id,
         p.person.first_name, p.person.last_name);

  printf("Enter first_name: ");
  scanf("%29s", people[p.index].first_name);
  printf("Enter last_name: ");
  scanf("%29s", people[p.index].last_name);

  getchar();

  save_to_file();

  printf("Person is updated/n");
};

struct PersonWithIndex get_person_by_id(int id) {
  struct PersonWithIndex person_data;
  person_data.index = -1;

  if (count == 0) {
    printf("File is empty");
    return person_data;
  }

  for (int i = 0; i < count; i++) {
    if (people[i].id == id) {
      person_data.person = people[i];
      person_data.index = i;
      return person_data;
    }
  }

  return person_data;
}

void delete() {
  if (count == 0) {
    printf("There is nothing to delete.\n");
    return;
  }

  int person_id;
  printf("Enter an id to delete: ");
  scanf("%d", &person_id);

  struct PersonWithIndex person_data = get_person_by_id(person_id);

  if (person_data.index == -1) {
    printf("No person found..\n");
    return;
  }

  struct Person person = person_data.person;

  printf("\nDeleting person: first_name: %s, last_name: %s\n",
         person.first_name, person.last_name);

  for (int i = person_data.index; i < count; i++) {
    people[i] = people[i + 1];
  }

  count--;

  printf("\nPerson is deleted.\n");

  save_to_file();
};
