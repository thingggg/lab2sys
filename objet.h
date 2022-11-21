//
// Created by oli_d on 11/16/2022.
//

#ifndef UNTITLED3_OBJET_H
#define UNTITLED3_OBJET_H

#include <iomanip>
#include "main.cpp"
#include "semaphore.h"
#include "time.h"
sem_t priority;
sem_t printSem;
sem_t clockSem;
sem_t threadsSem;
sem_t sem_elements[6] = {1,1,1,1,1,1};
sem_t sem_objet[6] = {1,1,1,1,1,1};
char buf[6][100];

enum element {Or, Argent, Bois, Metal, Diamants, Fer};
std::ostream& operator<<(std::ostream& out, const element value){
    const char* s = 0;
#define PROCESS_VAL(p) case(p): s = #p; break;
    switch(value){
        PROCESS_VAL(Or);
        PROCESS_VAL(Argent);
        PROCESS_VAL(Bois);
        PROCESS_VAL(Metal);
        PROCESS_VAL(Diamants);
        PROCESS_VAL(Fer);
        default: break;
    }
#undef PROCESS_VAL

    return out << s;
}
enum ElemState {AWAITING, FREE, ALLOCATED, NOT_REQUESTED};
std::ostream& operator<<(std::ostream& out, const ElemState value){
    const char* s = 0;
#define PROCESS_VAL(p, n) case(p): s = #n;  break;
    switch(value){
        PROCESS_VAL(AWAITING,       | attente de : );
        PROCESS_VAL(FREE,           | Libère : );
        PROCESS_VAL(ALLOCATED,      |  possède :);
        PROCESS_VAL(NOT_REQUESTED,  | pas demandé :);
        default: s="nan"; break;
    }
#undef PROCESS_VAL

    return out << s;
}
enum State {INITIATING, BLOCKED, COMPLETED};
std::ostream& operator<<(std::ostream& out, const State value){
    const char* s = 0;
#define PROCESS_VAL(p, n) case(p): s = #n; break;
    switch(value){
        PROCESS_VAL(INITIATING, __initié__    );
        PROCESS_VAL(BLOCKED,    __bloqué__       );
        PROCESS_VAL(COMPLETED,  _complété_   );
        default: s="nan"; break;
    }
#undef PROCESS_VAL

    return out << s;
}
enum statusrow {threadId,threadobjet ,objetId , threadstatus, statuse1 , element1, statuse2, element2};
enum Type{ COUPE_DE_VIN, PORTE, TABLE, BAGUE, CHANDELIER, EPEE };
std::ostream& operator<<(std::ostream& out, const Type value){
    const char* s = 0;
#define PROCESS_VAL(p,n) case(p): s = #n; break;
    switch(value){
        PROCESS_VAL(COUPE_DE_VIN, |-Coup de vin-| );
        PROCESS_VAL(PORTE,        |----Porte----| );
        PROCESS_VAL(TABLE,        |----Table----| );
        PROCESS_VAL(BAGUE,        |----Bague----| );
        PROCESS_VAL(CHANDELIER,   |--Chandelier-| );
        PROCESS_VAL(EPEE,         |-----Épée----| );
        default: s="nan"; break;
    }
#undef PROCESS_VAL

    return out << s;
}
using namespace  std;

class objet{

public:
    thread::id id;
    int index = 0;
    time_t begin;
    time_t end;
    struct tm * beginTime;
    struct tm * endTime;
    State state;
    element ressource[2]{};
    ElemState elemState[2] = {NOT_REQUESTED, NOT_REQUESTED};
    Type type;
    string status[10] = {" nan ", " nan ", " nan "," nan ", " nan "," nan ", " nan "," nan "};
    int InstanceId = 0;
    objet(Type t , const element (&elements)[2], int id) {
        index = id;
        ressource[0] = elements[0];
        ressource[1] = elements[1];
        type = t;
        state = INITIATING;
        status[element1] = elements[0];
        status[element2] = elements[1];
        status[threadobjet] = t;

    }

    void setInstanceId(int i){
        InstanceId = i;
        status[objetId] = to_string(i);
    }
    void displayTime(){
        printf("%s to %s \n", asctime(beginTime), asctime(endTime));
    }
    void displayStatus(){

        sem_wait(&clockSem);    //attend que le buffer cout soit remplis (tout dans l'ordre)
        cout << id << type << state << elemState[0] << ressource[0]  << elemState[1] << ressource[1] << endl;
        sem_post(&clockSem);
        sem_post(&printSem);    //attend que le changement d'état soit afficher au terminal (évite de passer des messages)
    }
    bool allocate(){
        int i = type;
        if (sem_trywait(&sem_elements[ressource[0]])){
            elemState[0] = ALLOCATED;
        } else{
            state = BLOCKED;
            elemState[0]= AWAITING;
            displayStatus();
            sem_wait(&printSem);
            sem_wait(&priority);
            return false;
        }
        if (sem_trywait(&sem_elements[ressource[1]])) {
            state = INITIATING;
            elemState[1] = ALLOCATED;
        } else {
            state = BLOCKED;
            elemState[1]= AWAITING;
            sem_wait(&sem_elements[ressource[1]]);
            displayStatus();
            sem_wait(&printSem);
            sem_post(&priority);
            elemState[0]= ALLOCATED;

        }
        return true;
    }


    void deAllocate(){
        sem_post(&sem_elements[ressource[0]]);
        elemState[0] = FREE;
        sem_post(&sem_elements[ressource[1]]);
        elemState[1] = FREE;
    }
    void make(){
        id =this_thread::get_id();
        displayStatus();
        sem_wait(&printSem);
        while(!allocate()){
        };
        sleep(1);
        state = COMPLETED;
        deAllocate();
        displayStatus();
        sem_wait(&printSem);

        //removeObject(&o);
        sem_post(&sem_objet[type]);
        sem_post(&threadsSem);
    }
};


#endif //UNTITLED3_OBJET_H
