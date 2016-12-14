//
// Created by metis on 11/30/16.
//

#include "Assignments.h"




     Assignment Assignments:: Null = Assignment("name");

    void Assignments::addConstraints(const std::vector<std::pair<Worker, Job>> &includeConstraints,
                                     const std::vector<std::pair<Worker, Job>> &excludeConstraints,
                                     const std::vector<std::pair<Worker, Job>> &priorityConstraints) {

        for (auto assignment : all){

            if(!includeConstraints.empty()
                    && (std::any_of(includeConstraints.begin(), includeConstraints.end(),
                                    [&](std::pair<Worker,Job> pair) { return pair.first.id() == assignment.getWorker().id()
                                                                             && pair.second.id() == assignment.getJob().id();}
            ))){
                assignment.getInterval()->SetPerformed(true);
            }

            if(!excludeConstraints.empty()
                    && (std::any_of(excludeConstraints.begin(), excludeConstraints.end(),
                                    [&](std::pair<Worker,Job> pair) { return pair.first.id() == assignment.getWorker().id()
                                                                             && pair.second.id() == assignment.getJob().id();}
            ))){
                assignment.getInterval()->SetPerformed(false);
            }

            if(!priorityConstraints.empty()
                    && (std::any_of(priorityConstraints.begin(), priorityConstraints.end(),
                                    [&](std::pair<Worker,Job> pair) { return pair.first.id() == assignment.getWorker().id()
                                                                             && pair.second.id() == assignment.getJob().id();}
            ))){

                assignment.setPriority(1);
            }

        }

        for(auto workerConstraints : matrix){

            for(auto wc1 : workerConstraints){

                std::vector<Assignment> ws(workerConstraints);
                auto j = ws.begin();

                while (j != ws.end()) {
                    if(wc1.getName() != j->getName()){
                        ws.erase(j);
                    }

                    else ++j;
                }

                ws.erase(j);




                //std::cout << vec.size() << std::endl;

                for(auto wc2 : ws){
                    if(wc1.getName() == wc2.getName()) continue;

                    if(wc1.getName() != Assignments::Null.getName() && wc2.getName() != Assignments::Null.getName()){

                        if(!wc1.getInterval()->MustBePerformed() || !wc2.getInterval()->MustBePerformed()){
                            solver->AddConstraint(solver->MakeTemporalDisjunction(wc1.getInterval(), wc2.getInterval()));
                        } else{
                            LG << "Skipping disjunct: " << wc1.getName() << ", " << wc2.getName();
                        }
                    }
                }

            }

        }





}

const std::unique_ptr<operations_research::Solver> &Assignments::getSolver() const {
    return solver;
}
