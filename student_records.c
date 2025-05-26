//*****************************************************************
//  student_records.c
//
//   
//  A simple, menu‑driven program to manage student records in a
//  binary file. Allows the user to manage records through first
//  creating the file and adding, deleting, updating, displaying
//  and searching for student records.
//****************************************************************
#include <stdio.h>
#include <string.h>
#include <unistd.h>

// Define structure for each student record
typedef struct Student {
    char   student_id[50];
    char   student_name[50];
    char   email_id[50];
    char   course_id[50];
    float  grade;
} Student;
 
// Function prototypes
void print_menu(void);
void create_binary_file(char *filename);
void add_student(char *filename);
void display_students(char *filename);
void search_student(char *filename);
void update_student(char *filename);
void delete_student(char *filename);
int  file_exists(const char *name);

// Presents a menu, reads user choices, and dispatches to appropriate
// handler. Operations 2‑6 are prevented until binary file is created.
int main(void) {
    int  choice;
    char filename[100] = "";  // empty until user creates a file
 
    do {
        print_menu();
        scanf("%d", &choice);
 
        // Require an existing file to perform operations on records
        if (choice >= 2 && choice <= 6 && !file_exists(filename)) {
            printf("No binary file found.  Please create one first (option 1).\n");
            sleep(1);
            while (getchar() != '\n');
            continue;
        }
 
        switch (choice) {
            case 1:
                create_binary_file(filename);
                break;
            case 2:
                printf("\nAdd Student Record\n");
                add_student(filename);
                break;
            case 3:
                printf("\nDisplay All Records\n");
                display_students(filename);
                break;
            case 4:
                printf("\nSearch for a Record\n");
                search_student(filename);
                break;
            case 5:
                printf("\nUpdate a Record\n");
                update_student(filename);
                break;
            case 6:
                printf("\nDelete a Record\n");
                delete_student(filename);
                break;
            case 7:
                printf("\nExiting...\n");
                sleep(1);
                printf("Your session has ended.\n");
                break;
            default:
                printf("Invalid choice: please enter 1-7.\n");
                sleep(1);
        }
 
        printf("\n");
        while (getchar() != '\n');
    } while (choice != 7);
 
    return 0;
}
 

// Displays the main menu to the user.
void print_menu(void) {
    printf("\nStudent Records Management System\n");
    printf("=================================\n\n");
    printf("1. Create Binary File\n");
    printf("2. Add Student Record\n");
    printf("3. Display All Records\n");
    printf("4. Search for a Record\n");
    printf("5. Update a Record\n");
    printf("6. Delete a Record\n");
    printf("7. Exit\n");
    printf("\nEnter your choice: ");
}

// Check if the binary file exists.
int file_exists(const char *name) {
    return (name[0] != '\0') && (access(name, F_OK) == 0);
}
 
// Function for creating a new binary file
void create_binary_file(char *filename) {
    FILE *fpointer;

    // Get file name from user
    printf("\nWhat would you like to name the file? (w/ or w/o .bin) ");
    scanf("%s", filename);

    // Append ".bin" if not present
    if (strstr(filename, ".bin") == NULL) {
        strcat(filename, ".bin");
    }

    // Open the file for writing
    fpointer = fopen(filename, "wb");

    // Check if the file was created successfully
    if (fpointer == NULL) {
        printf("Error: Could not create file.\n");
        return;
    }

    fclose(fpointer);
    printf("Binary file '%s' has been successfully created.\n", filename);
}
 

// Add a new student record to the binary file.
void add_student(char *filename) {
    Student new_student;

    while (getchar() != '\n'); // clear any stray input
    
    printf("\nEnter student ID: ");
    fgets(new_student.student_id, 50, stdin);
    new_student.student_id[strcspn(new_student.student_id, "\n")] = '\0';

    // Check if the ID already exists
    FILE *fpointer = fopen(filename, "rb");
    if (fpointer) {
        Student existing;
        while (fread(&existing, sizeof(Student), 1, fpointer) == 1) {
            if (strcmp(existing.student_id, new_student.student_id) == 0) {
                printf("\nError: Student ID %s already exists!\n", new_student.student_id);
                printf("\nPress Enter to continue...");
                //while (getchar() != '\n');
                fclose(fpointer);
                return;
            }
        }
        fclose(fpointer);
    }

    // Add new student record
    fpointer = fopen(filename, "ab");
    if (!fpointer) {
        printf("Error: Could not open file for writing.\n");
        return;
    }
    
    printf("Enter student name: ");
    fgets(new_student.student_name, 50, stdin);
    new_student.student_name[strcspn(new_student.student_name, "\n")] = '\0';
    
    printf("Enter student email: ");
    fgets(new_student.email_id, 50, stdin);
    new_student.email_id[strcspn(new_student.email_id, "\n")] = '\0';
    
    printf("Enter student course: ");
    fgets(new_student.course_id, 50, stdin);
    new_student.course_id[strcspn(new_student.course_id, "\n")] = '\0';
    
    printf("Enter student grade(gpa): ");
    scanf("%f", &new_student.grade);
    
    fwrite(&new_student, sizeof(new_student), 1, fpointer);
    fclose(fpointer);
    printf("\nStudent record added successfully.\n");
}


