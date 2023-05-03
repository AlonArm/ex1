#include "HackerEnrollment.h"
#include <stdlib.h> 
#include <string.h>    

void destroyStudent(void*temp)
{
    studentPtr student =(studentPtr)temp;
    if(student!=NULL)
    {
    free(student->city);
    free(student->name);
    free(student->surName);
    free(student->department);
    free(student);
    }
}

void destroyCourse(void* temp)
{
    coursePtr course = (coursePtr)temp;
    if(course!=NULL)
    {
    IsraeliQueueDestroy(course->queue);
    free(course);
    }
}

void destroyHacker(void* temp)
{
    hackerPtr hacker = (hackerPtr)temp;
    if(hacker!=NULL)
    {
        free(hacker->desiredCourses);
         free(hacker->friendsId);
          free(hacker->enemiesId);
          free(hacker);
    }

}

EnrollmentSystem createEnrollment(FILE* students, FILE* courses,FILE* hackers)
{
    if(students==NULL||courses==NULL||hackers==NULL)
    {
        return NULL;
    }
    EnrollmentSystem sys = (EnrollmentSystem)malloc(sizeof(EnrollmentSystem_t));
    if(sys==NULL)
    {
        return NULL;
    }
    sys->courseArr=inputCourseFile(courses);
    sys->studentArr=inputStudentFile(students);
    sys->hackerArr=inputHackerFile(hackers,sys->studentArr);
    if(sys->hackerArr==NULL||sys->courseArr==NULL||sys->studentArr==NULL)
    {
        destroyArr((void**)sys->hackerArr,destroyHacker);
        destroyArr((void**)sys->courseArr,destroyCourse);
        destroyArr((void**)sys->studentArr,destroyStudent);
        free(sys);
        return NULL;
    }
    return sys;
}

EnrollmentSystem readEnrollment(EnrollmentSystem sys, FILE* queues)
{
    if(sys==NULL||queues==NULL)
    {
        return NULL;
    }
    
    int lines = linesInFile(queues);
    if(lines==-1||lines==0)
    {
        return NULL;
    }
    for(int i =0;i<=lines;i++)
    {
        int courseNumber = -1;
        fscanf(queues,"%d ",&courseNumber);
        int j = 0;

        while( sys->courseArr[j]!=NULL&&sys->courseArr[j]->courseNumber!=courseNumber)
        {
         j++;
        }
        int numOfIds = numOfElementsInLine(queues);
        int lengthofLine = lineLength(queues);
        char buffer[lengthofLine+1];
        fgets(buffer,lengthofLine+1,queues);
        char** input=NULL;
        if(sys->courseArr[j]->queue==NULL)
        {
            int k = 0;
            while(sys->courseArr[k]!=NULL)
            {
                
               IsraeliQueueDestroy(sys->courseArr[k]->queue);
               return NULL;
            }
        }
        input = splitBufferToArr(buffer,numOfIds);
         if(input==NULL)
         {
             int k = 0;
            while(sys->courseArr[k]!=NULL)
            { 
               IsraeliQueueDestroy(sys->courseArr[k]->queue);
               return NULL;
            }
         }
        int l=0; //indexes for the next iterations
       
        while(l<numOfIds)
        {
             int m =0; //indexes for the next iterations
         while(strcmp(input[l],sys->studentArr[m]->id)!=0)
         {
             m++;
         }   
        if(IsraeliQueueEnqueue(sys->courseArr[j]->queue,(void*)sys->studentArr[m])!=ISRAELIQUEUE_SUCCESS)
        {
            int k = 0;
            while(sys->courseArr[k]!=NULL)
            {
                
               IsraeliQueueDestroy(sys->courseArr[k]->queue);
               return NULL;
            }
            free(input);
        }
        l++;
        }
   }
   return sys;
}

void destroyArr(void** arr, void (*destroyStruct)(void *))
{
    if(arr!=NULL&&destroyStruct!=NULL)
    {
    int i = 0;
while(arr[i]!=NULL)
{
    destroyStruct(arr[i]);
    i++;
}
free(arr);

}
}

