#include "HackerEnrollment.h"
#include <stdlib.h> 
#include <string.h>    

int linesInFile(FILE *f);
void freeArr(void** arr,int currIndex); //this function is supposed to help in not writing mundane code
studentPtr* studentEnrollment(FILE* students,int linesInStudentFile); //helper function to keep the code more organized
coursePtr* courseEnrollment(FILE* courses,int linesIncoursesFile); //helper function to keep the code more organized
hackerPtr* hackerEnrollment(FILE* hackers,int linesInhackersFile,int numOfStudents,int numOfCourses); //helper function to keep the code more organized
bool personalizedFgetc(char* param, FILE* inputFile); //creates a string, returns true if allocation succeded, false otherwise

EnrollmentSystem createEnrollment(FILE *students, FILE* courses, FILE* hackers)
 //need to free each struct individually in case one is broken
{
    int linesInStudentFile = linesInFile(students);
    int linesIncourseFile = linesInFile(courses);
    int linesInHackerFile = linesInFile(hackers);
    
    if(linesInStudentFile<0||linesInStudentFile<0||linesInHackerFile<0||linesInHackerFile%4!=0)
    {
        return NULL;
    }
    EnrollmentSystem system = malloc(sizeof(EnrollmentSystem_t));
    system->studentArr = studentEnrollment(students,linesInStudentFile);
    system->courseArr = courseEnrollment(courses,linesIncourseFile);
    system->hackerArr = hackerEnrollment(hackers,linesInHackerFile,linesInStudentFile,linesInHackerFile); // need to fix

    if( system->studentArr==NULL||system->courseArr==NULL||system->hackerArr==NULL)
    {
        //need to free each one individually
        free(system);
    return NULL;
    }
    return system;

}

studentPtr* studentEnrollment(FILE* students,int linesInStudentFile) 
{
     studentPtr* studentArr = malloc(linesInStudentFile * sizeof(studentPtr));
     if(studentArr==NULL)
     {  
         return NULL;
     }
     for(int i=0;i<linesInStudentFile;i++)
        {
            studentArr[i]=malloc(sizeof(Student));
            if(studentArr[i]==NULL) //checking if the malloc succeded
            { 
               freeArr((void**)studentArr,i); //if one of the mallocs fail i want to return a null pointer
               rewind(students);
                return NULL;
            }
             fscanf(students, "%d %d %lf", &studentArr[i]->id, &studentArr[i]->totalCredits, 
             &studentArr[i]->gpa);

             if(personalizedFgetc(studentArr[i]->name)==false)
            {
                //som kind of freeing of the data
            }
            if(personalizedFgetc(studentArr[i]->surName)==false)
            {
                //som kind of freeing of the data
            }
            if(personalizedFgetc(studentArr[i]->city)==false)
            {
                //som kind of freeing of the data
            }
            if(personalizedFgetc(studentArr[i]->department)==false)
            {
                //som kind of freeing of the data
            }
        
        }
        rewind(students);
        return studentArr;
}
coursePtr* courseEnrollment(FILE* courses,int linesInCourseFile)
{

    coursePtr* courseArr = malloc(linesInCourseFile*sizeof(coursePtr));
    if(courseArr==NULL)
    {
        return NULL;
    }
    for (int i = 0; i < linesInCourseFile; i++)
    {
        courseArr[i]=malloc(sizeof(Course));
         if(courseArr[i]==NULL) //checking if the malloc succeded
            { 
               freeArr((void**)courseArr,i); //if one of the mallocs fail i want to return a null pointer
               rewind(courses);
                return NULL;
            }
            fscanf(courses,"%d %d\n",&courseArr[i]->courseNumber,&courseArr[i]->courseSize);
            courseArr[i]->queue=NULL;
    }
    rewind(courses);
    return courseArr;
    
}

hackerPtr* hackerEnrollment(FILE* hackers,int linesInHackerFile,int numOfStudents,int numOfCourses)//need to do more checks on lines in hackerfile
{    
    hackerPtr* hackerArr = malloc(linesInHackerFile/4*sizeof(hackerPtr));
    int i =0;
    if(hackerArr==NULL)
    {
        return NULL;
    }
    while(linesInHackerFile>=4) //checking in the enrollment system file the file is correct
    {
        hackerArr[i]=malloc(sizeof(Hacker));
        if(hackerArr[i]==NULL)
        {
            freeArr((void**)hackerArr,i);
            rewind(hackers);
            return NULL;
        }
        fscanf(hackers,"%9d%*c",&hackerArr[i]->id);
        fgets(hackerArr[i]->desiredCourses,(numOfCourses+1),hackers);  //the +1 is because there are spaces
        //check for validity - check what checks need to be done inside fgets
        fgets(hackerArr[i]->friendsId,(numOfStudents+1),hackers);
        //check for validity
        fgets(hackerArr[i]->enemiesId,(numOfStudents+1),hackers);
           //check for validity
           linesInHackerFile=linesInHackerFile-4;
        i++;
    }
    rewind(hackers);
    return hackerArr;
}

int linesInFile(FILE* f) //this function returns the number of lines in a given program and is a helper function
{
    char curr ;
    int linesCounter = 0;
    if(f==NULL)
    {
        return -1;
    }
    while((curr=fgetc(f)) != EOF)
    {
        if(curr == '\n')
        {
            linesCounter++;
        }
    }
    rewind(f);
    return linesCounter;
}
void freeArr(void** arr,int currIndex) //helper function to free any array
{
    while(currIndex>=0)
    {
        free(arr[currIndex]);
    }
    free(arr);
}
bool personalizedFgetc(char* param, FILE* inputFile) // returns true in case the allocation worked false otherwise
{  
     char temp;
   int charCounter = 1;
       param = malloc(sizeof(char));
    if(param==NULL)
    {
        free(param);
        return false;
    }
    if((temp =fgetc(inputFile)) != EOF && temp != '\n' && temp!=' ')
    {
        param[0] = temp;
    }

    while ((temp =fgetc(inputFile)) != EOF && temp != '\n' && temp!=' ') 
    {
      charCounter++;   
      param = realloc(param,charCounter*sizeof(char));
      if(param==NULL)
      {
          free(param); 
          return false;
      }
    param[charCounter-1] = temp;
    }
    return true;
}


