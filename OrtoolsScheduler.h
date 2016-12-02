//
// Created by metis on 12/1/16.
//

#ifndef SCHEDULER_ORTOOLSSCHEDULER_H
#define SCHEDULER_ORTOOLSSCHEDULER_H

#include "constraint_solver/constraint_solver.h"
#include "SchedulingSettingsProvider.h"
#include "worker.h"
#include "Job.h"


    class OrtoolsScheduler {

    public:
      OrtoolsScheduler(SchedulingSettingsProvider schedulingSettingsProvider):
              schedulingSettingsProvider(schedulingSettingsProvider){}
      std::vector<std::pair<Worker, std::vector<Job>>> Schedule(std::vector<Worker> workers,
                                                                std::vector<Job> jobs,
                                                                std::vector<std::pair<Worker,Job>> includeConstraints,
                                                                std::vector<std::pair<Worker,Job>> excludeConstraints,
                                                                std::vector<std::pair<Worker,Job>> priorityConstraints
      );
    private:

        SchedulingSettingsProvider  schedulingSettingsProvider;



    };


#endif //SCHEDULER_ORTOOLSSCHEDULER_H
