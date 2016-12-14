//
// Created by metis on 11/30/16.
//
#include "constraint_solver/constraint_solver.h"
#include "proto/scheduler.pb.h"
#ifndef SCHEDULER_ASSIGNMENT_H
#define SCHEDULER_ASSIGNMENT_H



    class Assignment {
    public:

        Assignment(operations_research::IntervalVar *interval,
                   const std::vector<operations_research::IntVar *> &maximizeVars, const Worker &worker, const Job &job)
                : interval(interval), maximizeVars(maximizeVars), worker(worker), job(job) {

            this->cost = worker.cost() - job.cost();
            this->name = "{" + worker.name() + " :" + job.name() + "}";
        }

        Assignment(std::string n) : name("Empty") {

        }

        Assignment() {}

        operations_research::IntervalVar *getInterval() const {
            return interval;
        }

        void setPriority(int priority) {
            Assignment::priority = priority;
        }

        const std::vector<operations_research::IntVar *> &getMaximizeVars() const {
            return maximizeVars;
        }

        const Worker &getWorker() const {
            return worker;
        }

        const Job &getJob() const {
            return job;
        }

        int getCost() const {
            return cost;
        }

        int getPriority() const {
            return priority;
        }

        const std::string &getName() const {
            return name;
        }

    private:

        operations_research::IntervalVar *interval;
        std::vector<operations_research::IntVar *> maximizeVars;
        Worker worker;
        Job job;
        int cost = 0;
        int priority = 0;
        std::string name = "";


    };



#endif //SCHEDULER_ASSIGNMENT_H
