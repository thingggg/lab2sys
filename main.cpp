#pragma once
#include <iostream>
#include <semaphore.h>
#include <thread>
#include <vector>
#include <csignal>
#include "pthread.h"
#include "objet.h"

#include <cstdlib>
#include <deque>
#include <queue>



class objet;
using namespace  std;
sem_t printSem;
sem_t threadsSem;
sem_t sem_elements[6] = {1,1,1,1,1,1};
sem_t sem_objet[6] = {1,1,1,1,1,1};
bool ressource[6] = {true,true,true,true,true,true};

thread threads[6];
deque<objet> instances;
deque<objet*> JobPool;

objet objets[6] = {
        objet(COUPE_DE_VIN,{Or, Argent}),
        objet(PORTE,{Bois, Or}),
        objet(TABLE, {Metal, Bois}),
        objet(BAGUE, {Diamants, Metal}),
        objet(CHANDELIER, {Fer, Diamants}),
        objet(EPEE, {Argent, Diamants})
};

bool allocate(objet &o){
    int i = o.type;
    sem_wait(&sem_objet[o.type]);
    if (sem_trywait(&sem_elements[o.ressource[0]])){
        o.elemState[0] = ALLOCATED;
    } else{
        o.elemState[0]= AWAITING;
        sem_wait(&sem_elements[o.ressource[0]]);
        o.elemState[0]= ALLOCATED;
    }
    if (sem_trywait(&sem_elements[o.ressource[1]])) {
        o.elemState[1] = ALLOCATED;
    } else {
        o.elemState[1]= AWAITING;
        sem_wait(&sem_elements[o.ressource[1]]);
        o.elemState[0]= ALLOCATED;
        //o.state = BLOCKED;
        //o.elemState[0]= FREE;
        //o.elemState[1]= FREE;
        //sem_post(&sem_elements[o.ressource[0]]);
        //sem_post(&sem_objet[o.type]);
        //return false;
    }

    return true;
}


void deAllocate(objet &o){
    sem_post(&sem_elements[o.ressource[0]]);
    o.elemState[0] = FREE;
    sem_post(&sem_elements[o.ressource[1]]);
    o.elemState[1] = FREE;
}
bool ObjectAlreadyInThread(objet * o){
    for(objet i : instances){
        if(i.status[threadobjet] == o->status[threadobjet]) return true;
    }
    return false;
}
void removeObject(objet * o){
    int i =0;
    for(objet a : instances){
        if(a.status[threadobjet] == o->status[threadobjet]){instances.erase(instances.begin() + i); break;}
        i++;
    }
}
class factory{
public:
    explicit factory() {
        while(!JobPool.empty()){
            sem_wait(&threadsSem);
            //while(ObjectAlreadyInThread(&JobPool.front())){
            //    JobPool.push_back(JobPool.front());
            //   JobPool.pop_front();
            //}
            //instances.push_front(JobPool.front());
            thread t = thread(make,JobPool.front());

            JobPool.pop_front();
            t.detach();
        }
        int value = 0;
        while(value != 6){
            sem_getvalue(&threadsSem, &value);
        }
    }
    static void make(objet * o){
        o->startClock();
        o->displayStatus(printSem);
        allocate(*o);

        o->displayStatus(printSem);
        std::this_thread::sleep_for(std::chrono::seconds (1));
        o->state = COMPLETED;
        deAllocate(*o);
        o->stopClock();
        //o.displayTime();
        o->displayStatus(printSem);
        //removeObject(&o);
        sem_post(&threadsSem);
        sem_post(&sem_objet[o->type]);
    }
};


int main() {
    vector<objet> queue;
    sem_init(&threadsSem, 0,6);
    sem_init(&printSem, 0, 1);
    for(sem_t &sem : sem_elements){
        sem_init(&sem, 0, 1);
    }
    for(sem_t &sem : sem_objet){
        sem_init(&sem, 0, 1);
    }
    for(int i = 0; i < 100; i++){
        for(const objet& o : objets){
            auto * temp = new objet(o.type, o.ressource);
            temp->setInstanceId(i);
            JobPool.emplace_back(temp);
        }
    }
    auto* f1 = new factory();

}
