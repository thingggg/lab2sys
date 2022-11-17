//
// Created by oli_d on 11/16/2022.
//

#ifndef UNTITLED3_OBJET_H
#define UNTITLED3_OBJET_H


#include "main.cpp"
#include "semaphore.h"

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
    }
#undef PROCESS_VAL

    return out << s;
}
enum ElemState {AWAITING, FREE, ALLOCATED, NOT_REQUESTED};
std::ostream& operator<<(std::ostream& out, const ElemState value){
    const char* s = 0;
#define PROCESS_VAL(p, n) case(p): s = #n;  break;
    switch(value){
        PROCESS_VAL(AWAITING, "attente de ");
        PROCESS_VAL(FREE, "Liberé ");
        PROCESS_VAL(ALLOCATED, "alloué ");
        PROCESS_VAL(NOT_REQUESTED, "pas de requete");
    }
#undef PROCESS_VAL

    return out << s;
}
enum State {INITIATING, BLOCKED, COMPLETED};
std::ostream& operator<<(std::ostream& out, const State value){
    const char* s = 0;
#define PROCESS_VAL(p) case(p): s = #p; break;
    switch(value){
        PROCESS_VAL(INITIATING);
        PROCESS_VAL(BLOCKED);
        PROCESS_VAL(COMPLETED);
    }
#undef PROCESS_VAL

    return out << s;
}
enum statusrow {threadId,threadobjet ,objetId , threadstatus, statuse1 , element1, statuse2, element2};
enum Type{ COUPE_DE_VIN, PORTE, TABLE, BAGUE, CHANDELIER, EPEE };
std::ostream& operator<<(std::ostream& out, const Type value){
    const char* s = 0;
#define PROCESS_VAL(p) case(p): s = #p; break;
    switch(value){
        PROCESS_VAL(COUPE_DE_VIN);
        PROCESS_VAL(PORTE);
        PROCESS_VAL(TABLE);
        PROCESS_VAL(BAGUE);
        PROCESS_VAL(CHANDELIER);
        PROCESS_VAL(EPEE);
    }
#undef PROCESS_VAL

    return out << s;
}
using namespace  std;
struct objet{
public:
    chrono::system_clock::time_point begin;
    chrono::system_clock::time_point end;
    char * beginTime;
    char * endTime;
    std::time_t end_time;
    State state;
    element ressource[2];
    ElemState elemState[2] = {NOT_REQUESTED, NOT_REQUESTED};
    Type type;
    string status[10] = {" nan ", " nan ", " nan "," nan ", " nan "," nan ", " nan "," nan "};
    int InstanceId = 0;
    objet(Type t , const element (&elements)[2]) {
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
    void startClock(){
        begin = chrono::system_clock::now();
        beginTime = Time(begin);
    }
    void stopClock(){
        end = chrono::system_clock::now();
        endTime = Time(end);
    }
    chrono::duration<double> timeElapsed(){
        return end-begin;
    }
    char* Time(chrono::system_clock::time_point t){
        std::time_t end_time = std::chrono::system_clock::to_time_t(t);
        return ctime(&end_time);
    }
    void displayTime(){

        cout        << "began computation at " << Time(begin)
                    << "finished computation at " << Time(end)
                    << "elapsed time: " << timeElapsed().count() << "s"
                     << endl;

    }
    void displayStatus(sem_t sem){
        //sem_wait(&sem);
        //for (auto &s: status) {
        //    cout << s;
        //}
        cout << type << state << ressource[0] << elemState[0] << ressource[1] << elemState[1] << beginTime<< endTime;
        cout << endl;
        //sem_post(&sem);
    }
};


#endif //UNTITLED3_OBJET_H
