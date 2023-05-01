#include <stdio.h>      
#include <stdlib.h>     
#include <stdbool.h>
#include <string.h>
#include "HackerEnrollment.h"

int main(int argc, char** argv){
    if(argc != 6 && argc != 7){
        printf("Wrong Syntax! correct syntax is:\n%s\n",
         "HackEnrollment <flags> <students> <courses> <hackers> <queues> <target>");
        exit;
    }
    FILE* target = fopen(argv[argc], "w");
    if(target == NULL){
        printf("Could not find and open the target file!\n");
        exit;
    }
    FILE* queues = fopen(argv[argc - 1], "r");
    if(queues == NULL){
        printf("Could not find and open the queues file!\n");
        fclose(target);
        exit;
    }
    FILE* hackers = fopen(argv[argc - 2], "r");
    if(hackers == NULL){
        printf("Could not find and open the hackers file!\n");
        fclose(target);
        fclose(queues);
        exit;
    }
    FILE* courses = fopen(argv[argc - 3], "r");
    if(courses == NULL){
        printf("Could not find and open the courses file!\n");
        fclose(target);
        fclose(queues);
        fclose(hackers);
        exit;
    }
    FILE* students = fopen(argv[argc - 4], "r");
    if(students == NULL){
        printf("Could not find and open the students file!\n");
        fclose(target);
        fclose(queues);
        fclose(hackers);
        fclose(courses);
        exit;
    }
    bool ignoreBigLetters = argc == 7 && !strcmp(argv[1], "-i");
    EnrollmentSystem* system = createEnrollment(students, courses, hackers);
    if(system != NULL){
        EnrollmentSystem* queuedSystem = readEnrollment(system, queues);
        if(queuedSystem != NULL){
            hackEnrollment(queuedSystem, target);
            printf("Enrollment system was changed successfully");
        }
        //destroy queuedSystem
    }
    //destroy system
    fclose(target);
    fclose(queues);
    fclose(hackers);
    fclose(courses);
    fclose(students);
}