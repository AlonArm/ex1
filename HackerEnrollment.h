#ifndef PROVIDED_ISRAELIQUEUE_H
 #include "IsraeliQueue.h" //need to change to the place where this is actually at when finsihed
#endif



#ifndef HACKER_ENROLLMENT_H_
#define HACKER_ENROLLMENT_H_

#define ID_LENGTH 9
#define FRIENDSHIP_TH 20 //the friendship threshold as defined in the question
#define ENEMY_TH -10 //the enemy threshold as defined in the question

typedef enum { HACKERENROLLMENT_SUCCESS, HACKERENROLLMENT_ALLOC_FAILED, HACKERENROLLMENT_BAD_PARAM, HACKERENROLLMENT_ERROR} HackerEnrollmentError;
/**Error clarification:
 * HACKERENROLLMENT_SUCCESS: Indicates the function has completed its task successfully with no errors.
 * HACKERENROLLMENT_ALLOC_FAILED: Indicates memory allocation failed during the execution of the function.
 * HACKERENROLLMENT_BAD_PARAM: Indicates an illegal parameter was passed.
 * HACKERENROLLMENT_ERROR: Indicates any error beyond the above.
 * */


typedef struct Student* studentPtr; //pointer to a student
typedef struct Hacker* hackerPtr; //pointer to a hacker
typedef struct Course* coursePtr; //pointer to the courses
typedef struct EnrollmentSystem_t* EnrollmentSystem; //pointer to the enrollment system

//this struct holds the neccesary data on all the students, see that a student is also a hacker

typedef struct 
{

    char id[ID_LENGTH]; //must be 9 whole numbers - meaning need to check when opening file that this is the case
    int totalCredits; //must be a whole number bigger than 0
    double gpa; //num 0-100
    char* name; 
    char* surName; 
    char* city; 
    char* department;

}Student;

//this struct holds the neccesary data on the hackers
typedef struct 
{

    studentPtr student; //hacker is also a student, no need for id because we will find him in the student array
    char** desiredCourses; //expectation to be only numbers
    char *friendsId[ID_LENGTH];
    char *enemiesId[ID_LENGTH];

}Hacker;

//this struct holds the data on all the courses queues
typedef struct 
{
    int courseSize;
    char* courseNumber;
    IsraeliQueue queue;
}
Course;

//this is the struct that holds all the data on the enrollment system
typedef struct
{
    studentPtr* students; //array of students
    hackerPtr* hackers;  //array of hacker pointers
    coursePtr* course; //array of courses

}EnrollmentSystem_t;

/**
*OVERVIEW ON THE FUNCTIONS BELOW- 
*this functions return the value of freindships based on different parameters and need to be inside funclist
*1.friendshipValueByHackerFile - returns 20 if a hacker is friends with a student based on the file
*it returns -20 if they are enemys and 0 if they're neither
*2. friendshipValueByASCII returns the absolute value of the diffrence between both names of the
*hacker and the person
*3. the third returns the absolute value of both ids 
*/
int friendshipValueByHackerFile(hackerPtr,studentPtr); 
int friendshipValueByASCII(char* hackerName,char* studentName); 
int friendshipValueById(char hackerId[ID_LENGTH],char studentId[ID_LENGTH]); 

/**
 * OVERVIEW ON THE createEnrollment BELOW-
 * Creates a new EnrollmentSystem object with the provided students file functions, courses file, hacker file
 * returns a struct containing an array of students pointers, an array of hackers pointers,
 * and an array of the courses pointer and doesnt update the courses queue
 * in case of failure return a pointer to an enrollment system that all its paramters point to NULL.
 * 
 * PARAMETERS PASSED FORMAT-
 * 1.FILE* students: <Student ID> <Total Credits> <GPA> <Name> <Surname> <City> <Department>\n
 * 2.FILE* courses: <Course Number> <Size>\n
 * 3.FILE* hackers: <Student ID> \n<Course Numbers>*\n //Desired courses<Student ID>*\n //Friends<Student ID>*\n //Rivals
 */

EnrollmentSystem createEnrollment(FILE* students, FILE* courses, FILE* hackers);

/**
 * OVERVIEW ON readEnrollment BELOW-
 * enters into system the courses queues from a provided file of queues that cosists of the students
 * ids, and the course number, returns a pointer to the emrollmentsystem that has changed.
 * 
 * PARAMETERS PASSED FORMAT-
 * 1.EnrollmentSystem sys: pointer to the system itself
 * 2.FILE* queues: <Course Number> <Student IDs>*
*/
EnrollmentSystem readEnrollment(EnrollmentSystem sys, FILE* queues);

/**
 * OVERVIEW ON hackEnrollment BELOW-
 * iterates through every hacker insisde the enrollment system and 
 * enters him to the place in the queue that he can enter based on the freindship functions above.
 * the iteration is in order of the hacker file. then it 
 * prints out for every hacker that didnt get in at least two of the courses that he wanted-
 *  "Cannot satisfy constraints for <Student ID>+" into the out file
 * see that hackers maybe enemies of other hackers, or even friends
 * 
 * PARAMETERS PASSED FORMAT-
 * 1. EnrollmentSystem sys: pointer to the system itself
 * 2. FILE* out: file that consists of "Cannot satisfy constraints for <Student ID>+" 
 */
void hackEnrollment(EnrollmentSystem sys, FILE* out);
#endif
