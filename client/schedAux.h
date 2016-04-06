/**
 * @file    schedAux.h
 * @author  DFLM (dflarios@dte.us.es)
 * @author  Christian Antoñanzas(chanma@iteam.upv.es)
 * @date    December, 2015
 * @version 0.1b
 * @bug     not known bugs in schedAux.h
 * @brief   Priority Scheduler definitions.
 *
 * Auxiliary functions used to manage threads devices.
*/

/**
* @defgroup schedAux The schedAux Group
* @ingroup  Aux
* @brief    Functions to manage priority scheduler.
* @{
*/
#ifndef SCHEDAUX_H
#define SCHEDAUX_H


//******************************************************************************
//* INCLUDES                                                                   *
//****************************************************************************** 
#include <sched.h>						// Manage Thread priority in posix
#include <pthread.h>
#include <iostream>


//******************************************************************************
//* CONSTANTS                                                                  *
//******************************************************************************


//******************************************************************************
//* GLOBAL VARIABLES														   *
//******************************************************************************


//******************************************************************************
//* TYPEDEFS                                                                   *
//******************************************************************************


//******************************************************************************
//* FUNCTION PROPTOTIPES                                                       *
//******************************************************************************
/**
 * @brief The setscheduler function.
 *
 * Set the process task at maximum priority.
 * @param prioreduce reducction from maximun priority
 */
void setschedulerMaxPriority(const int& prioreduce=0){
        struct sched_param sched_param;
        if (sched_getparam(0, &sched_param) < 0) {
            std::cout << "Scheduler getparam failed..."<<std::endl;
            return;
        }
        sched_param.sched_priority = sched_get_priority_max(SCHED_RR)-prioreduce;  //SCHED_RR
        if (!sched_setscheduler(0, SCHED_RR, &sched_param)) {           //SCHED_RR
            std::cout << "Scheduler set with priority " << sched_param.sched_priority <<"..."<<std::endl;
                fflush(stdout);
                return;
        }
        std::cout << "!!!Scheduler set with priority " << sched_param.sched_priority<<" FAILED!!!"<<std::endl;
}

/**
 * @brief The setThreadMaxPriority function.
 *
 * Set the thread task at maximum priority.
 * @param prioreduce Reducction from maximun priority
 */
void setThreadMaxPriority(const int& prioreduce=0){
        struct sched_param sched_param;
        int policy;

        pthread_getschedparam(pthread_self(), &policy, &sched_param);
        policy=SCHED_RR;
        sched_param.sched_priority = sched_get_priority_max(SCHED_RR)-prioreduce;
        pthread_setschedparam(pthread_self(), policy, &sched_param);

        std::cout << "thread with priority: "<< sched_param.sched_priority <<std::endl;
}


/**
 * @brief The setCore function.
 *
 * Force the execution of the thread in a specific core [0-n] range.
 * @param core Core where the thread is going to be executed.
 */
void setCore(const int& core){
        unsigned int nthreads=std::thread::hardware_concurrency();

        cpu_set_t cpuset;
        CPU_ZERO(&cpuset);
        CPU_SET((core%nthreads), &cpuset);
        pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset);

        std::cout<<"set thread in core "<< (core%nthreads) <<std::endl;
}


//******************************************************************************
//* CLASS PROPTOTIPES                                                          *
//******************************************************************************


#endif
/** @} */ //end of group
