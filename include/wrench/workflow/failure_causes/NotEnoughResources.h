/**
 * Copyright (c) 2017. The WRENCH Team.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef WRENCH_NOT_ENOUGH_RESOURCES_H
#define WRENCH_NOT_ENOUGH_RESOURCES_H

#include <set>
#include <string>

#include <wrench/workflow/failure_causes/FailureCause.h>
#include <wrench/services/compute/ComputeService.h>

namespace wrench {

    class WorkflowJob;

    /***********************/
    /** \cond DEVELOPER    */
    /***********************/


    /**
     * @brief A "compute service doesn't have enough cores" failure cause
     */
    class NotEnoughResources : public FailureCause {
    public:
        /***********************/
        /** \cond INTERNAL     */
        /***********************/
        NotEnoughResources(WorkflowJob *job, std::shared_ptr<ComputeService> compute_service);
        /***********************/
        /** \endcond           */
        /***********************/

        WorkflowJob *getJob();
        std::shared_ptr<ComputeService> getComputeService();
        std::string toString();

    private:
        WorkflowJob *job;
        std::shared_ptr<ComputeService> compute_service;
    };


    /***********************/
    /** \endcond           */
    /***********************/
};


#endif //WRENCH_NOT_ENOUGH_RESOURCES_H
