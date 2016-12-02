//
// Created by metis on 12/1/16.
//

#ifndef SCHEDULER_SCHEDULINGSETTINGSPROVIDER_H
#define SCHEDULER_SCHEDULINGSETTINGSPROVIDER_H


    class SchedulingSettingsProvider {

    public:

        SchedulingSettingsProvider(bool MaximizeFleetLoad,
        int WeightFactor,
        int IsoRefFactor,
        int RatingFactor,
        int TimeFactor,
        int TransitionFactor,
        int PayFactor,
        int LegFactor);

        bool isMaximizeFleetLoad() const;

        int getWeightFactor() const;

        int getIsoRefFactor() const;

        int getRatingFactor() const;

        int getTimeFactor() const;

        int getTransitionFactor() const;

        int getPayFactor() const;

        int getLegFactor() const;

    private:
        bool MaximizeFleetLoad ;
        int WeightFactor;
        int IsoRefFactor;
        int RatingFactor;
        int TimeFactor;
        int TransitionFactor;
        int PayFactor;
        int LegFactor ;

    };


#endif //SCHEDULER_SCHEDULINGSETTINGSPROVIDER_H
