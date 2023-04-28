#include "HackerEnrollment.h"
#include <stdlib.h> 
#include <string.h>    

int linesInFile(FILE *f);
void freeArr(void** arr,int currIndex); //this function is supposed to help in not writing mundane code
studentPtr* studentEnrollment(FILE* students,int linesInStudentFile); //helper function to keep the code more organized
coursePtr* courseEnrollment(FILE* courses,int linesIncoursesFile); //helper function to keep the code more organized
hackerPtr* hackerEnrollment(FILE* hackers,int linesInhackersFile); //helper function to keep the code more organized


EnrollmentSystem createEnrollment(FILE *students, FILE* courses, FILE* hackers)
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
    system->hackerArr = hackerEnrollment(hackers,linesInHackerFile);

    if( system->studentArr==NULL||system->courseArr==NULL||system->hackerArr==NULL)
    {
    return NULL;
    }
    return system;

}

studentPtr* studentEnrollment(FILE* students,int linesInStudentFile) //need to check that lines is greater than 1
{
    FILE* tmpFile = students;
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
                return NULL;
            }
             if(fscanf(tmpFile, "%9d %d %3lf %s %s %s %[^\n]", &studentArr[i]->id, &studentArr[i]->totalCredits, 
             &studentArr[i]->gpa, studentArr[i]->name, studentArr[i]->surName, studentArr[i]->city, studentArr[i]->department)!=7||
             studentArr[i]->id<0||studentArr[i]->totalCredits<0||studentArr[i]->gpa<0)
             {
                freeArr((void**)studentArr,i);
                 return NULL;
             }
        }
        return studentArr;
}
coursePtr* courseEnrollment(FILE* courses,int linesInCourseFile)
{
    FILE* tmpFile = courses;
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
                return NULL;
            }
            if (fscanf(tmpFile,"%d %d\n",&courseArr[i]->courseNumber,&courseArr[i]->courseSize)!=2||courseArr[i]->courseNumber<0
            ||courseArr[i]->courseSize<0)
            {
                freeArr((void**)courseArr,i);
                 return NULL;
            }
            courseArr[i]->queue=NULL;
    }
    return courseArr;
    
}
hackerPtr* hackerEnrollment(FILE* hackers,int linesInHackerFile)//need to do more checks on lines in hackerfile
{   
    
    FILE *tmpfile = hackers;
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
            return NULL;
        }
        if(fscanf(tmpfile,"%9d\n",&hackerArr[i]->id)!=1||hackerArr[i]->id<0)
        {
            freeArr((void**)hackerArr,i);
            return NULL;
        }
        linesInHackerFile--;
        //input everything into the courses file
        if(fgets(hackerArr[i]->desiredCourses,BUFFER_LENGTH,tmpfile)==NULL)
        {
            freeArr((void**)hackerArr,i);
            return NULL;
        }
         linesInHackerFile--;
         //input everything into friends id
        if(fgets(hackerArr[i]->friendsId,BUFFER_LENGTH,tmpfile)==NULL)
        {
            freeArr((void**)hackerArr,i);
            return NULL;
        }
        linesInHackerFile--;
        //input everything into enemiesid
        if(fgets(hackerArr[i]->enemiesId,BUFFER_LENGTH,tmpfile)==NULL)
        {
            freeArr((void**)hackerArr,i);
            return NULL;
        }
        linesInHackerFile--;
        
        i++;
    }
    return hackerArr;
}
//returns -2 if file is corrupt, returns -1 if there is no file,returns 0 if file is empty, returns num>0 if file is ok
int linesInFile(FILE* f) //this function returns the number of lines in a given program and is a helper function
{
    char buffer[BUFFER_LENGTH];
    FILE* tmp =f;
    int count = 0;
    if(tmp ==NULL)
    {
        return -1; //if there is no file at all just return -1
    }
    while(fgets(buffer,BUFFER_LENGTH,f)!=NULL)
    {
        if(buffer[strlen(buffer)-1]=='\n')
        {
            count++;
        }
        else
        {
            return -2; //there is an error with the file
        }
    }
    return count;
}
void freeArr(void** arr,int currIndex) //helper function to free any array
{
    while(currIndex>=0)
    {
        free(arr[currIndex]);
    }
    free(arr);
}





