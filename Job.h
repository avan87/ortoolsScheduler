//
// Created by metis on 11/30/16.
//
#include<iostream>
#include "SchedulingParameters.h"

#ifndef SCHEDULER_JOB_H
#define SCHEDULER_JOB_H


    class Job {

        public:
            Job(std::string id, int cost, std::string name, double x, double y, SchedulingParameters parameters);

        Job();

        const std::string &getId() const;

        int getCost() const;

        const std::string &getName() const;

        double getX() const;

        double getY() const;

        const SchedulingParameters &getParameters() const;


        double getStart() const;

        double getEnd() const;

    private:
            std::string id;
            int cost = 0;
            std::string name;
            double start = 0;
            double end = 0;
            double x = 0;
            double y = 0;
            SchedulingParameters parameters;

    };


#endif //SCHEDULER_JOB_H
