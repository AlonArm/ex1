#include "HackerEnrollment.h"
#include <stdlib.h> 
#include <string.h>    

int friendsByHackerFile(void*, void*);
int friendsByName(void*, void*);
int friendByID(void*, void*);
int nameDiff(char*, char*);
int compareStudents(void*, void*);

char* readLine(FILE*);
char* getWord(char*);
bool isEmpty(FILE*);

struct Student* createStudent(char*);
struct Course* createCourse(char*);
struct Hacker* createHacker(char*, char*, char*);

void destroyList(struct Node*, void(void*));
void destroyStudent(void*);
void destroyCourse(void*);
void destroyHacker(void*);
void destroyNodes(void*);

struct Student* findStudent(struct Node*, char*);
struct Course* findCourse(struct Node*, char*);

int posInQueue(IsraeliQueue, void*);

int friendsByHackerFile(void* hacker, void* student){
    if(hacker == NULL || student == NULL) return 0;
    struct Student* hr = (struct Student*)hacker;
    if(hr->hackerInfo == NULL) return 0;
    char* studentID = ((struct Student*)student)->ID;
    
    Node* temp = hr->hackerInfo->friends;
    while(temp != NULL){
        if(strcmp((char*)(temp->data), studentID) == 0){
            return 20;
        }
        temp = temp->next;
    }
    temp = hr->hackerInfo->rivals;
    while(temp != NULL){
        if(strcmp((char*)(temp->data), studentID) == 0){
            return -20;
        }
        temp = temp->next;
    }
    return 0;
}
int friendsByName(void* hr, void* st){
    if(hr == NULL || st == NULL) return 0;
    struct Student* hacker = (struct Student*)hr;
    if(hacker->hackerInfo == NULL) return 0;
    struct Student* student = (struct Student*)st;
    return nameDiff(hacker->name, student->name)+nameDiff(hacker->surName, student->surName);
}
int nameDiff(char* name1, char* name2){
    int sum = 0;
    int i = 0;
    while(name1[i] != '\0' && name2[i] != '\0'){
        if(name1[i] > name2[i]) sum += name1[i] - name2[i];
        else sum += name2[i] - name1[i];
        i++;
    }
    char* name = (name1[i] != '\0') ? name1 : name2;
    while(name[i] != '\0'){
        sum += name[i];
        i++;
    }
    return sum;
}
int friendByID(void* hr, void* st){
    if(hr == NULL || st == NULL || ((struct Student*)hr)->hackerInfo == NULL) return 0;
    int id1 = atoi(((struct Student*)hr)->ID);
    int id2 = atoi(((struct Student*)st)->ID);
    if(id2 > id1) return id2 - id1;
    return id1 - id2;
}
int compareStudents(void* st1, void* st2){
    if(st1 == NULL || st2 == NULL) return 0;
    if(strcmp(((struct Student*)st1)->ID, ((struct Student*)st2)->ID) == 0){
        return 1;
    }
    return 0;
}