// Display all student records from file.
void display_students(char *filename) {
    FILE *fpointer = fopen(filename, "rb");
    
    Student record;
    int count = 0;
    printf("\nStudent Records:\n");
    printf("=================\n");
    while (fread(&record, sizeof(record), 1, fpointer)) {
        printf("ID: %s\n", record.student_id);
        printf("Name: %s\n", record.student_name);
        printf("Email: %s\n", record.email_id);
        printf("Course: %s\n", record.course_id);
        printf("Grade: %.2f\n", record.grade);
        printf("-----------------------------\n");
        count++;
    }

    // Notify user that no records were found
    if (count == 0) printf("No records found.\n");
    fclose(fpointer);
}
 

// Search and display specific student by ID.
void search_student(char *filename) {
    FILE *fpointer = fopen(filename, "rb");
    
    char search_id[50];
    printf("Enter the ID of the student to search: "); 
    scanf("%49s", search_id);
    while (getchar() != '\n');

    Student record;
    int found = 0;

    while (fread(&record, sizeof(record), 1, fpointer)) {
        if (strcmp(record.student_id, search_id) == 0) {
            printf("ID: %s\n", record.student_id);
            printf("Name: %s\n", record.student_name);
            printf("Email: %s\n", record.email_id);
            printf("Course: %s\n", record.course_id);
            printf("Grade: %.2f\n", record.grade);
            found = 1;
            break;
        }
    }
    // Check if student record was not found.
    if (!found) {
        printf("No student record with ID %s exists. Please check and try again.\n", search_id);
    }
    fclose(fpointer);
 }
 

// Update an existing student record.
void update_student(char *filename) {
    FILE *fpointer = fopen(filename, "r+b");
    
    char search_id[50];
    printf("Enter the ID of the student to update: ");
    scanf("%s", search_id);
    while (getchar() != '\n');

    Student record;
    int found = 0;

    // Search for the record with the given ID
    while (fread(&record, sizeof(Student), 1, fpointer)) {
        if (strcmp(record.student_id, search_id) == 0) {
            found = 1;
            
            printf("Enter new name: ");
            fgets(record.student_name, 50, stdin);
            record.student_name[strcspn(record.student_name, "\n")] = '\0';
            
            printf("Enter new email: ");
            fgets(record.email_id, 50, stdin);
            record.email_id[strcspn(record.email_id, "\n")] = '\0';
            
            printf("Enter new course: ");
            fgets(record.course_id, 50, stdin);
            record.course_id[strcspn(record.course_id, "\n")] = '\0';
            
            printf("Enter new grade: ");
            scanf("%f", &record.grade);

            // Move the file pointer back to the position of the record
            fseek(fpointer, -sizeof(Student), SEEK_CUR);
            if (fwrite(&record, sizeof(Student), 1, fpointer) != 1) {
                printf("Error updating student record.\n");
            } else {
                printf("Student record updated successfully.\n");
            }
            break;
        }
    }

    if (!found) {
        printf("Record with ID %s not found.\n", search_id);
    }
    fclose(fpointer);
 }
 

// Delete a student record by rewriting file.
void delete_student(char *filename) {
    FILE *fpointer = fopen(filename, "r+b");
    if (fpointer == NULL) {
        printf("Error: Could not open file.\n");
        return;
    }
    char search_id[50];
    printf("\nEnter the ID of the student to delete: ");
    scanf("%s", search_id);
    while (getchar() != '\n');

    Student record;
    int found = 0;

    // Create a temp file to store records
    FILE *temp_fpointer = fopen("temp.bin", "wb");
    if (temp_fpointer == NULL) {
        printf("\nError: Could not create temporary file.\n");
        fclose(fpointer);
        return;
    }

    // Copy records to temp, skip the one to delete
    while (fread(&record, sizeof(Student), 1, fpointer)) {
        if (strcmp(record.student_id, search_id) == 0) {
            found = 1;
            continue;
        }
        fwrite(&record, sizeof(Student), 1, temp_fpointer);
    }    

    fclose(fpointer);
    fclose(temp_fpointer);

    // Replace the original file with the temp file
    remove(filename);
    rename("temp.bin", filename);

    if (found) {
        printf("\nStudent record with ID %s deleted successfully.\n", search_id);
        printf("\nPress Enter to continue...");
    } else {
        printf("\nRecord with ID %s not found.\n", search_id);
        printf("\nPress Enter to continue...");
    }
}
