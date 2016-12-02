//
// Created by metis on 11/30/16.
//
#include<iostream>
#include "SchedulingParameters.h"
#ifndef SCHEDULER_WORKER_H
#define SCHEDULER_WORKER_H


    class Worker {
        public:
            Worker(std::string id, int cost, std::string name, double x, double y, SchedulingParameters parameters);

        Worker();

        const std::string &getId() const;

        const SchedulingParameters &getParameters() const;

        int getCost() const;

        const std::string &getName() const;

        double getX() const;

        double getY() const;

    private:
            std::string id;
            int cost = 0;
            std::string name;
            double x = 0;
            double y = 0;
            SchedulingParameters parameters;


    };


#endif //SCHEDULER_WORKER_H