char* readLine(FILE* file){
    if(file == NULL) return NULL;
    long pos = ftell(file);
    int counter = 0;
    char c;
    do{
        c = fgetc(file);
        counter++;
    }while(c != '\n' && c != EOF);
    fseek(file, pos, SEEK_SET);
    char* line = (char*)malloc(sizeof(char) * (counter + 1));
    if(line == NULL) return NULL;
    char* temp = fgets(line, counter + 1, file);
    if(temp == NULL){
        free(line);
        return NULL;
    }
    line[counter - 1] = '\0';
    return line;
}
char* getWord(char* line){
    if(line == NULL || line[0] == '\0') return NULL;
    int i = 0;
    while(line[i] != '\0' && line[i] != ' ') i++;
    if(i == 0) return NULL;
    line[i] = '\0';
    char* newWord = (char*)malloc((i + 1) * sizeof(char));
    if(newWord == NULL) return NULL;
    for(int j = 0 ; j <= i ; j++) newWord[j] = line[j];
    int j = 0;
    while(line[j + i + 1] != '\0'){
        line[j] = line[j + i + 1];
        j++;
    }
    while(i >= 0){
        line[j + i] = '\0';
        i--;
    }
    return newWord;
}
bool isEmpty(FILE* file){
    if(file == NULL) return true;
    long pos = ftell(file);
    bool isEmpty = fgetc(file) == EOF;
    fseek(file, pos, SEEK_SET);
    return isEmpty;
}
struct Student* createStudent(char* info){
    if(info == NULL) return NULL;
    struct Student* st = (struct Student*)malloc(sizeof(struct Student));
    if(st == NULL) return NULL;
    st->ID = getWord(info);
    st->totalCredits = getWord(info);
    st->GPA = getWord(info);
    st->name = getWord(info);
    st->surName = getWord(info);
    st->city = getWord(info);
    st->department = getWord(info);
    st->hackerInfo = NULL;
    return st;
}
struct Course* createCourse(char* info){
    if(info == NULL) return NULL;
    struct Course* course = (struct Course*)malloc(sizeof(struct Course));
    if(course == NULL) return NULL;
    FriendshipFunction functions[4] = {friendsByHackerFile, friendsByName, friendByID, NULL};
    course->queue = IsraeliQueueCreate(functions, compareStudents, FRIENDSHIP_TH, ENEMY_TH);
    course->courseNum = getWord(info);
    char* size = getWord(info);
    course->size = atoi(size);
    free(size);
    return course;
}
struct Hacker* createHacker(char* courses, char* friends, char* rivals){
    struct Hacker* hacker = (struct Hacker*)malloc(sizeof(struct Hacker));
    if(hacker == NULL) return NULL;
    hacker->desiredCourses = NULL;
    hacker->friends = NULL;
    hacker->rivals = NULL;
    char* tempWord = getWord(courses);
    while(tempWord != NULL){
        struct Node* addedCourse = (struct Node*)malloc(sizeof(struct Node));
        if(addedCourse == NULL){
            destroyHacker(hacker);
            free(tempWord);
            return NULL;
        }
        addedCourse->data = tempWord;
        addedCourse->next = hacker->desiredCourses;
        hacker->desiredCourses = addedCourse;
        tempWord = getWord(courses);
    }
    tempWord = getWord(friends);
    while(tempWord != NULL){
        struct Node* addedFriend = (struct Node*)malloc(sizeof(struct Node));
        if(addedFriend == NULL){
            destroyHacker(hacker);
            free(tempWord);
            return NULL;
        }
        addedFriend->data = tempWord;
        addedFriend->next = hacker->friends;
        hacker->friends = addedFriend;
        tempWord = getWord(friends);
    }
    tempWord = getWord(rivals);
    while(tempWord != NULL){
        struct Node* addedRival = (struct Node*)malloc(sizeof(struct Node));
        if(addedRival == NULL){
            destroyHacker(hacker);
            free(tempWord);
            return NULL;
        }
        addedRival->data = tempWord;
        addedRival->next = hacker->rivals;
        hacker->rivals = addedRival;
        tempWord = getWord(rivals);
    }
    return hacker;
}

void destroyStudent(void* student){
    if(student == NULL) return;
    struct Student* st = (struct Student*)student;
    free(st->ID);
    free(st->totalCredits);
    free(st->GPA);
    free(st->name);
    free(st->surName);
    free(st->city);
    free(st->department);
    if(st->hackerInfo != NULL) destroyHacker(st->hackerInfo);
    free(st);
}
void destroyCourse(void* course){
    if(course == NULL) return;
    struct Course* cs = (struct Course*)course;
    free(cs->courseNum);
    IsraeliQueueDestroy(cs->queue);
    free(cs);
}
void destroyHacker(void* hacker){
    if(hacker == NULL) return;
    struct Hacker* hr = (struct Hacker*)hacker;
    destroyList(hr->desiredCourses, free);
    destroyList(hr->friends, free);
    destroyList(hr->rivals, free);
    free(hacker);
}
void destroyList(struct Node* list, void(*destroyFunc)(void*)){
    if(list == NULL) return;
    (destroyFunc)(list->data);
    destroyList(list->next, destroyFunc);
    free(list);
}
void destroyNodes(void* node){
    if(node == NULL) return;
    struct Node* temp = (struct Node*)node;
    destroyNodes(temp->next);
    free(node);
}
void destroySystem(EnrollmentSystem sys){
    destroyList(sys->studentsList, destroyStudent);
    destroyList(sys->coursesList, destroyCourse);
    destroyNodes(sys->hackers);
    free(sys);
}

