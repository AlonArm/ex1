#ifndef PROVIDED_ISRAELIQUEUE_H
 #include "IsraeliQueue.h" //need to change to the place where this is actually at when finsihed
#endif

#include <stdio.h>


#ifndef HACKER_ENROLLMENT_H_
#define HACKER_ENROLLMENT_H_

#define ID_LENGTH 10
#define FRIENDSHIP_TH 20 //the friendship threshold as defined in the question
#define ENEMY_TH -10 //the enemy threshold as defined in the question

typedef struct EnrollmentSystem_t* EnrollmentSystem;

typedef struct Node{ //helper struct for mananging the data in lines and not in arrays
    void* data;
    struct Node* next;
}Node;

//this is the student struct made for copying the student data from the file to this struct
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

//this is the course struct made for copying the data from the courses files into the course struct
typedef struct Course{
    char* courseNum;
    int size;
    IsraeliQueue queue;
}Course;

//this is the hacker struct made for copying the hacke data from the hacker file into the hacker struct
typedef struct Hacker{
    struct Node* desiredCourses;
    struct Node* friends;
    struct Node* rivals;
}Hacker;

//this is the enrollment system struct made for holding all the data inside all the given files
typedef struct EnrollmentSystem_t
{
    struct Node* coursesList;
    struct Node* studentsList;
    struct Node* hackers;
}EnrollmentSystem_t;


/**
 * OVERVIEW ON THE printSystem BELOW-
 * prints all the data concerning an enrollment system struct
 * 
 * PARAMETERS PASSED-
 * 1.EnrollmentSystem sys
*/
void printSystem(EnrollmentSystem);

/**
 * OVERVIEW ON THE destroySystem BELOW-
 * deletes all the data inside a system
 * 
 * PARAMETERS PASSED-
 * 1.EnrollmentSystem sys
*/
void destroySystem(EnrollmentSystem);

/**
 * OVERVIEW ON THE createEnrollment BELOW-
 * Creates a new EnrollmentSystem object with the provided students file functions, courses file, hacker file
 * returns a struct containing a list of students pointers, a list of hackers pointers,
 * and a list of the courses pointer in case of failure return a pointer to an enrollment system that all its paramters point to NULL.
 * 
 * PARAMETERS PASSED FORMAT-
 * 1.FILE* students: <Student ID> <Total Credits> <GPA> <Name> <Surname> <City> <Department>\n
 * 2.FILE* courses: <Course Number> <Size>\n
 * 3.FILE* hackers: <Student ID> \n<Course Numbers>*\n //Desired courses<Student ID>*\n //Friends<Student ID>*\n //Rivals
 */
EnrollmentSystem createEnrollment(FILE*, FILE*, FILE*);

/**
 * OVERVIEW ON readEnrollment BELOW-
 * enters into system the courses queues from a provided file of queues that cosists of the students
 * ids, and the course number, returns a pointer to the emrollmentsystem that has changed.
 * 
 * PARAMETERS PASSED FORMAT-
 * 1.EnrollmentSystem sys: pointer to the system itself
 * 2.FILE* queues: <Course Number> <Student IDs>*
*/
EnrollmentSystem readEnrollment(EnrollmentSystem, FILE*);

/**
 * OVERVIEW ON hackEnrollment BELOW-
 * iterates through every hacker insisde the enrollment system and 
 * enters him to the place in the queue that he can enter based on the freindship functions above.
 * the iteration is in order of the hacker file. then it 
 * prints out for every hacker that didnt get in to two of the courses that he wanted(if he wanted only one then it is fine)-
 *  "Cannot satisfy constraints for <Student ID>+" into the out file
 * see that hackers maybe enemies of other hackers, or even friends
 * 
 * PARAMETERS PASSED FORMAT-
 * 1. EnrollmentSystem sys: pointer to the system itself
 * 2. FILE* out: file that consists of "Cannot satisfy constraints for <Student ID>+" 
 */
void hackEnrollment(EnrollmentSystem, FILE*);
#endif
