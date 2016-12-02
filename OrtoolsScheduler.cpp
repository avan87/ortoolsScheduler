//
// Created by metis on 12/1/16.
//

#include "OrtoolsScheduler.h"
#include "Assignments.h"


    std::vector<std::pair<Worker, std::vector<Job>>>
    OrtoolsScheduler::Schedule(std::vector<Worker> workers, std::vector<Job> jobs,
                                         std::vector<std::pair<Worker, Job>> includeConstraints,
                                         std::vector<std::pair<Worker, Job>> excludeConstraints,
                                         std::vector<std::pair<Worker, Job>> priorityConstraints) {


        auto assignments = Assignments(workers, jobs, includeConstraints,
                                       excludeConstraints,
                                       priorityConstraints,
                                       schedulingSettingsProvider
        );

        assignments.addConstraints(includeConstraints, excludeConstraints, priorityConstraints);

        std::vector<operations_research::IntervalVar*> intervals = assignments.getAllIntervalVars();
        std::vector<operations_research::IntVar*> performedExpressions{intervals.size()};

        std::transform(intervals.begin(), intervals.end(), performedExpressions.begin(),
        [&] (operations_research::IntervalVar* x) { return x->PerformedExpr()->Var();});

        auto performAllPossibleVar = assignments.getSolver()->
             MakeSum(performedExpressions)->Var();

        auto performAllPossibleObjective = assignments.getSolver()->MakeMaximize(performAllPossibleVar, 1);

        auto complexVar = assignments.getSolver()->MakeProd(performAllPossibleVar,1000)->Var();

//        if(schedulingSettingsProvider.isMaximizeFleetLoad()){
//
//            auto maximizeFleetLoadVars =
//
//        } else{
//
//        }


        return std::vector<std::pair<Worker, std::vector<Job>>>();
    }
