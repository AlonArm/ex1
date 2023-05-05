#include <stdio.h>      
#include <stdlib.h>     
#include <stdbool.h>
#include <string.h>
#include "HackerEnrollment.h"
//gcc -std=c99 -o HackEnrollment -lm -I/home/alon.armoni/hw/hw1/ex1 -Itool -Wall -pedantic-errors -Werror -DNDEBUG IsraeliQueue.c tool/HackerEnrollment.c tool/main.c

int main(int argc, char** argv){
    if(argc == 6 || argc == 7){
        FILE* target = fopen(argv[argc - 1], "w");
        if(target == NULL){
            printf("Could not find and open the target file!\ngiven file:%s\n", argv[argc]);
            return 0;
        }
        FILE* queues = fopen(argv[argc - 2], "r");
        if(queues == NULL){
            printf("Could not find and open the queues file!\n");
            fclose(target);
            return 0;
        }
        FILE* hackers = fopen(argv[argc - 3], "r");
        if(hackers == NULL){
            printf("Could not find and open the hackers file!\n");
            fclose(target);
            fclose(queues);
            return 0;
        }
        FILE* courses = fopen(argv[argc - 4], "r");
        if(courses == NULL){
            printf("Could not find and open the courses file!\n");
            fclose(target);
            fclose(queues);
            fclose(hackers);
            return 0;
        }
        FILE* students = fopen(argv[argc - 5], "r");
        if(students == NULL){
            printf("Could not find and open the students file!\n");
            fclose(target);
            fclose(queues);
            fclose(hackers);
            fclose(courses);
            return 0;
        }
        bool ignoreBigLetters = argc == 7 && !strcmp(argv[1], "-i");
        //ignoreBigLetters needs to be added to the enrollment system
        EnrollmentSystem system = createEnrollment(students, courses, hackers);
        printf("main1\n");
        if(system != NULL){
            if(ignoreBigLetters){
                printf("main2\n");
                changeCapitalLetters(system);
            }
            printf("main3\n");
            EnrollmentSystem queuedSystem = readEnrollment(system, queues);
            printf("main4\n");
            if(queuedSystem != NULL){
                hackEnrollment(queuedSystem, target);
                printf("Enrollment system was changed successfully");
                destroyEnrollmentSystem(queuedSystem);
            }
            else{
                printf("could not produce the queued system!\n");
            }
            destroyEnrollmentSystem(system);
        }
        else{
            printf("could not produce the enrollment system!\n");
        }
        fclose(target);
        fclose(queues);
        fclose(hackers);
        fclose(courses);
        fclose(students);
    }
    else{
        printf("Wrong Syntax! correct syntax is:\n%s\n",
        "HackEnrollment <flags> <students> <courses> <hackers> <queues> <target>");
    }
    return 0;
}