//
// Created by metis on 11/30/16.
//
#include<iostream>
#include "Assignment.h"
#include "worker.h"
#include "Job.h"
#include "constraint_solver/constraint_solver.h"
#include "SchedulingSettingsProvider.h"
#include <memory>
#ifndef SCHEDULER_ASSIGNMENTS_H
#define SCHEDULER_ASSIGNMENTS_H


    class Assignments {

    public:
        Assignments(std::vector<Worker> &workers, std::vector<Job> &jobs,
          std::vector<std::pair<Worker,Job>> &includeConstraints,
                    std::vector<std::pair<Worker,Job>> &excludeConstraints,
                    std::vector<std::pair<Worker,Job>> &priorityConstraints,
                    SchedulingSettingsProvider schedulingSettingsProvider
        ) {



             if(workers.size() == 0 || jobs.size() == 0){
                 LG << "No workers or jobs for planning";
             }

            for (int i = 0; i < workers.size(); ++i) {
                matrix.push_back(std::vector<Assignment>(jobs.size()));
            }

            for (int i = 0; i < workers.size(); ++i) {
                for (int j = 0; j < jobs.size(); ++j) {
                    int isIsoOrRef = jobs[j].getParameters().isIso() &&
                                     (jobs[j].getParameters().isRef()) ? 1 : 0;
                    int raiting = workers[i].getParameters().getRating();


                    if(!excludeConstraints.empty()
                       && (std::any_of(excludeConstraints.begin(), excludeConstraints.end(),
                                       [&](std::pair<Worker,Job> pair) { return pair.first.getId() == workers[i].getId()
                            && pair.second.getId() == jobs[j].getId();}
                    ))){
                        auto it = matrix[i].begin();
                        matrix[i].at(j) = Assignments::Null;

                    }


                    if(!includeConstraints.empty()
                       &&  (std::any_of(includeConstraints.begin(), includeConstraints.end() ,
                                        [&] (std::pair<Worker,Job> pair){ return pair.first.getId() == workers[i].getId()
                                                && pair.second.getId() == jobs[j].getId();})) ){

                        for (int k = 0; k < workers.size(); ++k) {
                            if (k != i){
                                auto it = matrix[k].begin();

                                matrix[k].insert(it + j, Assignments:: Null);
                            }
                        }

                    }

                    if(matrix[i][j].getName() != Null.getName()){

                             matrix[i][j] = Assignment(
                                     solver->MakeFixedDurationIntervalVar(jobs[j].getStart(), jobs[j].getEnd(),
                                                                          (int) jobs[j].getEnd() - jobs[j].getStart(),
                                     true,  std::to_string(i) + std::to_string(j)),
                                     std::vector<operations_research::IntVar*>{
                                         solver->MakeIntConst((workers[i].getCost() - jobs[j].getCost()) * schedulingSettingsProvider.getWeightFactor()),
                                         solver->MakeIntConst(isIsoOrRef * schedulingSettingsProvider.getIsoRefFactor()),
                                         solver->MakeIntConst(raiting * schedulingSettingsProvider.getRatingFactor()),
                                     },
                                     workers[i],
                                     jobs[j]

                             );

                    }

                }
            }

            for (int i = 0; i < jobs.size(); ++i) {
                std::vector<Assignment> vec;
                for (int j = 0; j < workers.size(); ++j) {
                    vec.push_back(matrix[j][i]);
                }

                std::vector<Assignment> tempVec;


                std::copy_if(vec.begin(), vec.end(),  std::back_inserter(tempVec),
                             [&] (Assignment x) { return x.getName() != "Empty" ;});


                std::vector<operations_research::IntervalVar*> intervals{tempVec.size()};
                std::vector<operations_research::IntVar *> performedVec{intervals.size()};

                std::transform(tempVec.begin(), tempVec.end(), intervals.begin(), [&] (Assignment x) {return x.getInterval();});





                std::transform(intervals.begin(), intervals.end(), performedVec.begin(),
                [] (operations_research::IntervalVar* x) { return x->PerformedExpr()->Var();});
                if (!performedVec.empty()) {
                    solver->AddConstraint(solver->MakeLessOrEqual(solver->MakeSum(performedVec), 1));
                }

                vec.clear();


            }

            for (int i = 0; i < workers.size(); ++i) {
                for (int j = 0; j < jobs.size(); ++j) {
                    all.push_back(matrix[i][j]);
                }
            }

            for (auto x : all){
                allIntervalVars.push_back(x.getInterval());
            }




        }

        void addConstraints(std::vector<std::pair<Worker,Job>> includeConstraints,
                            std::vector<std::pair<Worker,Job>> excludeConstraints,
                            std::vector<std::pair<Worker,Job>> proirityConstraints
        );


        operations_research::Solver *getSolver() const {
            return solver;
        }

        const Assignment &getNull() const {
            return Null;
        }

        const std::vector<std::vector<Assignment>> &getMatrix() const {
            return matrix;
        }

        const std::vector<Assignment> &getAll() const {
            return all;
        }

        const std::vector<operations_research::IntervalVar *> &getAllIntervalVars() const {
            return allIntervalVars;
        }


    private:
        operations_research:: Solver *solver = new operations_research::Solver("scheduler");
        static Assignment Null;
        std::vector<std::vector<Assignment>> matrix;
        std::vector<Assignment> all;
        std::vector<operations_research::IntervalVar*> allIntervalVars;





    };


#endif //SCHEDULER_ASSIGNMENTS_H