studentPtr* inputStudentFile(FILE* f)
{
   int numOfLines = linesInFile(f);
   if(numOfLines==-1)
   {
       return NULL;
   }
    studentPtr* StudentArr = (studentPtr*)malloc(sizeof(studentPtr)*(numOfLines+1));
    for(int i = 0;i<numOfLines;i++)
    {
        StudentArr[i]=(studentPtr)malloc(sizeof(Student));
        if(StudentArr[i]==NULL)
        {
           while(i>=0)
           {
               destroyStudent(StudentArr[i]);
               i--;
           }
           free(StudentArr);
           return NULL;
        }
        fscanf(f,"%10s %d %lf ",StudentArr[i]->id,&StudentArr[i]->totalCredits,&StudentArr[i]->gpa);
        int restOfLineLength = lineLength(f);
        char buffer[restOfLineLength+1];
        if(fgets(buffer,restOfLineLength,f)==NULL)
        { 
            printf("error in fgets");
            while(i>=0)
           {
               destroyStudent(StudentArr[i]);
               i--;
           }
           free(StudentArr);
           return NULL;
        }
        StudentArr[i]->name = strtok(buffer," ");
         StudentArr[i]->surName = strtok(NULL," ");
          StudentArr[i]->city = strtok(NULL," ");
           StudentArr[i]->department = strtok(NULL," ");
        if(StudentArr[i]->city==NULL||StudentArr[i]->name==NULL||StudentArr[i]->surName==NULL||StudentArr[i]->department==NULL)
      {
          while(i>=0)
           {
               destroyStudent(StudentArr[i]);
               i--;
           }
          free(StudentArr);
          return NULL;
     }       
    }
    StudentArr[numOfLines]=NULL;
    return StudentArr;

}

coursePtr* inputCourseFile(FILE* f)
{
    if(f==NULL)
    {
        return NULL;
    }
  int numOflines = linesInFile(f);
  if(numOflines==-1)
  {
      return NULL;
  }
  coursePtr* courseArr = (coursePtr*)malloc(sizeof(coursePtr)*(numOflines+1));
    if(courseArr==NULL)
    {
        return NULL;
    }
    for(int i= 0;i<=numOflines;i++)
    {
        courseArr[i]= (coursePtr)malloc(sizeof(Course));
        if(courseArr[i]==NULL)
        {
            while(i>=0)
            {
                destroyCourse(courseArr[i]);
                i--;
            }
            free(courseArr);
            return NULL;
        }
        fscanf(f,"%d %d\n",&courseArr[i]->courseNumber,&courseArr[i]->courseSize);
        courseArr[i]->queue = IsraeliQueueCreate(NULL,NULL,FRIENDSHIP_TH,ENEMY_TH);
        
        if(IsraeliQueueAddFriendshipMeasure(courseArr[i]->queue,friendshipValueById)!=ISRAELIQUEUE_SUCCESS||
        IsraeliQueueAddFriendshipMeasure(courseArr[i]->queue,friendshipValueByHackerFile)!=ISRAELIQUEUE_SUCCESS
        ||IsraeliQueueAddFriendshipMeasure(courseArr[i]->queue,friendshipValueByASCII)!=ISRAELIQUEUE_SUCCESS)
        {
             while(i>=0)
            {
                destroyCourse(courseArr[i]);
                i--;
            }
            free(courseArr);
            return NULL;
        }
    }
    courseArr[numOflines]=NULL;
    return courseArr;

}

