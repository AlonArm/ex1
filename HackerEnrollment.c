#include "HackerEnrollment.h"
#include <stdlib.h> 
#include <string.h>    

int linesInFile(FILE *f); //this function counts how many lines are in a given file, returns a negative number if there is no file, else returns how many rows in that file
void freeArr(void** arr,int currIndex); //this function is supposed to help in not writing mundane code
bool readLine(char** buffer, FILE* inputFile); //this function is supposed to read a line and put it inside a buffer
bool personalizedFgetc(char** param, FILE* inputFile); //creates a string from a line, returns true if allocation succeded, false otherwise
void freeStudentStrings(studentPtr* studentArr,int currIndex);// this function is supposed to cut down on mundane operations
void intializeArr(void** arr,int length); //this function initializes the array pointers to null
void freeHackerStrings(hackerPtr* arr,int currIndex); //this function frees the hacker strings
studentPtr* studentEnrollment(FILE* students,int linesInStudentFile); //helper function to keep the code more organized

coursePtr* courseEnrollment(FILE* courses,int linesIncoursesFile); //helper function to keep the code more organized

hackerPtr* hackerEnrollment(FILE* hackers,int linesInhackersFile,int numOfStudents,int numOfCourses); //helper function to keep the code more organized

//this is one of the functions inside the h file
EnrollmentSystem createEnrollment(FILE *students, FILE* courses, FILE* hackers)
 //need to free each struct individually in case one is broken
{
    //checking if the files are ok and thier size
    int linesInStudentFile = linesInFile(students); 
    int linesIncourseFile = linesInFile(courses);
    int linesInHackerFile = linesInFile(hackers);
    
    if(linesInStudentFile<0||linesInStudentFile<0||linesInHackerFile<0||linesInHackerFile%4!=0)
    {
        return NULL; //if one file input was wrong
    }
    EnrollmentSystem system = malloc(sizeof(EnrollmentSystem_t)); //creating the struct
    if(system==NULL)
    {
        return NULL;
    }

    //creating the inner struct course arr and checking if it ran smoothly
    system->courseArr = courseEnrollment(courses,linesIncourseFile); 
    if(system->courseArr==NULL)
    {
        free(system);
        return NULL;
    }
    //creating the hacker struct from the hacker file, if there was a prblem with allocation then we need to free all the mallocs above
    system->hackerArr = hackerEnrollment(hackers,linesInHackerFile,linesInStudentFile,linesInHackerFile); 
    if(system->hackerArr==NULL)
    {
        freeArr((void**)system->courseArr,linesIncourseFile-1);
        free(system);
        return NULL;
        
    }
    //creating the student structs from the file, need to free all the mallocs if there was a problem, including the strings from the prev struct
    system->studentArr = studentEnrollment(students,linesInStudentFile);
    if(system->studentArr==NULL)
    {
        freeHackerStrings(system->hackerArr,linesInHackerFile/4);
        freeArr((void**)system->courseArr,linesIncourseFile-1);
         freeArr((void**)system->hackerArr,linesInHackerFile/4-1);
         free(system);
         return NULL;

    }
    return system;

}


