#include "HackerEnrollment.h"
#include <stdlib.h> 
#include <string.h>    

void destroyStudent(void*temp)
{ 
    studentPtr student =(studentPtr)temp;
    if(student!=NULL)
    {
        //we free everything that we have allocated, maybe because i didnt build the char** elements might cause a problem
        free(student->city);
        free(student->name);
        free(student->surName);
        free(student->department);
        free(student);
    }
}

void destroyCourse(void* temp)
{
    //the course doesnt hold dynamically allocated data except for the queue therefore well only destroy queue
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
        //same thing with char** i didnt create the elements but im not sure that it wont leak
        free(hacker->desiredCourses);
        free(hacker->friendsId);
        free(hacker->enemiesId);
        free(hacker);
    }
}

EnrollmentSystem createEnrollment(FILE* students, FILE* courses,FILE* hackers)
{
    if(students==NULL||courses==NULL||hackers==NULL) //if one is null then there is a problem
    {
        return NULL;
    }
    EnrollmentSystem sys = (EnrollmentSystem)malloc(sizeof(EnrollmentSystem_t));
    if(sys==NULL) //if the malloc didnt succeed returning null
    {
        return NULL;
    }
    sys->courseArr=inputCourseFile(courses); //use of malloc
    sys->hackerArr=inputHackerFile(hackers);
    sys->studentArr=inputStudentFile(students,sys->hackerArr);

    if(sys->hackerArr==NULL||sys->courseArr==NULL||sys->studentArr==NULL) //if malloc didnt succeed free so no memory leak
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
    if(sys==NULL||queues==NULL) //if param are bad return null
    {
        return NULL;
    }
    
    int lines = linesInFile(queues); //check how many lines in file
    if(lines==0) //if file doesnt have lines or is corrupt return null
    {
        return NULL;
    }
    for(int i =0;i<=lines;i++) //for all lines in the file
    {
        int courseNumber = -1;
        fscanf(queues,"%d ",&courseNumber); //scan the course number
        int j = 0;

        while( sys->courseArr[j]!=NULL&&sys->courseArr[j]->courseNumber!=courseNumber) //check for the coursenum in the courses array once found stop
        {
            j++;
        }
        int numOfIds = numOfElementsInLine(queues);  //check how many ids the course has 
        char** input=NULL; //initialize the input array
        input = splitLineToArrStrings(queues); //create the array of ids with malloc
        if(input==NULL) //if the malloc failed destroy the queues that you have added the inputs to
        {
            int k = 0;
            while(sys->courseArr[k]!=NULL){ 
                IsraeliQueueDestroy(sys->courseArr[k]->queue); 
                return NULL;
            }
        }
        int l=0; //indexes for the next iterations
       
        while(l<numOfIds) //search for the ids in the student arr, and when you find them add the, using enqueue
        {
            int m =0; //indexes for the next iterations
            while(strcmp(input[l],sys->studentArr[m]->id)!=0)
            {
                m++;
            }   
            if(IsraeliQueueEnqueue(sys->courseArr[j]->queue,(void*)sys->studentArr[m])!=ISRAELIQUEUE_SUCCESS) //if malloc not succeeded delete
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
    if(arr!=NULL&&destroyStruct!=NULL) //if inputs are not bad delete all elements of the array
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

studentPtr* inputStudentFile(FILE* f,hackerPtr* hackerArr)
{
    int numOfLines = linesInFile(f); //searching how much students to create every line = 1 student
    studentPtr* StudentArr = (studentPtr*)malloc(sizeof(studentPtr)*(numOfLines+1));
    for(int i = 0;i<numOfLines;i++)
    { 
        StudentArr[i]=(studentPtr)malloc(sizeof(Student)); 
        if(StudentArr[i]==NULL) //if malloc didnt succeed delete all the mallocs created
        {
            destroyArr((void**)StudentArr,destroyStudent);
            return NULL;
        }
        fscanf(f,"%10s %d %lf ",StudentArr[i]->id,&StudentArr[i]->totalCredits,&StudentArr[i]->gpa); //scan the data you can with fscanf
        int restOfLineLength = lineLength(f);
        char buffer[restOfLineLength+1];
        if(fgets(buffer,restOfLineLength,f)==NULL)
        { 
            printf("error in fgets");
            destroyArr((void**)StudentArr,destroyStudent);
            return NULL;
        }
        StudentArr[i]->name = strtok(buffer," "); //print the rest of the line to each buffer in their order in the file
        StudentArr[i]->surName = strtok(NULL," ");
        StudentArr[i]->city = strtok(NULL," ");
        StudentArr[i]->department = strtok(NULL,"\n"); //end of line so the delimeter is the end
        if(StudentArr[i]->city==NULL||StudentArr[i]->name==NULL||StudentArr[i]->surName==NULL||StudentArr[i]->department==NULL)
        { //if one of the params is baad delete all the allocated memory
            destroyArr((void**)StudentArr,destroyStudent);
            return NULL;
        }       
        int j =0;
        StudentArr[i]->hacker=NULL; //initializing the hacker, 
        while(hackerArr[j]!=NULL) //search if the student is also a hacker if so put a pointer to the hacker
        {
            if(strcmp(StudentArr[i]->id,hackerArr[j]->id)==0){
                StudentArr[i]->hacker=hackerArr[j];
            }
            j++;
        }
    }
    StudentArr[numOfLines]=NULL; //put the last element in the arr as a null to know it is the end
    return StudentArr;

}

coursePtr* inputCourseFile(FILE* f)
{
    int numOflines = linesInFile(f);
    coursePtr* courseArr = (coursePtr*)malloc(sizeof(coursePtr)*(numOflines+1)); //create the course array
    if(courseArr==NULL) //if malloc didnt succeed exit the function
    {
        return NULL;
    }
    for(int i= 0;i<=numOflines;i++) //loop through all the lines of the file to input the data into the course array
    {
        courseArr[i]= (coursePtr)malloc(sizeof(Course)); //create each student
        if(courseArr[i]==NULL) //if malloc fails delete all previously created students
        {
            destroyArr((void*)courseArr,destroyCourse);
            return NULL;
        }
        fscanf(f,"%d %d\n",&courseArr[i]->courseNumber,&courseArr[i]->courseSize); //scan the static data into the struct
        courseArr[i]->queue = IsraeliQueueCreate(NULL,NULL,FRIENDSHIP_TH,ENEMY_TH); //create the israeliQueue
        
        bool flag = IsraeliQueueAddFriendshipMeasure(courseArr[i]->queue,friendshipValueById)!=ISRAELIQUEUE_SUCCESS;
        flag &= IsraeliQueueAddFriendshipMeasure(courseArr[i]->queue,friendshipValueByHackerFile)!=ISRAELIQUEUE_SUCCESS;
        flag &= IsraeliQueueAddFriendshipMeasure(courseArr[i]->queue,friendshipValueByASCII)!=ISRAELIQUEUE_SUCCESS;
        if(!flag) //add the wanted frienship functions if error then malloc failed and delete the whole data struct
        {
            destroyArr((void**)courseArr,destroyCourse);
            return NULL;
        }
    }
    courseArr[numOflines]=NULL; //put in the last element in the array null to indicate end of arr
    return courseArr;
}

hackerPtr* inputHackerFile(FILE *f)
{
    int numOfLines = linesInFile(f);
    if(numOfLines%4!=0) //each four lines is 1 struct so checking to see if there are 4*n lines in file
    {
        return NULL;
    }
    hackerPtr* hackerArr = (hackerPtr*)malloc(sizeof(hackerPtr)*(numOfLines/4+1)); //creating the struct and checking if worked
    if(hackerArr==NULL) //exit function if alloc didnt work
    {
        return NULL;
    }
    for(int i=0;i<numOfLines;i++) //iterating and creating the structs and putting them inside the array
    {
        hackerArr[i]= (hackerPtr)malloc(sizeof(Hacker));
        if(hackerArr[i] == NULL){
            destroyArr((void**)hackerArr,destroyHacker);
            return NULL;
        }
        
        //change this part to be less repetetive
        fscanf(f,"%10s\n",hackerArr[i]->id);
        hackerArr[i]->desiredCourses=splitLineToArrStrings(f); //putting the currents hacker's dynamic data, splitLineToArrStrings contains malloc
        hackerArr[i]->friendsId=splitLineToArrStrings(f);
        hackerArr[i]->enemiesId=splitLineToArrStrings(f);
        if(hackerArr[i]->friendsId==NULL||hackerArr[i]->enemiesId==NULL||hackerArr[i]->desiredCourses==NULL) //checking if memalloc succeeded, if not free the array
        {
            destroyArr((void**)hackerArr,destroyHacker);
            return NULL;
        }   
        hackerArr[i]->coursesHacked=2; //initializing this to 2 so that when it reaches 0 i know he is ok
    }
    hackerArr[numOfLines]=NULL; //the last element should be null to indicate end of array
    return hackerArr; 
}

int lineLength(FILE *f)
{
    int i=0;
    long position = ftell(f); //saving the initial position of the file pointer
    char c;
    while((c =fgetc(f))!='\n'||c!=EOF) //while we are still int the line 
    {
        i++;
    }
    fseek(f, position, SEEK_SET); //returning the positin to its initial position
    return i;
}

int numOfElementsInLine(FILE *f)
{
    int i=0;
    long position = ftell(f); //saving the initial position of the file pointer
    char c;
    while((c =fgetc(f))!='\n'||c!=EOF) //while not at the end of the line or the end of the file
    {
    if(c==' ') // counting is space is encountered
    {
        i++;
    }
   
    }
     fseek(f,position,SEEK_SET); //setting the file pointer back
    return i;
}

int linesInFile(FILE* f)
{
    long position = ftell(f); //saving the initial position of the file pointer
    int i = 0;
    char c=fgetc(f); 
    while(c!=EOF)
    {
        if(c=='\n') //every time reached the new line charachter add 1
        {
            i++;
        }
        c= fgetc(f);
    }
    fseek(f,position,SEEK_SET); //setting the file pointer back
    return i;
    
}

char** splitLineToArrStrings(FILE *f)
{
    //this functions utilizes the other functions to create an array of strings
    int numOfValueInLine = numOfElementsInLine(f);
    int lengthOfLine = lineLength(f);
    char buffer[lengthOfLine+1];
    fgets(buffer,lengthOfLine+1,f);
    char** arr = malloc((numOfValueInLine+1)*sizeof(char*));//creating the array with 1 more space to have space for the last element
    if(arr==NULL){
        return NULL;
    }
    char* token = strtok(buffer, " "); //creating the strtok
    for(int i=0;i<numOfValueInLine-1;i++) //iterating the inputs into the arr up until ine element before the end
    {
        arr[i] = token;
        token = strtok(NULL, " ");
    }
    arr[numOfValueInLine-1]=token;
    arr[numOfValueInLine]=NULL;
    return arr;
}

int friendshipValueByHackerFile(void* paramHacker,void* paramStudent)
{
    studentPtr hacker =(studentPtr)paramHacker; //creating the pointers
    studentPtr student =(studentPtr)paramStudent;
    
    int i = 0;
    while(hacker->hacker->friendsId[i]!=NULL) //checking if friends
    {
        if(strcmp(student->id,hacker->hacker->friendsId[i])==0)
        {
            return 20;
        } 
        i++;
    }
    int j =0;
    while(hacker->hacker->enemiesId[j]!=NULL) //checking if enemies
    {
        if(strcmp(student->id,hacker->hacker->enemiesId[j])==0)
        {
            return -20;
        }
        j++;
    }
    return 0; //returning 0 if neither freinds and enemies
}

int friendshipValueById(void* hackerId,void* studentId)
{
    studentPtr hacker = (studentPtr)hackerId; //creating the pointers
    studentPtr student = (studentPtr)studentId;

        int hackerValue = atoi(hacker->id); //turning values to int
        int studentValue = atoi(student->id);
        if(hackerValue-studentValue>0) //returning the absolute values
        {
           return hackerValue-studentValue; 
        }
        return studentValue-hackerValue;
}

int friendshipValueByASCII(void* hackerParam,void* studentParam)
{
    studentPtr hacker = (studentPtr)hackerParam; //creating the pointers
    studentPtr student = (studentPtr)studentParam;
    int value=0;
    value=nameDifference(hacker->name,student->name); //nameDifference returns the difference in values in two names
    value+=nameDifference(hacker->surName,student->surName);
    return value;
}

void changeCapitalLetter(EnrollmentSystem sys)
{   
    if(sys == NULL){
        return;
    }
    studentPtr* arr = sys->studentArr;
    for(int i = 0 ; arr[i] != NULL ; i++){
        for(int j = 0 ; arr[i]->name[j] != '\0' ; j++){
            if(arr[i]->name[j] >= 65 ||arr[i]->name[j] <=90){
                arr[i]->name[j] += 32;
            }
        }
        for(int j = 0 ; arr[i]->surName[j] != '\0' ; j++){
            if(arr[i]->surName[j] >= 65 ||arr[i]->surName[j] <=90){
                arr[i]->surName[j] += 32;
            }
        }
    }
}

int nameDifference(char* firstName, char* secondName)
{
    int sum=0;
    int j =0;

    while(firstName[j]!='\0'&&secondName[j]!='\0') //looping to check for each letter the difference
    { 
        if(firstName[j] > secondName[j]){
            sum+=firstName[j] - secondName[j];
        }
        else{
             sum+=secondName[j] - firstName[j];
        }
        j++;
    }
    if(strlen(firstName)==strlen(secondName)) //as specified
    {
        return sum;
    }
    char* name = (strlen(firstName)>strlen(secondName)) ? firstName : secondName;     
    while(name[j]!='\0'){
        sum+=name[j];
        j++;
    }
    return sum;
}

void hackEnrollment(EnrollmentSystem sys, FILE* out) //need to iterate here over student and not hacker arr
{
    if(sys==NULL||out==NULL){
        return;
    }
    for(int i=0;sys->courseArr[i]!=NULL;i++) //looping through the courses to enter the hackers to the courses
    {
       for(int j=0;sys->studentArr[j]!=NULL;j++)
       {
           if(sys->studentArr[j]->hacker!=NULL) //if it is a hacker check if he wanted to enter to the course
           {
               for(int k=0;sys->studentArr[j]->hacker->desiredCourses[k]!=NULL;k++)
               {
                   if(atoi(sys->studentArr[j]->hacker->desiredCourses[k])==sys->courseArr[i]->courseNumber) //if hacker wanted to enter course
                   {
                       if(IsraeliQueueEnqueue(sys->courseArr[i]->queue,sys->studentArr[j])!=ISRAELIQUEUE_SUCCESS) //putting the hackers inside the course
                       {
                           return;
                       }
                   }
               }
           }
       }
    }

    //in this part we will print to out file
    FILE* tempFile=fopen("temporaryFile","w");
    for(int l=0; sys->courseArr[l] !=NULL;l++) //[rinting to temp file the hackers and if hacker then desiredcourses--
    {
        int sizeOfCourse =sys->courseArr[l]->courseSize; //initializing a variable to know if hackers got in 
        fprintf(tempFile,"%d",sys->courseArr[l]->courseNumber);
        bool flag = true;
        while(flag)
        {
            studentPtr studentDequeued = (studentPtr)IsraeliQueueDequeue(sys->courseArr[l]->queue);
            if(studentDequeued==NULL) //if end of line
            {
                fprintf(tempFile,"\n"); //print end of line
                flag = false;
            }
            else
            {
                sizeOfCourse--;
                if(studentDequeued->hacker!=NULL&&sizeOfCourse>0) //if student is hacker and got into the course
                {
                    studentDequeued->hacker->coursesHacked--;
                }
                fprintf(tempFile," %10s",studentDequeued->id); //print to the temp file
            }
        }
    }
    fclose(tempFile);
    for(int g=0;sys->hackerArr[g]!=NULL;g++) //if one hacker didnt get in to two course print this to out
    {
        if(sys->hackerArr[g]->coursesHacked>0){        
            fprintf(out,"Cannot satisfy constraints for %s",sys->hackerArr[g]->id);
        }
    }
    FILE* readTemp =fopen("temporaryFile","r");
    char c = fgetc(readTemp);
    while(c!=EOF)
    {
        fputc(c,out);
        c=fgetc(readTemp);
    }
}
