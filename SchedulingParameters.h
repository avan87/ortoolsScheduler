//
// Created by metis on 11/30/16.
//
#include<iostream>
#ifndef SCHEDULER_SCHEDULINGPARAMETERS_H
#define SCHEDULER_SCHEDULINGPARAMETERS_H



    class SchedulingParameters {
         public:
             SchedulingParameters(bool Ref, bool Iso, int raiting, std::string groupId);

        SchedulingParameters();

        bool isRef() const;

        bool isIso() const;

        int getRating() const;

        const std::string &getGroupId() const;

    private:
             bool Ref;
             bool Iso;
             int rating;
             std::string groupId;
    };


#endif //SCHEDULER_SCHEDULINGPARAMETERS_H
