#ifndef PROVIDED_ISRAELIQUEUE_H
 #include "../IsraeliQueue.h" //need to change to the place where this is actually at when finsihed
#endif

#include <stdio.h>


#ifndef HACKER_ENROLLMENT_H_
#define HACKER_ENROLLMENT_H_

#define ID_LENGTH 10
#define FRIENDSHIP_TH 20 //the friendship threshold as defined in the question
#define ENEMY_TH -10 //the enemy threshold as defined in the question

typedef struct Student* studentPtr; //pointer to a student
typedef struct Hacker* hackerPtr; //pointer to a hacker
typedef struct Course* coursePtr; //pointer to the courses
typedef struct EnrollmentSystem_t* EnrollmentSystem; //pointer to the enrollment system
typedef struct Node* nodePtr;
typedef struct List* listPtr;

typedef enum { HackerEnrollment_SUCCESS,HackerEnrollment_ALLOC_FAILED, HackerEnrollment_BAD_PARAM, HackerEnrollment_ERROR } HackerEnrollmentError;

/**Error clarification:
 * HackerEnrollmentSUCCESS: Indicates the function has completed its task successfully with no errors.
 * HackerEnrollment_ALLOC_FAILED: Indicates memory allocation failed during the execution of the function.
 * HackerEnrollment_BAD_PARAM: Indicates an illegal parameter was passed.
 * HackerEnrollment_ERROR: Indicates any error beyond the above.
 * */

//this struct holds the neccesary data on all the students, see that a student is also a hacker
typedef struct Student
{

   char id[ID_LENGTH]; //must be 9 whole numbers (so id_length=10)
    int totalCredits; //must be a whole number bigger than 0
    double gpa; //num 0-100
    char* name; 
    char* surName; 
    char* city; 
    char* department;
    hackerPtr hacker;

}Student;

//this struct holds the neccesary data on the hackers
typedef struct Hacker
{
    char id[ID_LENGTH]; //9 digit num
    char** desiredCourses; //List of integers  NOT POINTER TO INT
    char** friendsId; //list of 9 digit char (so char[10]) NOT POINTER TO CHAR
    char** enemiesId; //list of 9 digit char (so char[10]) NOT POINTER TO CHAR
    int coursesHacked; //this is for the last function

}Hacker;

//this struct holds the data on all the courses queues
typedef struct Course
{
    int courseSize;
    int courseNumber;
    IsraeliQueue queue;

}
Course;

//this is the struct that holds all the data on the enrollment system
typedef struct EnrollmentSystem_t
{
    studentPtr* studentArr; //arr of students pointers
    hackerPtr* hackerArr;  //arr of hacker pointers
    coursePtr* courseArr; //arr of courses pointers

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
int friendshipValueByHackerFile(void*,void*); 
int friendshipValueById(void*,void*); 
int friendshipValueByASCII(void*,void*); //see that this function is different because this function takes 

/**
 * this function is used to change the capital letters to small letters on all names and surnames
*/
void changeCapitalLetters(EnrollmentSystem);

/**
 * this function is used to check the difference between the ascii values of two words
 */
int nameDifference(char* firstName, char* secondName);
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

EnrollmentSystem createEnrollment(FILE*, FILE*,FILE*); //done

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

//this are the functions for the structs
/**
 * this function frees all memroy inside the student struct
*/
void destroyStudent(void* student); // done

/**
 * this function frees all memroy inside the course struct
*/
void destroyCourse(void*  course); // done

/**
 * this function frees all memroy inside the course struct
*/
void destroyHacker(void*  hacker); // done

/**
 * this function frees all memroy inside the enrollmentSystem struct
*/
void destroyEnrollmentSystem(void*  sys); // done

/**
 * this function inputs a student file into and creates a array of student structs takes into account the file is valid
*/
studentPtr* inputStudentFile(FILE* student,hackerPtr*);//done

/**
 * this function inputs a course file into and creates a array of course structs
*/
coursePtr* inputCourseFile(FILE* course); //done

/**
 * this function inputs a course file into and creates an array of course structs
*/
hackerPtr* inputHackerFile(FILE* Hacker); //done

/**
 * this function tells us the size of a line, mainly used for a buffer
*/
int lineLength(FILE *f); //done

/**
 * this function tells us how many lines are in the file
*/
int linesInFile(FILE* f); //done

/**
 * this function inputs a buffer and an int and outputs a string array
*/
char** splitBufferToArr(char* buffer, int numOfElements); //done

/**
 * this function tells us how many elements are in the given line
*/
int numOfElementsInLine(FILE *f); //done

/**
 * this function destroys an array of structs
*/
void destroyArr(void** arr, void (*destroyStruct)(void *));

/**
 * this functiontells you at what position the data has entered the list
*/
int israeliQueueGetPosition(IsraeliQueue queue ,void* data);

/**
 * this function is used to split a line of a file into a string array
 */

char** splitLineToArrStrings(FILE *f);
void destroyEnrollmentSystem(EnrollmentSystem);

#endif