//this are the functions which read the texts from the files and enters them into our structs
studentPtr* studentEnrollment(FILE* students,int linesInStudentFile) 
{
    //here we input the data from the file into the struct, if there was failure in allocating memory then we free everything
     studentPtr* studentArr = malloc(linesInStudentFile * sizeof(studentPtr)); 
     if(studentArr==NULL)
     {  
         return NULL;
     }
     intializeArr((void**)studentArr,linesInStudentFile);

     for(int i=0;i<linesInStudentFile;i++)
        {
            studentArr[i]=malloc(sizeof(Student));
            if(studentArr[i]==NULL) //checking if the malloc succeded
            { 
               freeArr((void**)studentArr,i); //if one of the mallocs fail i want to return a null pointer
               rewind(students);
                return NULL;
            }
             fscanf(students, "%s %d %lf", studentArr[i]->id, &studentArr[i]->totalCredits, 
             &studentArr[i]->gpa);
             studentArr[i]->id[ID_LENGTH-1]='\0';

             if(personalizedFgetc(&studentArr[i]->name,students)==false)
            {
             freeStudentStrings(studentArr,i);
               freeArr((void**)studentArr,i);
               rewind(students);
               return NULL;
            }
            if(personalizedFgetc(&studentArr[i]->surName,students)==false)
            {
               free(studentArr[i]->name);
               freeStudentStrings(studentArr,i);
               freeArr((void**)studentArr,i);
               rewind(students);
               return NULL;

            }
            if(personalizedFgetc(&studentArr[i]->city,students)==false)
            {
               free(studentArr[i]->name); 
                free(studentArr[i]->surName);
               freeStudentStrings(studentArr,i);
               freeArr((void**)studentArr,i);
               rewind(students);
               return NULL;
            }
            if(personalizedFgetc(&studentArr[i]->department,students)==false)
            {
                free(studentArr[i]->name); 
                free(studentArr[i]->surName);
                free(studentArr[i]->city);
               freeStudentStrings(studentArr,i);
               freeArr((void**)studentArr,i);
               rewind(students);
               return NULL;
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
         intializeArr((void**)courseArr,linesInCourseFile);

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
         intializeArr((void**)hackerArr,numOfStudents);

    while(linesInHackerFile>=4) //checking in the enrollment system file the file is correct
    {
        hackerArr[i]=malloc(sizeof(Hacker));
        if(hackerArr[i]==NULL)
        {
            freeArr((void**)hackerArr,i);
            rewind(hackers);
            return NULL;
        }
        fscanf(hackers,"%s%*c",hackerArr[i]->id);
        hackerArr[i]->id[ID_LENGTH-1]='\0'; //null terminator at the end of a string

         if(readLine(&hackerArr[i]->desiredCourses,hackers)==false)
            {
                freeHackerStrings(hackerArr,i);
                freeArr((void**)hackerArr,i);
                rewind(hackers);
                return NULL;
            }
             if(readLine(&hackerArr[i]->friendsId,hackers)==false)
            {   
                free(hackerArr[i]->desiredCourses);
                freeHackerStrings(hackerArr,i);
                freeArr((void**)hackerArr,i);
                rewind(hackers);
                return NULL;
            }
              if(readLine(&hackerArr[i]->enemiesId,hackers)==false)
            {   
                free(hackerArr[i]->friendsId);
                free(hackerArr[i]->desiredCourses);
                freeHackerStrings(hackerArr,i);
                freeArr((void**)hackerArr,i);
                rewind(hackers);
                return NULL;
            }
           //check for validity
           linesInHackerFile=linesInHackerFile-4;
        i++;
    }
    rewind(hackers);
    return hackerArr;
}




//down here are all the functions that help us read the text from the files
//this function returns the number of lines in a given program and is a helper function
int linesInFile(FILE* f) 
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
//helper function to free any array of pointers to structs
void freeArr(void** arr,int currIndex) 
{
    while(currIndex>=0)
    {
        free(arr[currIndex]);
    }
    free(arr);
}
// this function is supposed to cut down on mundane operations and frees the
//previous structs string mallocs
void freeStudentStrings(studentPtr* studentArr,int currIndex)
{
    while(currIndex>=1)
    {
        free(studentArr[currIndex-1]->name);
        free(studentArr[currIndex-1]->surName);
        free(studentArr[currIndex-1]->city);
        free(studentArr[currIndex-1]->department);
        currIndex--;
    }
}
//this function initializes the array pointers to null so that undefined behaviour doesnt happen
void intializeArr(void** arr,int length) 
{
    int i = 0;
    while(i<length)
    {
    arr[i] =NULL;
    i++;
    }

}
//this function reads a line and enters it intos somekind of buffer, returns true if succeded and false else
bool readLine(char**buffer, FILE* inputFile) 
{  
     char temp;
   int charCounter = 2;
       *buffer = malloc(sizeof(char)*2);
    if(*buffer==NULL)
    {
        free(*buffer);
        return false;
    }
    if((temp =fgetc(inputFile)) != EOF && temp != '\n')
    {
        (*buffer)[0] = temp;
    }

    while ((temp =fgetc(inputFile)) != EOF && temp != '\n')
    {
      charCounter++;   
      *buffer = realloc(*buffer,charCounter*sizeof(char));
      if(*buffer==NULL)
      {
          free(*buffer); 
          return false;
      }
    (*buffer)[charCounter-2] = temp;
    }
    (*buffer)[charCounter-1] = '\0';
    return true;
}
// returns true in case the allocation worked, false otherwise
bool personalizedFgetc(char** param, FILE* inputFile) 
{  
     char temp;
   int charCounter = 2;
       *param = malloc(2*sizeof(char));
    if(*param==NULL)
    {
        free(*param );
        return false;
    }
    if((temp =fgetc(inputFile)) != EOF && temp != '\n' && temp!=' ')
    {
        (*param)[0] = temp;
    }

    while ((temp =fgetc(inputFile)) != EOF && temp != '\n' && temp!=' ') 
    {
      charCounter++;   
      *param = realloc(*param ,charCounter*sizeof(char));
      if(*param ==NULL)
      {
          free(*param ); 
          return false;
      }
    (*param)[charCounter-2] = temp;
    }
    (*param)[charCounter-1]='\0';
    return true;
}
//this helper function deletes the previous hacker strings
void freeHackerStrings(hackerPtr* arr,int currIndex)
{
    while(currIndex>=1)
    {
        free(arr[currIndex-1]->desiredCourses);
          free(arr[currIndex-1]->friendsId);
          free(arr[currIndex-1]->enemiesId);
        currIndex--;
    }
}
