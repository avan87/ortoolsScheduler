//
// Created by metis on 12/1/16.
//

#ifndef SCHEDULER_ORTOOLSSCHEDULER_H
#define SCHEDULER_ORTOOLSSCHEDULER_H

#include "constraint_solver/constraint_solver.h"
#include "proto/scheduler.pb.h"

    class OrtoolsScheduler {

    public:
        OrtoolsScheduler(SchedulingSettingsProvider schedulingSettingsProvider) :
                schedulingSettingsProvider(schedulingSettingsProvider) {}

        std::vector<std::pair<Worker, Job>> Schedule(const std::vector<Worker> &workers,
                                                     const std::vector<Job> &jobs,
                                                     const std::vector<std::pair<Worker, Job>> &includeConstraints,
                                                     const std::vector<std::pair<Worker, Job>> &excludeConstraints,
                                                     const std::vector<std::pair<Worker, Job>> &priorityConstraints
        );

    private:

        SchedulingSettingsProvider schedulingSettingsProvider;


    };


#endif //SCHEDULER_ORTOOLSSCHEDULER_H
