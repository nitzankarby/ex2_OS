//
// Created by NitzanKarby on 01/04/2022.
//

#include "../../Downloads/uthreads.h"
#include <stdio.h>
#include <setjmp.h>
#include <signal.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdbool.h>
#include <sys/time.h>
#include <vector>
#include <queue>
#include <set>
#include<map>
#include <iostream>
using namespace std;
using std::vector;
using std::queue;
using std::set ;
using std::map;
// Consts for the program
#define FAIL (-1)
#define SUCCESS (0)
#define JB_SP 4
#define JB_PC 5
typedef void (*thread_entry_point)(void);
typedef unsigned long address_t;

#define THREAD_ERROR "thread library error: "
// data structures
struct itimerval timer;
struct sigaction sa= {nullptr};
sigjmp_buf env[MAX_THREAD_NUM];
set<int> ids;
queue<int> ready_queue;
map<int, int> sleeping_map;
set<int> blocked_threads;
map<int,int> quantum_counter;


// global parameters
int quantum;
int total_quantum;
int current_running_thread;

// func declarations
void increment_quantum_counter(int id);
int get_thread_id();
void sleep_list_handling();
address_t translate_address(address_t addr);
int add_to_ready_queue(int id);

void time_handler(int signal){
  fprintf(stderr, "in time handler \n", total_quantum);
  // Running thread to wait list
  add_to_ready_queue(current_running_thread);
  // Sleep list handling
  sleep_list_handling();
  // Wait list to running
  current_running_thread = ready_queue.front();
  ready_queue.pop();
  total_quantum++;
  increment_quantum_counter(current_running_thread);
  // Actual jump
  // TODO: Understand how the code that ran before keep on going
  siglongjmp(env[current_running_thread], 1);  // Nice !
}

int return_error_msg(int type, char* str){
    if (type == 1) fprintf(stderr, "thread library error: %s\n", str);
    else if (type == 0) fprintf(stderr, "system error: \n");
    return FAIL;
}

int initialize_main_thread(){
    int id = get_thread_id();
    if (id == MAX_THREAD_NUM) return return_error_msg(1, (char*)"maximum number of threads exceeded \n");
    current_running_thread = id;
    quantum_counter[current_running_thread] = 1;
    sigsetjmp(env[0], 1);
    return SUCCESS;
}

int initialize_timer(){
    total_quantum = 0;
    sa.sa_handler=&time_handler;
    if (sigaction(SIGVTALRM, &sa, nullptr) < 0) return return_error_msg(0, (char*)"signal handling error\n");
    int quantum_sec = quantum / 1000000;
    int quantum_usec = quantum % 1000000;
    timer.it_value.tv_sec = quantum_sec;
    timer.it_value.tv_usec = quantum_usec;
    timer.it_interval.tv_sec = quantum_sec;
    timer.it_interval.tv_usec = quantum_usec;
    if (setitimer(ITIMER_VIRTUAL, &timer, nullptr)) return return_error_msg(0, (char *) "timer problem\n");
    return SUCCESS;
}

void initialize_ids_set(){
    // initialize properly later
    for (int i = 0; i <MAX_THREAD_NUM; ++i) {
        ids.insert(i);
    }
}

int uthread_init(int quantum_usecs) {
    if (quantum_usecs <= 0) return return_error_msg(1, (char*)"bad input! must be greater than 0\n");
    quantum = quantum_usecs;
    initialize_ids_set();
    if (initialize_main_thread() == FAIL) return FAIL;
    if (initialize_timer() == FAIL) return FAIL;
    return SUCCESS;
}

int uthread_spawn(thread_entry_point entry_point) {
    if (!entry_point) return return_error_msg(1,(char *) "bad entry point\n");
    int id = get_thread_id();
    if (id == MAX_THREAD_NUM) return return_error_msg(1, (char*)"maximum number of threads exceeded \n");
    // initializing the thread//
    //creating the threads stack
    char * stack;
    try{
        stack = new char[STACK_SIZE];
    } catch (const bad_alloc& e){
      return return_error_msg(0, (char *) "Memory allocation failed\n");
    }
    address_t sp = (address_t) stack + STACK_SIZE - sizeof(address_t); // check if this is the syntax
    address_t pc = (address_t) entry_point;
    sigsetjmp(env[id], 1);
    (env[id]->__jmpbuf)[JB_SP] = translate_address(sp);
    (env[id]  ->__jmpbuf)[JB_PC] = translate_address(pc);
    sigemptyset(&env[id]->__saved_mask);
    add_to_ready_queue(id);
    return 0;
}

int uthread_terminate(int tid) {
    if (tid < 0 || tid >= (MAX_THREAD_NUM) || ids.find(tid) != ids.end()){
      // No such process
      return return_error_msg(1, (char *) "No such thread id");
    }
    if (tid == 0){
      // Need to terminate all processes
    }


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
    return current_running_thread;
}

int uthread_get_total_quantums() {
    return total_quantum;
}

int uthread_get_quantums(int tid) {
    return quantum_counter[tid];
}

// TODO : this function should be covered by the uthread_spawn() so must be deleted
//int initialize_thread(thread_entry_point entry_point) {
//    int cur_id = get_thread_id();
//    if (cur_id == MAX_THREAD_NUM) return return_error_msg(1, (char*)"maximum number of threads exceeded\n");
//    char* stack = new char[STACK_SIZE];
//    return SUCCESS;
//}

int get_thread_id() {
    if (ids.empty()) return MAX_THREAD_NUM;
    auto id = *ids.begin();
    ids.erase(ids.begin());
    return id;
}

void sleep_list_handling(){
  map<int, int>::iterator it;
  vector<int> deleted = {};
  for (it = sleeping_map.begin(); it != sleeping_map.end(); it++)
  {
    --it->second;
    if (it->second <= 0){
      deleted.push_back(it->first);
      if (blocked_threads.find(it->first) == blocked_threads.end()){
        ready_queue.push(it->first);
      }
    }
  }
  for (int & i : deleted)
    {
      sleeping_map.erase(i);
    }
}
void increment_quantum_counter(int id ){
  if (quantum_counter.find(id) != quantum_counter.end()){
      quantum_counter[id] = 0;
    }
  quantum_counter[id] += 1;
}


int add_to_ready_queue(int id){
    ready_queue.push(id);

}

address_t translate_address(address_t addr)
{
    address_t ret;
    asm volatile("xor    %%gs:0x18,%0\n"
                 "rol    $0x9,%0\n"
    : "=g" (ret)
    : "0" (addr));
    return ret;
}