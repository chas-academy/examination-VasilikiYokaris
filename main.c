#include <stdio.h>
#include <string.h>
#include <ctype.h>


#define NUM_STUDENTS 5 // macros used to define constants.
#define NUM_TESTS 13
#define NAME_MAX 11  // max 10 chars + null terminator
#define LINE_MAX 200

typedef struct {
    char name[NAME_MAX]; // define for each student name string, score, and float average
    int scores[NUM_TESTS];
    float average;
} Student;


void capitalize(char* name) { // Capitalize first letter, lowercase rest
    name[0] = toupper(name[0]);
    for (int i = 1; name[i]; i++) {
        name[i] = tolower(name[i]);
    }
}

// Check if name valid: only letters a-z or A-Z, max 10 chars
int is_valid_name(const char* name) {
    int len = strlen(name);
    if (len == 0 || len > 10) return 0;
    for (int i = 0; i < len; i++) {
        if (!((name[i] >= 'a' && name[i] <= 'z') || (name[i] >= 'A' && name[i] <= 'Z'))) {
            return 0;
        }
    }
    return 1;
}

// Parse one line of input for one student, validate, and fill student struct
int parse_student_line(const char* line, Student* student) {
    char name[NAME_MAX];
    int scores[NUM_TESTS];
    int count;

    // Use sscanf with %10s to avoid overflow on name
    // Then read 13 integers
    count = sscanf(line, "%10s %d %d %d %d %d %d %d %d %d %d %d %d %d", // sscanf tries to read name and 13 scores from the input line
                   name,
                   &scores[0], &scores[1], &scores[2], &scores[3], &scores[4],
                   &scores[5], &scores[6], &scores[7], &scores[8], &scores[9],
                   &scores[10], &scores[11], &scores[12]);

    if (count != 14) {
        // Didn't read name + 13 scores
        return 0;
    }

    if (!is_valid_name(name)) {
        return 0;
    }

    // Validate scores
    for (int i = 0; i < NUM_TESTS; i++) {
        if (scores[i] < 0 || scores[i] > 10) {
            return 0;
        }
    }

    // Copy valid data into student
    strcpy(student->name, name);
    for (int i = 0; i < NUM_TESTS; i++) {
        student->scores[i] = scores[i];
    }
    return 1;
}

int main() {
    Student students[NUM_STUDENTS];
    float total_average = 0.0f;
    char line[LINE_MAX];

    for (int i = 0; i < NUM_STUDENTS; i++) {
        printf("Enter data for student %d (name and 13 scores separated by spaces):\n", i + 1);

        while (1) {
            if (!fgets(line, sizeof(line), stdin)) {
                printf("Input error. Try again.\n");
                continue;
            }

            // Remove trailing newline if any
            size_t len = strlen(line);
            if (len > 0 && line[len - 1] == '\n') {
                line[len - 1] = '\0';
            }

            if (parse_student_line(line, &students[i])) { // Parses one input line 
                // Calculate average
                int sum = 0;
                for (int j = 0; j < NUM_TESTS; j++) {
                    sum += students[i].scores[j];
                }
                students[i].average = sum / (float)NUM_TESTS;
                total_average += students[i].average;
                break;
            } else {
                printf("Invalid input format or values. Please enter again:\n");
            }
        }
    }

    // Find student with highest average
    int top_index = 0;
    for (int i = 1; i < NUM_STUDENTS; i++) {
        if (students[i].average > students[top_index].average) {
            top_index = i;
        }
    }

    char temp_name[NAME_MAX];
    strcpy(temp_name, students[top_index].name);
    capitalize(temp_name);
    printf("\n%s\n", temp_name);

    float class_avg = total_average / NUM_STUDENTS;

    // Print names below class average
    for (int i = 0; i < NUM_STUDENTS; i++) {
        if (students[i].average < class_avg) {
            strcpy(temp_name, students[i].name);
            capitalize(temp_name);
            printf("%s\n", temp_name);
        }
    }

    return 0;
}
