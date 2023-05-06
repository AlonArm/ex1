#ifndef PROVIDED_ISRAELIQUEUE_H
 #include "../IsraeliQueue.h" //need to change to the place where this is actually at when finsihed
#endif

#include <stdio.h>


#ifndef HACKER_ENROLLMENT_H_
#define HACKER_ENROLLMENT_H_

#define ID_LENGTH 10
#define FRIENDSHIP_TH 20 //the friendship threshold as defined in the question
#define ENEMY_TH -10 //the enemy threshold as defined in the question

typedef struct EnrollmentSystem_t* EnrollmentSystem;

typedef struct Node{
    void* data;
    struct Node* next;
}Node;
typedef struct Student{
    char* ID;
    char* totalCredits;
    char* GPA;
    char* name;
    char* surName;
    char* city;
    char* department;
    struct Hacker* hackerInfo;
}Student;

typedef struct Course{
    char* courseNum;
    int size;
    IsraeliQueue queue;
}Course;

typedef struct Hacker{
    struct Node* desiredCourses;
    struct Node* friends;
    struct Node* rivals;
}Hacker;

typedef struct EnrollmentSystem_t
{
    struct Node* coursesList;
    struct Node* studentsList;
    struct Node* hackers;
}EnrollmentSystem_t;
void printSystem(EnrollmentSystem);
void destroySystem(EnrollmentSystem);
EnrollmentSystem createEnrollment(FILE*, FILE*, FILE*);
EnrollmentSystem readEnrollment(EnrollmentSystem, FILE*);
void hackEnrollment(EnrollmentSystem, FILE*);
#endif
