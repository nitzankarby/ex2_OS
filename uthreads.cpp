//
// Created by NitzanKarby on 01/04/2022.
//

#include "uthreads.h"

// Consts for the program
#define FAIL -1
int quantum;




int uthread_init(int quantum_usecs) {
    if (quantum_usecs <= 0) return FAIL;
    quantum = quantum_usecs;

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

