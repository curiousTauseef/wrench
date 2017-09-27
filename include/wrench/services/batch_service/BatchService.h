//
// Created by suraj on 8/29/17.
//

#ifndef WRENCH_BATCH_SERVICE_H
#define WRENCH_BATCH_SERVICE_H

#include "wrench/services/Service.h"
#include <queue>
#include <deque>
#include "wrench/workflow/job/StandardJob.h"
#include "BatchServiceProperty.h"
#include "wrench/services/compute/ComputeService.h"
#include "wrench/services/compute/standard_job_executor/StandardJobExecutor.h"
#include "wrench/workflow/job/WorkflowJob.h"
#include <tuple>
#include "BatchJob.h"
#include "BatchScheduler.h"
#include <set>

namespace wrench {

    class BatchService: public ComputeService {

    /**
     * @brief A Batch Service
     */


    private:
        std::map<std::string, std::string> default_property_values =
                {{BatchServiceProperty::STOP_DAEMON_MESSAGE_PAYLOAD,          "1024"},
                 {BatchServiceProperty::DAEMON_STOPPED_MESSAGE_PAYLOAD,       "1024"},
                 {BatchServiceProperty::THREAD_STARTUP_OVERHEAD,              "0"},
                 {BatchServiceProperty::STANDARD_JOB_DONE_MESSAGE_PAYLOAD,    "1024"},
                 {BatchServiceProperty::SUBMIT_STANDARD_JOB_ANSWER_MESSAGE_PAYLOAD,    "1024"},
                 {BatchServiceProperty::SUBMIT_PILOT_JOB_ANSWER_MESSAGE_PAYLOAD,        "1024"},
                 {BatchServiceProperty::STANDARD_JOB_FAILED_MESSAGE_PAYLOAD,  "1024"},
                 {BatchServiceProperty::PILOT_JOB_STARTED_MESSAGE_PAYLOAD,    "1024"},
                 {BatchServiceProperty::SUBMIT_BATCH_JOB_ANSWER_MESSAGE_PAYLOAD,     "1024"},
                 {BatchServiceProperty::SUBMIT_BATCH_JOB_REQUEST_MESSAGE_PAYLOAD,    "1024"},
                 {BatchServiceProperty::PILOT_JOB_EXPIRED_MESSAGE_PAYLOAD,           "1024"},
                 {BatchServiceProperty::HOST_SELECTION_ALGORITHM,           "FIRSTFIT"},
                 {BatchServiceProperty::JOB_SELECTION_ALGORITHM,           "FCFS"}
                };

    public:
        BatchService(std::string hostname,
        std::vector<std::string> nodes_in_network,
                     StorageService *default_storage_service,
                     bool supports_standard_jobs,
                     bool supports_pilot_jobs,
                std::map<std::string, std::string> plist = {});

        //cancels the job
        void cancelJob(unsigned long jobid);
        //returns jobid,started time, running time
        std::vector<std::tuple<unsigned long,double,double>> getJobsInQueue();


    private:
        BatchService(std::string hostname,
        std::vector<std::string> nodes_in_network,
        StorageService *default_storage_service,
                     bool supports_standard_jobs,
                     bool supports_pilot_jobs,
                     PilotJob* parent_job,
                     unsigned long reduced_cores,
        std::map<std::string, std::string> plist,
        std::string suffix);

        //parent batch job (this is necessary for pilot jobs)
        PilotJob* parent_pilot_job;

        //Configuration to create randomness in measurement period initially
        unsigned long random_interval = 10;


        /* Resources information in Batchservice */
        unsigned long total_num_of_nodes;
        std::map<std::string,unsigned long> nodes_to_cores_map;
        std::vector<double> timeslots;
        std::map<std::string,unsigned long> available_nodes_to_cores;
        /*End Resources information in Batchservice */

        // Vector of standard job executors
        std::set<StandardJobExecutor *> standard_job_executors;

        //Queue of pending batch jobs
        std::deque<BatchJob*> pending_jobs;
        //A set of running batch jobs
        std::set<BatchJob*> running_jobs;

        unsigned long generateUniqueJobId();

        //submits the standard job
        //overriden function of parent Compute Service
        unsigned long submitStandardJob(StandardJob *job,std::map<std::string,unsigned long> batch_job_args) override;

        //submits the standard job
        //overriden function of parent Compute Service
        unsigned long submitPilotJob(PilotJob *job,std::map<std::string,unsigned long> batch_job_args) override;

        int main() override;
        bool processNextMessage(double);
        bool dispatchNextPendingJob();
        void processStandardJobCompletion(StandardJobExecutor *executor, StandardJob *job);

        void processStandardJobFailure(StandardJobExecutor *executor,
                                       StandardJob *job,
                                       std::shared_ptr<FailureCause> cause);

        void failPendingStandardJob(StandardJob *job, std::shared_ptr<FailureCause> cause);
        void failRunningStandardJob(StandardJob *job, std::shared_ptr<FailureCause> cause);
        void terminateRunningStandardJob(StandardJob *job);

        std::set<std::pair<std::string,unsigned long>> scheduleOnHosts(std::string host_selection_algorithm,
                                                                       unsigned long, unsigned long);

        BatchJob* scheduleJob(std::string);

        //Terminate the batch service (this is usually for pilot jobs when they act as a batch service)
        void terminate(bool);

        //Fail the standard jobs inside the pilot jobs
        void failCurrentStandardJobs(std::shared_ptr<FailureCause> cause);

        //Process the pilot job completion
        void processPilotJobCompletion(PilotJob* job);

        //Process standardjob timeout
        void processStandardJobTimeout(StandardJob* job);

        //Process standardjob timeout
        void processPilotJobTimeout(PilotJob* job);

        //notify upper level job submitters (about pilot job termination)
        void notifyJobSubmitters(PilotJob* job);

        //update the resources
        void udpateResources(std::set<std::pair<std::string,unsigned long>> resources);

        //send call back to the pilot job submitters
        void sendPilotJobCallBackMessage(PilotJob* job);

    };
}


#endif //WRENCH_BATCH_SERVICE_H