hackerPtr* inputHackerFile(FILE *f,studentPtr* studentArr)
{
        if(f==NULL)
    {
        return NULL;
    }
    int numOfLines = linesInFile(f);
    if(numOfLines==-1||numOfLines%4!=0)
    {
        return NULL;
    }
    hackerPtr* hackerArr = (hackerPtr*)malloc(sizeof(hackerPtr)*(numOfLines/4+1));
    if(hackerArr==NULL)
    {
        return NULL;
    }
    for(int i=0;i<numOfLines;i++)
    {
        hackerArr[i]= (hackerPtr)malloc(sizeof(Hacker));
        if(hackerArr[i]==NULL)
        {
        while(i>=0)
        {
            destroyHacker(hackerArr[i]);
            i--;
        }
        free(hackerArr);
        return NULL;
        }
        //change this part to be less repetetive
        fscanf(f,"%10s\n",hackerArr[i]->id);
        int numOfValuesInDesiredCourses = numOfElementsInLine(f);
        int lengthDesiredCourses = lineLength(f);
        char bufferDesiredCourses[lengthDesiredCourses+1];
        fgets(bufferDesiredCourses,lengthDesiredCourses+1,f);
        hackerArr[i]->desiredCourses=splitBufferToArr(bufferDesiredCourses,numOfValuesInDesiredCourses);

        int numOfValuesInfriends = numOfElementsInLine(f);
        int lengthFriendsId = lineLength(f);
        char bufferFriendsId[lengthFriendsId+1];
        fgets(bufferFriendsId,lengthFriendsId+1,f);
         hackerArr[i]->friendsId=splitBufferToArr(bufferFriendsId,numOfValuesInfriends);

        int numOfValueInEnenmy = numOfElementsInLine(f);
        int lengthEnemiesId= lineLength(f);
        char bufferEnemiesId[lengthEnemiesId+1];
        fgets(bufferEnemiesId,lengthEnemiesId+1,f);
         hackerArr[i]->enemiesId=splitBufferToArr(bufferEnemiesId,numOfValueInEnenmy);
         if(hackerArr[i]->friendsId==NULL||hackerArr[i]->enemiesId==NULL||hackerArr[i]->desiredCourses==NULL)
         {
             while(i>=0)
             {
                 destroyHacker(hackerArr[i]);
                 i--;
             }
             free(hackerArr);
             return NULL;
         }   

            int j =0;
            while(strcmp(studentArr[j]->id,hackerArr[i]->id)!=0)
            {
                j++;
            }    
            hackerArr[i]->name=studentArr[i]->name;
            hackerArr[i]->surName=studentArr[i]->surName;
            hackerArr[i]->coursesHacked=2; //initializing this to 2 so that when it reaches 0 i know he is ok

    }
    hackerArr[numOfLines]=NULL;
  return hackerArr; 
}

int lineLength(FILE *f)
{
    if (f==NULL)
    {
        return -1;
    }
    int i=0;
    long position = ftell(f);
    char c;
    while((c =fgetc(f))!='\n'||c!=EOF)
    {
        i++;
    }
    fseek(f, position, SEEK_SET);
    return i;
}

int numOfElementsInLine(FILE *f)
{
     if (f==NULL)
    {
        return -1;
    }
    int i=0;
    long position = ftell(f);
    char c;
    while((c =fgetc(f))!='\n'||c!=EOF)
    {
    if(c==' ')
    {
        i++;
    }
   
    }
     fseek(f,position,SEEK_SET);
    return i;
}

int linesInFile(FILE* f)
{
    if(f==NULL)
    {
        return -1;
    }
    int i = 0;
    char c=fgetc(f);
        if(c==EOF)
        {
            return i;
        }
        i++;
    while(c!=EOF)
    {
        if(c=='\n')
        {
            i++;
        }
        c= fgetc(f);
    }
    rewind(f);
    return i;
    
}

char** splitBufferToArr(char* buffer,int numOfElements)
{
    if(buffer==NULL||numOfElements==-1)
    {
        return NULL;
    }
    char** arr = malloc((numOfElements+1)*sizeof(char*));
    if(arr==NULL)
    {
        return NULL;
    }
    char* token = strtok(buffer, " ");
  for(int i=0;i<numOfElements;i++)
  {

      arr[i] = token;
      token = strtok(NULL, " ");
  }
  arr[numOfElements+1]=NULL;
  return arr;
    

}

