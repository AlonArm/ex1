#include "HackerEnrollment.h"
#include <stdlib.h> 
#include <string.h>    

listPtr createList() 
{
    listPtr list = (listPtr)malloc(sizeof(List));
    if (list == NULL) 
    {
        return NULL; // failed to allocate memory for list
    }
    list->head = createNewNode(NULL);
    return list;
}
nodePtr createNewNode(void* data) 
{
    nodePtr newNode = (nodePtr) malloc(sizeof(Node));
    if(newNode==NULL)
    {
        return NULL;
    }
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}
HackerEnrollmentError destroyList(listPtr list)
{
    if(list==NULL)
    {
        return HackerEnrollment_BAD_PARAM;
    }
    nodePtr current = list->head;
   while (current != NULL)
    {
      nodePtr temp = current;
      current = current->next;
      free(temp);
    }
    free(list);
    return HackerEnrollment_SUCCESS;
}

HackerEnrollmentError addAnotherNode(listPtr list, void* data) 
{
    if(list==NULL||data==NULL)
    {
        return HackerEnrollment_BAD_PARAM;
    }
   nodePtr currentNode =  list->head ;
    while (currentNode->next != NULL) 
    {
        currentNode = currentNode->next;
    }
    nodePtr newNode = createNewNode(data);
    currentNode->next = newNode;
    return HackerEnrollment_SUCCESS;
}

HackerEnrollmentError readString(FILE *f, char** word)
{
    int i = 0;
 if(f==NULL||fgetc(f)==EOF)
 {
     return HackerEnrollment_BAD_PARAM;
 }   
 *word = malloc(sizeof(char)*(i+2));
 if(*word==NULL)
 {
     return HackerEnrollment_ALLOC_FAILED;
 }
 (*word)[0]='\0';

char temp = fgetc(f);
while(temp!=EOF&&temp!=' '&&temp!='\n')
{
    (*word)[i]=temp;
    *word = (char*)realloc(*word,sizeof(char)*(i+2));
    if(*word==NULL)
    {
        free(*word);
        return HackerEnrollment_BAD_PARAM;
    }
    i++;
}

word[i]='\0';
return HackerEnrollment_SUCCESS;
}

HackerEnrollmentError destroyStudent(studentPtr student)
{
    if(student==NULL)
    {
        return HackerEnrollment_BAD_PARAM;
    }

    free(student->city);
    free(student->name);
    free(student->surName);
    free(student->department);
    return HackerEnrollment_SUCCESS;
}

HackerEnrollmentError destroyCourse(coursePtr course)
{
    if(course==NULL)
    {
        return HackerEnrollment_BAD_PARAM;
    }
    free(course);
    return HackerEnrollment_SUCCESS;
}

HackerEnrollmentError destroyHacker(hackerPtr hacker)
{
    if(hacker==NULL)
    {
        return HackerEnrollment_BAD_PARAM;
    }
    destroyList(hacker->desiredCourses);
    destroyList(hacker->friendsId);
    destroyList(hacker->enemiesId);
    free(hacker);
    return HackerEnrollment_SUCCESS;
}

HackerEnrollmentError destroyStructList(listPtr list, HackerEnrollmentError(*deleteStruct)(void*))
{

    if(list==NULL||deleteStruct==NULL)
    {
        return HackerEnrollment_BAD_PARAM;
    }
    nodePtr temp = list->head;
    while(temp!=NULL)
    {
        deleteStruct(temp->data);
        temp = temp->next;    
    }
    destroyList(list);
    return HackerEnrollment_SUCCESS;
}

/*char* readWholeNumber(FILE *f) //im not very sure this function is necesseray as it also uses malloc
{
    char stringReturn[11];
    stringReturn[10]='\0';
    char temp=fgetc(f);
    int i = 0;
    while(temp!=EOF&&temp!=' '&&temp!='\n')
    {
        stringReturn[i]=temp;

        i++;

    }        
    stringReturn[i+1]='\0';
    return strdup(stringReturn);
}
*/
