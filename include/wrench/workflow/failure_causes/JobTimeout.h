/**
 * Copyright (c) 2017. The WRENCH Team.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef WRENCH_JOB_TIMEOUT_H
#define WRENCH_JOB_TIMEOUT_H

#include <set>
#include <string>

#include "wrench/workflow/failure_causes/FailureCause.h"

namespace wrench {

    class WorkflowJob;

    /***********************/
    /** \cond DEVELOPER    */
    /***********************/

    /**
    * @brief A "job has timed out" failure cause
    */
    class JobTimeout : public FailureCause {
    public:
        /***********************/
        /** \cond INTERNAL     */
        /***********************/
        JobTimeout(WorkflowJob *job);
        /***********************/
        /** \endcond           */
        /***********************/

        WorkflowJob *getJob();
        std::string toString();

    private:
        WorkflowJob *job;
    };


    /***********************/
    /** \endcond           */
    /***********************/
};


#endif //WRENCH_JOB_TIMEOUT_H