void changeBigLetters(EnrollmentSystem sys){
    if(sys == NULL) return;
    struct Node* iterator = sys->studentsList;
    while(iterator != NULL){
        char* name = ((struct Student*)iterator->data)->name;
        for(int i = 0 ; name[i] != '\0' ; i++){
            if(name[i] >= 'A' && name[i] <= 'Z') name[i] += 'a'-'A';
        }
        char* surname = ((struct Student*)iterator->data)->surName;
        for(int i = 0 ; surname[i] != '\0' ; i++){
            if(surname[i] >= 'A' && surname[i] <= 'Z') surname[i] += 'a' - 'A';
        }
        iterator = iterator->next;
    }
}
int posInQueue(IsraeliQueue queue, void* item){
    if(queue == NULL || item == NULL) return -1;
    IsraeliQueue tempQueue = IsraeliQueueClone(queue);
    int pos = 0;
    while(IsraeliQueueSize(tempQueue) > 0){
        void* tempItem = IsraeliQueueDequeue(tempQueue);
        if(compareStudents(item, tempItem)){
            IsraeliQueueDequeue(tempQueue);
            IsraeliQueueDestroy(tempQueue);
            return pos;
        }
        pos++;
    }
    IsraeliQueueDestroy(tempQueue);
    return -1;
}

struct Student* findStudent(struct Node* studentList, char* ID){
    if(ID == NULL || studentList == NULL) return NULL;
    if(strcmp(ID, ((struct Student*)studentList->data)->ID) == 0){
        return (struct Student*)(studentList->data);
    }
    return findStudent(studentList->next, ID);
}
struct Course* findCourse(struct Node* coursesList, char* courseNum){
    if(courseNum == NULL || coursesList == NULL) return NULL;
    if(strcmp(courseNum, ((struct Course*)coursesList->data)->courseNum) == 0){
        return (struct Course*)(coursesList->data);
    }
    return findCourse(coursesList->next, courseNum);
}
EnrollmentSystem createEnrollment(FILE* students, FILE* courses, FILE* hackers){
    if(students == NULL || courses == NULL || hackers == NULL) return NULL;
    EnrollmentSystem system = (EnrollmentSystem)malloc(sizeof(struct EnrollmentSystem_t));
    if(system == NULL) return NULL;
    system->coursesList = NULL;
    system->hackers = NULL;
    system->studentsList = NULL;
    while(!isEmpty(students)){
        char* line = readLine(students);
        struct Student* newStudent = createStudent(line);
        free(line);
        struct Node* temp = (struct Node*)malloc(sizeof(struct Node));
        if(temp == NULL || newStudent == NULL){
            destroySystem(system);
            destroyStudent(newStudent);
            free(temp);
            return NULL;
        }
        temp->data = newStudent;
        temp->next = system->studentsList;
        system->studentsList = temp;
    }
    while(!isEmpty(courses)){
        char* line = readLine(courses);
        struct Course* newCourse = createCourse(line);
        free(line);
        struct Node* temp = (struct Node*)malloc(sizeof(struct Node));
        if(temp == NULL || newCourse == NULL){
            destroySystem(system);
            destroyCourse(newCourse);
            free(temp);
            return NULL;
        }
        temp->data = newCourse;
        temp->next = system->coursesList;
        system->coursesList = temp;
    }
    while(!isEmpty(hackers)){
        char* idLine = readLine(hackers);
        struct Student* student = findStudent(system->studentsList, idLine);
        free(idLine);
        char* desiredCourses = readLine(hackers);
        char* friends = readLine(hackers);
        char* rivals = readLine(hackers);
        struct Hacker* newHacker = createHacker(desiredCourses, friends, rivals);
        free(desiredCourses);
        free(friends);
        free(rivals);
        if(newHacker == NULL || student == NULL){
            destroySystem(system);
            destroyHacker(newHacker);
            return NULL;
        }
        if(system->hackers == NULL){
            system->hackers = (struct Node*)malloc(sizeof(struct Node));
            if(system->hackers == NULL){
                destroySystem(system);
                destroyHacker(newHacker);
                return NULL;
            }
            system->hackers->data = student;
            system->hackers->next = NULL;
        }
        else{
            struct Node* iterator = system->hackers;
            while(iterator->next != NULL) iterator = iterator->next;
            iterator->next = (struct Node*)malloc(sizeof(struct Node));
            if(iterator->next == NULL){
                destroySystem(system);
                destroyHacker(newHacker);
                return NULL;
            }
            iterator->next->data = student;
            iterator->next->next = NULL;
        }
        student->hackerInfo = newHacker;
    }
    return system;
}
EnrollmentSystem readEnrollment(EnrollmentSystem sys, FILE* queues){
    if(sys == NULL || queues == NULL) return NULL;
    while(!isEmpty(queues)){
        char* line = readLine(queues);
        if(line == NULL) return NULL;
        char* courseNum = getWord(line);
        struct Course* course = findCourse(sys->coursesList, courseNum);
        free(courseNum);
        if(course == NULL){
            free(line);
            return NULL;
        }
        char* studentID = getWord(line);
        while(studentID != NULL){
            struct Student* student = findStudent(sys->studentsList, studentID);
            if(student == NULL){
                free(studentID);
                free(line);
                return NULL;
            }
            if(!IsraeliQueueContains(course->queue, student)){
                IsraeliQueueEnqueue(course->queue, student);
            }
            free(studentID);
            studentID = getWord(line);
        }
        free(line);
    }
    return sys;
}
void hackEnrollment(EnrollmentSystem sys, FILE* out){
    if(sys == NULL || out == NULL) return;
    struct Node* iterator = sys->hackers;
    while(iterator != NULL){
        struct Student* st = (struct Student*)(iterator->data);
        struct Node* courseI = st->hackerInfo->desiredCourses;
        while(courseI != NULL){
            struct Course* course = findCourse(sys->coursesList, (char*)(courseI->data));
            if(course == NULL) return;
            IsraeliQueueEnqueue(course->queue, st);
            courseI = courseI->next;
        }
        iterator = iterator->next;
    }
    iterator = sys->hackers;
    while(iterator != NULL){
        struct Student* tmp = (struct Student*)iterator->data;
        struct Node* desiredCourses = tmp->hackerInfo->desiredCourses;
        int acceptedCount = 0;
        int rejectedCount = 0;
        while(desiredCourses != NULL){
            struct Course* course = findCourse(sys->coursesList, (char*)desiredCourses->data);
            int pos = posInQueue(course->queue, tmp);
            int size = course->size;
            if(pos < size) acceptedCount++;
            else rejectedCount++;
            desiredCourses = desiredCourses->next;
        }
        if(acceptedCount < 2 && rejectedCount > 0){
            fprintf(out, "Cannot satisfy constraints for %s\n", ((struct Student*)iterator->data)->ID);
            return;
        }
        iterator = iterator->next;
    }
    iterator = sys->coursesList;
    while(iterator != NULL){
        struct Course* course = (struct Course*)(iterator->data);
        if(IsraeliQueueSize(course->queue) > 0){
            fprintf(out, course->courseNum);
            struct Student* queueHead = (struct Student*)IsraeliQueueDequeue(course->queue);
            while(queueHead != NULL){
                fprintf(out, " %s", queueHead->ID);
                queueHead = (struct Student*)IsraeliQueueDequeue(course->queue);
            }
            fprintf(out, "\n");
        }
        iterator = iterator->next;
    }
}

