//
// Created by NitzanKarby on 01/04/2022.
//

#include "uthreads.h"
#include <iostream>
#include <vector>
#include <set>
using std::vector ;
using std::set ;
// Consts for the program
#define FAIL (-1)

#define THREAD_ERROR "thread library error: "
int quantum;
sigjmp_buf env[MAX_THREAD_NUM];
set<int> ids;


int get_thread_id();

int initialize_main_thread(){
    int id = get_thread_id();
    if (id == MAX_THREAD_NUM) {
        fprintf(stderr, "thread library error: maximum number of threads exceeded \n");
        return FAIL;
    }
    sigsetjmp(env[0], 1);

}

void initialize_ids_set(){
    // initialize properly later
    for (int i = 0; i <MAX_THREAD_NUM; ++i) {
        ids.insert(i);
    }
}

int uthread_init(int quantum_usecs) {
    if (quantum_usecs <= 0) return FAIL;
    quantum = quantum_usecs;
    initialize_ids_set();

    //initialize main thread

    return 0;
}

int uthread_spawn(thread_entry_point entry_point) {
    return 0;
}

int uthread_terminate(int tid) {
    return 0;
}

int uthread_block(int tid) {
    return 0;
}

int uthread_resume(int tid) {
    return 0;
}

int uthread_sleep(int num_quantums) {
    return 0;
}

int uthread_get_tid() {
    return 0;
}

int uthread_get_total_quantums() {
    return 0;
}

int uthread_get_quantums(int tid) {
    return 0;
}

int initialize_thread(thread_entry_point entry_point) {
    int cur_id = get_thread_id();
    if (cur_id == MAX_THREAD_NUM) {
        fprintf(stderr, "thread library error: maximum number of threads exceeded \n");
        return FAIL;
    }
    char* stack = new char[STACK_SIZE];
}

int get_thread_id() {
    if (ids.empty()) return MAX_THREAD_NUM;
    auto id = *ids.begin();
    ids.erase(ids.begin());
    return id;
}



