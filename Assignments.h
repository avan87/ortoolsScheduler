//
// Created by metis on 11/30/16.
//
#include<iostream>
#include "Assignment.h"

#include "constraint_solver/constraint_solver.h"
#include "proto/scheduler.pb.h"
#include <memory>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <chrono>
#include "utils.h"

#ifndef SCHEDULER_ASSIGNMENTS_H
#define SCHEDULER_ASSIGNMENTS_H


    class Assignments {

    public:
        Assignments(const std::vector<Worker> &workers,
                    const std::vector<Job> &jobs,
                    const std::vector<std::pair<Worker, Job>> &includeConstraints,
                    const std::vector<std::pair<Worker, Job>> &excludeConstraints,
                    const std::vector<std::pair<Worker, Job>> &priorityConstraints,
                    const SchedulingSettingsProvider &schedulingSettingsProvider
        ) {


            solver.reset(new operations_research::Solver("solver"));

            if (workers.size() == 0 || jobs.size() == 0) {
                LG << "No workers or jobs for planning";
            }

            for (int i = 0; i < workers.size(); ++i) {
                matrix.push_back(std::vector<Assignment>(jobs.size()));
            }

            for (int i = 0; i < workers.size(); ++i) {
                for (int j = 0; j < jobs.size(); ++j) {

                    int isIsoOrRef = (jobs[j].parameters().iso() ? jobs[j].parameters().iso() : false )
                                     && (jobs[j].parameters().ref()  ? jobs[j].parameters().ref() : false )
                                     ? 1 : 0;

                    int raiting = workers[i].parameters() .rating() ?
                                  workers[i].parameters() .rating() : 0;


                    if (!excludeConstraints.empty()
                        && (std::any_of(excludeConstraints.begin(), excludeConstraints.end(),
                                        [&](std::pair<Worker, Job> pair) {
                                            return pair.first.id() == workers[i].id()
                                                   && pair.second.id() == jobs[j].id();
                                        }
                    ))) {

                        matrix[i].at(j) = Assignments::Null;

                    }


                    if (!includeConstraints.empty()
                        && (std::any_of(includeConstraints.begin(), includeConstraints.end(),
                                        [&](std::pair<Worker, Job> pair) {
                                            return pair.first.id() == workers[i].id()
                                                   && pair.second.id() == jobs[j].id();
                                        }))) {

                        for (int k = 0; k < workers.size(); ++k) {
                            if (k != i) {


                                matrix[k][j] = Assignments::Null;
                            }
                        }

                    }

                    if (matrix[i][j].getName() != Null.getName()) {

                        long stime = getMillisValueFromDate(jobs[j].start());
                        long etime = getMillisValueFromDate(jobs[j].end());

                        matrix[i][j] = Assignment(
                                solver->MakeFixedDurationIntervalVar(stime, etime,
                                                                     (int)  etime - stime,
                                                                     true, std::to_string(i) + std::to_string(j)),
                                std::vector<operations_research::IntVar *>{
                                        solver->MakeIntConst((workers[i].cost() - jobs[j].cost()) *
                                                             schedulingSettingsProvider.weightfactor()),
                                        solver->MakeIntConst(isIsoOrRef * schedulingSettingsProvider.isoreffactor()),
                                        solver->MakeIntConst(raiting * schedulingSettingsProvider.ratingfactor()),
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
                    if (matrix[j][i].getName() != Null.getName()) {
                        vec.push_back(matrix[j][i]);
                    }
                }

                std::vector<operations_research::IntVar *> performedVec{vec.size()};

                std::transform(vec.begin(), vec.end(), performedVec.begin(),
                               [&](Assignment x) { return x.getInterval()->PerformedExpr()->Var(); });


                if (!performedVec.empty()) {
                    solver->AddConstraint(solver->MakeLessOrEqual(solver->MakeSum(performedVec), 1));
                }

                vec.clear();


            }


            for (int i = 0; i < workers.size(); ++i) {
                for (int j = 0; j < jobs.size(); ++j) {
                    if (matrix[i][j].getName() != Null.getName()) {
                        all.push_back(matrix[i][j]);
                    }
                }
            }


            std::transform(all.begin(), all.end(), std::back_inserter(allIntervalVars),
                           [](Assignment x) { return x.getInterval(); });


        }

        void addConstraints(const std::vector<std::pair<Worker, Job>> &includeConstraints,
                            const std::vector<std::pair<Worker, Job>> &excludeConstraints,
                            const std::vector<std::pair<Worker, Job>> &proirityConstraints
        );


        const std::vector<std::vector<Assignment>> &getMatrix() const {
            return matrix;
        }

        const std::vector<Assignment> &getAll() const {
            return all;
        }

        const std::vector<operations_research::IntervalVar *> &getAllIntervalVars() const {
            return allIntervalVars;
        }

        static Assignment Null;

    private:
        std::unique_ptr<operations_research::Solver> solver;

        std::vector<std::vector<Assignment>> matrix;
        std::vector<Assignment> all;
        std::vector<operations_research::IntervalVar *> allIntervalVars;


    public:
        const std::unique_ptr<operations_research::Solver> &getSolver() const;

    };


#endif //SCHEDULER_ASSIGNMENTS_H