void printSystem(EnrollmentSystem sys){
    if(sys == NULL){
        printf("NULL\n");
        return;
    }
    printf("courses:\n");
    struct Node* temp = sys->coursesList;
    while(temp != NULL){
        struct Course* course = (struct Course*)(temp->data);
        printf("course number: %s, course size %d\n", course->courseNum, course->size);
        temp = temp->next;
    }
    printf("\nstudents:\n");
    temp = sys->studentsList;
    while(temp != NULL){
        struct Student* student = (struct Student*)(temp->data);
        printf("ID: %s, credits: %s, GPA: %s, name: %s, surname: %s, city: %s, department: %s\n",
        student->ID, student->totalCredits, student->GPA, student ->name,
        student->surName, student->city, student->department);
        struct Hacker* hackerInfo = student->hackerInfo;
        if(hackerInfo == NULL) printf("not a hacker\n");
        else{
            printf("desired courses: ");
            struct Node* temp1 = hackerInfo->desiredCourses;
            while(temp1 != NULL){
                printf("%s, ", (char*)(temp1->data));
                temp1 = temp1->next;
            }
            printf("\nfriends: ");
            temp1 = hackerInfo->friends;
            while(temp1 != NULL){
                printf("%s, ", (char*)(temp1->data));
                temp1 = temp1->next;
            }
            printf("\nrivals: ");
            temp1 = hackerInfo->rivals;
            while(temp1 != NULL){
                printf("%s, ", (char*)(temp1->data));
                temp1 = temp1->next;
            }
            printf("\n");
        }
        temp = temp->next;
    }
}