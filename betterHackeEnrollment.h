#ifndef PROVIDED_ISRAELIQUEUE_H
 #include "IsraeliQueue.h" //need to change to the place where this is actually at when finsihed
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

//this is a generic list data structure 
typedef struct Node {
    void* data;
    nodePtr next;
} Node;

typedef struct List {
    nodePtr head;
} List;

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

}Student;

//this struct holds the neccesary data on the hackers
typedef struct Hacker
{

    char id[ID_LENGTH]; //9 digit num
    listPtr desiredCourses; //List of integers  NOT POINTER TO INT
    listPtr friendsId; //list of 9 digit char (so char[10]) NOT POINTER TO CHAR
    listPtr enemiesId; //list of 9 digit char (so char[10]) NOT POINTER TO CHAR


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
    listPtr studentList; //arr of students pointers
    listPtr hackerList;  //arr of hacker pointers
    listPtr courseList; //arr of courses pointers

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

EnrollmentSystem createEnrollment(FILE*, FILE*,FILE*);

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

//the functions below were not given by the instructors but they are crucial for this exercise

//these 2 functions will help us read from files
/**
 * this function receives a file and reads the first word before a space that its pointer is pointing to,
 * it creates that specific word, returns true if the malloc suceded and false else
*/
HackerEnrollmentError readString(FILE *f, char** word); //done
/**
 * this function recieves a pointer to a file and reads up to 10 whole numbers
 * and a null terminator and returns the string that it outputs
*/
char* readWholeNumber(FILE *f); //done


//this functions will help us in use of the list
/**
 * this function frees up all memory appointed by a list
*/
HackerEnrollmentError destroyList(listPtr List); //done

/**
 * this function returns a pointer to a newly created list with a tail and a head
*/
listPtr createList(); //done

/**
 * this function adds creates another node and returns it
*/
nodePtr createNewNode(void* data); //done
/**
 * this function adds another element to the end of the list
*/
HackerEnrollmentError addAnotherNode(listPtr list,void* data); //done


//this are the functions for the structs
/**
 * this function frees all memroy inside the student struct
*/
HackerEnrollmentError destroyStudent(studentPtr student); //done

/**
 * this function frees all memroy inside the course struct
*/
HackerEnrollmentError destroyCourse(coursePtr course); //done

/**
 * this function frees all memroy inside the course struct
*/
HackerEnrollmentError destroyHacker(hackerPtr hacker); //done

/**
 * this function erases a data struct entirely
*/
HackerEnrollmentError destroyStructList(listPtr list, HackerEnrollmentError(*add_ptr)(void*)); //done 

//need to create createStudent, createHacker, createCourse struct

/**
 * this function inputs a student file into and creates a list of student structs
*/
listPtr inputStudentFile(FILE* student);

/**
 * this function inputs a course file into and creates a list of course structs
*/
listPtr inputCourseFile(FILE* course);

/**
 * this function inputs a course file into and creates a list of course structs
*/
listPtr inputHackerFile(FILE* Hacker);


#endif
