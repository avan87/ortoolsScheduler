//
// Created by metis on 12/1/16.
//

#include "OrtoolsScheduler.h"
#include "Assignments.h"
#include <chrono>
using namespace std::chrono;


    std::vector<std::pair<Worker, Job> >
    OrtoolsScheduler::Schedule(const std::vector<Worker> &workers,
                               const std::vector<Job> &jobs,
                               const std::vector<std::pair<Worker, Job>> &includeConstraints,
                               const std::vector<std::pair<Worker, Job>> &excludeConstraints,
                               const std::vector<std::pair<Worker, Job>> &priorityConstraints) {


        auto assignments = Assignments(workers, jobs, includeConstraints,
                                       excludeConstraints,
                                       priorityConstraints,
                                       schedulingSettingsProvider
        );

        assignments.addConstraints(includeConstraints, excludeConstraints, priorityConstraints);



        std::vector<operations_research::IntVar*> performedExpressions;

        for (auto i : assignments.getAllIntervalVars()) {
            performedExpressions.push_back(i->PerformedExpr()->Var());
        }

        auto performAllPossibleVar = assignments.getSolver()->
             MakeSum(performedExpressions)->Var();

        auto performAllPossibleObjective = assignments.getSolver()->MakeMaximize(performAllPossibleVar, 1);

        auto complexVar = assignments.getSolver()->MakeProd(performAllPossibleVar,1000)->Var();

        if(schedulingSettingsProvider.maximizefleetload()){

            std::vector<operations_research:: IntVar*> maximizeFleetLoadVars;

            std::vector<std::vector<Assignment>> rows;

            for(auto vec : assignments.getMatrix()){
                if(std::any_of(vec.begin(), vec.end(),
                               [&](Assignment x){return x.getName() != assignments.Null.getName();})){
                    rows.push_back(vec);
                }

            }


            for (auto row: rows){
                std::vector<Assignment> assigns;

                std::copy_if(row.begin(), row.end(), std::back_inserter(assigns),
                [&](Assignment x){ return x.getName() != assignments.Null.getName()  ;});

                std::vector<operations_research::IntVar*> intVars;

                std::transform(assigns.begin(), assigns.end(), std::back_inserter(intVars),[] (Assignment x){
                    return x.getInterval()->PerformedExpr()->Var();
                });

                auto maxIntVarInARow = assignments.getSolver()->MakeMax(intVars)->Var();
                maximizeFleetLoadVars.push_back(maxIntVarInARow);

            }



            operations_research::IntVar* maximizeFleetLoadVarsMaxVar =

                        !maximizeFleetLoadVars.empty() ?
                       assignments.getSolver()->MakeSum(maximizeFleetLoadVars)->Var() :
                       assignments.getSolver()->MakeSum(
                               std::vector<operations_research::IntVar*>{assignments.getSolver()
                                                                                 ->MakeIntVar(1,1)})->Var();



            complexVar = assignments.getSolver()->MakeProd(complexVar, maximizeFleetLoadVarsMaxVar)->Var();


        } else {

            std::vector<operations_research::IntVar *> minimizeFleetLoadVars;
            std::vector<std::vector<Assignment>> rows;

            for(auto vec : assignments.getMatrix()){
                if(std::any_of(vec.begin(), vec.end(),
                               [&](Assignment x) {
                                   return x.getName() != assignments.Null.getName();
                               })){
                    rows.push_back(vec);
                }
            }



            for (auto row: rows) {
                std::vector<Assignment> assigns;

                std::copy_if(row.begin(), row.end(), std::back_inserter(assigns),
                             [&](Assignment x) { return x.getName() != assignments.Null.getName(); });



                std::vector<operations_research::IntVar *> intVars;

                std::transform(assigns.begin(), assigns.end(), std::back_inserter(intVars), [&](Assignment x) {
                    return assignments.getSolver()->MakeDifference(1, x.getInterval()->PerformedExpr())->Var();
                });

                auto minIntVarInARow = assignments.getSolver()->MakeMin(intVars)->Var();
                minimizeFleetLoadVars.push_back(minIntVarInARow);



                operations_research::IntVar *minimizeFleetLoadVarsMaxVar =

                        minimizeFleetLoadVars.empty() ?
                        assignments.getSolver()->MakeSum(minimizeFleetLoadVars)->Var() :
                        assignments.getSolver()->
                                MakeSum(std::vector<operations_research::IntVar *>{assignments.getSolver()->
                                MakeIntVar(1, 1)})->Var();

                complexVar = assignments.getSolver()->MakeProd(complexVar, minimizeFleetLoadVarsMaxVar)->Var();


            }
        }

            operations_research::OptimizeVar* complexVarOptimize = assignments.getSolver()->MakeMaximize(complexVar,1);

            operations_research::SearchLimit* timeLimit = assignments.getSolver()->MakeTimeLimit(60 * 1000);
            operations_research::SearchLimit* solutionLimit = assignments.getSolver()->MakeSolutionsLimit(5);



            operations_research::SolutionCollector* bestCollector = assignments.getSolver()->MakeBestValueSolutionCollector(true);
            bestCollector->Add(assignments.getAllIntervalVars());
            bestCollector->AddObjective(complexVar);

            std::vector<operations_research::SearchMonitor*> smVector = {
                    complexVarOptimize,
                    bestCollector,
                    timeLimit,
                    solutionLimit
            };


            operations_research::DecisionBuilder* decisionBuilderPhase1 = assignments.getSolver()->
                    MakePhase(assignments.getAllIntervalVars(),assignments.getSolver()->INTERVAL_DEFAULT);

            assignments.getSolver()->NewSearch(decisionBuilderPhase1, smVector);


        high_resolution_clock Clock;

        auto fss = Clock.now();

        while (assignments.getSolver()->NextSolution()) { }
            assignments.getSolver()->EndSearch();

        auto fse = Clock.now();

        std::cout << "first search time: " << duration_cast<seconds>(fse - fss).count() << std::endl;


            operations_research::Assignment* bestSolution =  bestCollector->solution(0);
            int64 optimizedObj  =  bestCollector->objective_value(0);

            operations_research::DecisionBuilder* decisionBuilderPhase2 = assignments.getSolver()->MakePhase(
                    assignments.getAllIntervalVars(),assignments.getSolver()->INTERVAL_DEFAULT);



            operations_research::IntVar* maxVarsTimesPerformedExpr;
            std::vector<operations_research::IntVar*> vars;

            for(Assignment a: assignments.getAll()){


                operations_research::IntVar* var =   assignments.getSolver()->MakeProd(assignments.getSolver()->
                        MakeSum(a.getMaximizeVars()), a.getInterval()->PerformedExpr())->Var();

                vars.push_back(var);

            }

            maxVarsTimesPerformedExpr = assignments.getSolver()->MakeSum(vars)->Var();


            auto minimizeFactors = assignments.getSolver()->MakeMinimize(maxVarsTimesPerformedExpr, 1);

            assignments.getSolver()->AddConstraint(assignments.getSolver()->MakeEquality(complexVar, optimizedObj));

            std::vector<operations_research::SearchMonitor*> smVectorPhase2 = {
                    minimizeFactors,
                    bestCollector,
                    timeLimit,
                    solutionLimit
            };

            assignments.getSolver()->NewSearch(decisionBuilderPhase2, smVectorPhase2);

            auto sss = Clock.now();
            while (assignments.getSolver()->NextSolution()) { }
            assignments.getSolver()->EndSearch();
            auto sse = Clock.now();

            std::cout << "second search time: " << duration_cast<seconds>(sse - sss).count() << std::endl;

            if (bestCollector->solution_count() == 0){ LG << "No Solutions" ;}

            auto bestSolutionPhase2 = bestCollector->solution(0);
            auto optimizedObj2 = bestCollector->objective_value(0);

            std::vector<std::pair<Worker, Job>> solution;

            std::vector<Assignment> performedValueEqToOne;
            std::vector<Assignment> notIntIcludeConstraints;





            std::copy_if(assignments.getAll().begin(), assignments.getAll().end(), std::back_inserter(performedValueEqToOne),
            [&bestSolutionPhase2](Assignment x)
                           { return bestSolutionPhase2->PerformedValue(x.getInterval()) == 1; } );


            if(!includeConstraints.empty()) {

                for(auto assign: performedValueEqToOne){

                  bool inInclConstr =   !std::any_of(includeConstraints.begin(), includeConstraints.end(),
                                [&](std::pair<Worker, Job> pair) {
                                    return pair.first.id() == assign.getWorker().id() &&
                                           pair.second.id() == assign.getJob().id();
                                });
                    if(inInclConstr) {
                        notIntIcludeConstraints.push_back(assign);

                    } else continue;


                }


//                std::copy_if(performedValueEqToOne.begin(), performedValueEqToOne.end(),
//                             std::back_inserter(notIntIcludeConstraints),
//                             [&](Assignment x) {
//                                 return !std::any_of(includeConstraints.begin(), includeConstraints.end(),
//                                                     [&](std::pair<Worker, Job> pair) {
//                                                         return pair.first.id() == x.getWorker().id() &&
//                                                                pair.second.id() == x.getJob().id();
//                                                     });
//                             });


                std::transform(notIntIcludeConstraints.begin(), notIntIcludeConstraints.end(), std::back_inserter(solution),
                               [&](Assignment x) { return std::make_pair(x.getWorker(), x.getJob()); });

            } else{


                for(Assignment a : performedValueEqToOne){
                    solution.push_back(std::make_pair(a.getWorker(), a.getJob()));
                }
            }








        return solution;
    }