int friendshipValueByHackerFile(void* paramHacker,void* paramStudent)
{
    hackerPtr hacker =(hackerPtr)paramHacker;
    studentPtr student =(studentPtr)paramStudent;
    
    int i = 0;
    while(hacker->friendsId[i]!=NULL)
    {
        if(strcmp(student->id,hacker->friendsId[i])==0)
        {
            return 20;
        }
        i++;
    }
    int j =0;
    while(hacker->enemiesId[j]!=NULL)
    {
        if(strcmp(student->id,hacker->enemiesId[j])==0)
        {
            return -20;
        }
        j++;
    }
    return 0;
}

int friendshipValueById(void* hackerId,void* studentId)
{
    hackerPtr hacker = (hackerPtr)hackerId;
    studentPtr student = (studentPtr)studentId;

        int hackerValue = atoi(hacker->id);
        int studentValue = atoi(student->id);
        if(hackerValue-studentValue>0)
        {
           return hackerValue-studentValue;
        }
        return studentValue-hackerValue;
}

int friendshipValueByASCII(void* hackerParam,void* studentParam)
{
    hackerPtr hacker = (hackerPtr)hackerParam;
    studentPtr student = (studentPtr)studentParam;
    int value=0;
    value=nameDifference(hacker->name,student->name);
    value+=nameDifference(hacker->surName,student->surName);
    return value;
}

char changeCapitalLetter(char c)
{
        if(c>=65&&c<=91)
        {
        c=c+32;
        }
        return c;
}

int nameDifference(char* firstName, char* secondName)
{
    int sum=0;
    int j =0;
    char name1;
    char name2;

   
    while(firstName[j]!='\0'&&secondName[j]!='\0')
    { 
        name1 =changeCapitalLetter(firstName[j]);
        name2 =changeCapitalLetter(secondName[j]);
        if(name1>name2)
        {
            sum+=(name1-name2);
        }
        else
        {
             sum+=(-name1+name2);
        }
        j++;
    }
    if(strlen(firstName)==strlen(secondName))
    {
        return sum;
    }
    if(strlen(firstName)>strlen(secondName))
    {
        
        while(firstName[j]!='\0')
        {
            sum+=changeCapitalLetter(firstName[j]);
            j++;
        }
    }
    if(strlen(secondName)>strlen(firstName))
    {
        
        while(secondName[j]!='\0')
        {
            sum+=changeCapitalLetter(secondName[j]);
            j++;
        }
        
    }
return sum;
}

void hackEnrollment(EnrollmentSystem sys, FILE* out)
{
    int i =0;
    while(sys->courseArr[i]!=NULL)
    {
        int j=0;
        while(sys->hackerArr[j]!=NULL)
        {
            int k=0;
            while(sys->hackerArr[j]->desiredCourses!=NULL)
            {
                if(atoi(sys->hackerArr[j]->desiredCourses[k])==sys->courseArr[i]->courseNumber)
                {
                    if(IsraeliQueueEnqueue(sys->courseArr[i]->queue,(void*)sys->hackerArr[j])!=ISRAELIQUEUE_SUCCESS)
                    {
                        return; //i seriously have no idea what else to do in a void function if this alloc fails
                    }
                    if(israeliQueueGetPosition(sys->courseArr[i]->queue,(void*)sys->hackerArr[j])<sys->courseArr[i]->courseSize)
                    {
                        sys->hackerArr[j]->coursesHacked--;
                    }
                }
                k++;
            }
            j++;
        }
        i++;
    }
    
    int l=0;
    while(sys->hackerArr[l]!=NULL)
    {
        if(sys->hackerArr[l]->coursesHacked>0)
        {
            fprintf(out,"Cannot satisfy constraints for %s",sys->hackerArr[l]->id);
            return;
        }
    }
    

}
int israeliQueueGetPosition(IsraeliQueue queue ,void* data)
{
    int i=1;
    personPtr temp =queue->head;
    while(temp!=NULL)
    {
        if(temp->person==data)
        {
            return i;
        }
        i++;
        temp=temp->next;
    }
    return -1;//if error but there is not supposed to be any
}

void printQueueToFile(IsraeliQueue queue,FILE* out)
{
    personPtr temp =queue->head;
    while(temp!=NULL)
    {
     
     fprintf(out, "%s ",)
    }
}
