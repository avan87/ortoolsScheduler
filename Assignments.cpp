//
// Created by metis on 11/30/16.
//

#include "Assignments.h"




     Assignment Assignments:: Null = Assignment("name");

    void Assignments::addConstraints(std::vector<std::pair<Worker, Job>> includeConstraints,
                                     std::vector<std::pair<Worker, Job>> excludeConstraints,
                                     std::vector<std::pair<Worker, Job>> priorityConstraints) {

        for (auto assignment : all){

            if(!includeConstraints.empty()
                    && (std::any_of(includeConstraints.begin(), includeConstraints.end(),
                                    [&](std::pair<Worker,Job> pair) { return pair.first.getId() == assignment.getWorker().getId()
                                                                             && pair.second.getId() == assignment.getJob().getId();}
            ))){
                assignment.getInterval()->SetPerformed(true);
            }

            if(!excludeConstraints.empty()
                    && (std::any_of(excludeConstraints.begin(), excludeConstraints.end(),
                                    [&](std::pair<Worker,Job> pair) { return pair.first.getId() == assignment.getWorker().getId()
                                                                             && pair.second.getId() == assignment.getJob().getId();}
            ))){
                assignment.getInterval()->SetPerformed(false);
            }

            if(!priorityConstraints.empty()
                    && (std::any_of(priorityConstraints.begin(), priorityConstraints.end(),
                                    [&](std::pair<Worker,Job> pair) { return pair.first.getId() == assignment.getWorker().getId()
                                                                             && pair.second.getId() == assignment.getJob().getId();}
            ))){

                assignment.setPriority(1);
            }

        }

        for(auto workerConstraints : matrix){

            for(auto wc1 : workerConstraints){


                int counter = 0;
                while(workerConstraints[counter].getName() != wc1.getName()){ ++counter;}

                std::vector<Assignment> vec(workerConstraints.begin() + (counter + 1), workerConstraints.end());



                //std::cout << vec.size() << std::endl;

                for(auto wc2 : vec){
                    if(wc1.getName() == wc2.getName()) continue;

                    if(&wc1 == &Assignments::Null && &wc2 == &Assignments::Null){

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