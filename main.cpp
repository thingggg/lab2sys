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


bool ressource[6] = {true,true,true,true,true,true};

thread threads[6];
deque<objet> instances;
deque<objet*> JobPool;

objet objets[6] = {
        objet(COUPE_DE_VIN,{Or, Argent}, 0),
        objet(PORTE,{Bois, Or}, 1),
        objet(TABLE, {Metal, Bois}, 2),
        objet(BAGUE, {Diamants, Metal}, 3),
        objet(CHANDELIER, {Fer, Diamants}, 4),
        objet(EPEE, {Argent, Diamants}, 5)
};
class factory{
public:
    explicit factory() {
        for(int i = 0 ; i<600; i++) {
            sem_wait(&threadsSem);
            for (int i = 0; i < 6; i++) {
                if (sem_trywait(&sem_objet[i])) {
                    thread t = thread(&objet::make, objets[i]);
                    t.detach();
                    break;
                }
            }
        }

        int value = 0;
        while(value != 6){
            sem_getvalue(&threadsSem, &value);
        }
    }
    int buffer_print_index = 0;
//    void printStatus(){
//        // Verifier s'il y a un string a printer. P (consommateur_signal_sem);
//        sem_wait (&printSem);
//        printf ("%s", buf [buffer_print_index]);
//        //Exclusion mutuelle
//        sem_wait (&mutex_sem) ;
//        buffer_print_index++;
//        if (buffer_print_index == 6)
//            buffer_print_index = 0;
//        sem_post (&mutex_sem) ;
//        /* Contenu du buffer printer */
//        sprintf(buf[i])
//        sem_post (&buffer_count_sem) ;
//    }
//    objet make(objet &o){
//        sem_wait(&clockSem);
//        o.startClock();
//        sem_post(&clockSem);
//        //o->displayStatus(printSem);
//        while(!allocate(o)){
//        };
//
//        //o->displayStatus(printSem);
//        std::this_thread::sleep_for(std::chrono::seconds (1));
//        o.state = COMPLETED;
//        deAllocate(o);
//        sem_wait(&clockSem);
//        o.stopClock();
//        sem_post(&clockSem);
//        sem_wait(&printSem);
//        o.displayTime();
//        sem_post(&printSem);
//        //removeObject(&o);
//        sem_post(&sem_objet[o.type]);
//        sem_post(&threadsSem);
//
//    }
};


int main() {
    vector<objet> queue;
    sem_init(&threadsSem, 0,6);
    sem_init(&priority, 0,1);
    sem_init(&printSem, 0, 1);
    sem_init(&clockSem, 0, 1);
    for(sem_t &sem : sem_elements){
        sem_init(&sem, 0, 1);
    }
    for(sem_t &sem : sem_objet){
        sem_init(&sem, 0, 1);
    }
//    for(int i = 0; i < 100; i++){
//        for(const objet& o : objets){
//            auto * temp = new objet(o.type, o.ressource);
//            temp->setInstanceId(i);
//            JobPool.emplace_back(temp);
//        }
//    }
    auto* f1 = new factory();
    cout<< "program over";

}
