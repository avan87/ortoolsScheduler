//
// Created by metis on 12/10/16.
//

#ifndef ORTOOLSSCHEDULER_UTILS_H
#define ORTOOLSSCHEDULER_UTILS_H
#include<iostream>
#include <ctime>
#include <iomanip>
#include <chrono>
#include <sstream>

static long getMillisValueFromDate(std::string time){

    time.replace(10, 1," ");
    std::tm tm = {};
    std::stringstream st(time);
    st >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
    auto tp = std::chrono::system_clock::from_time_t(std::mktime(&tm));
    long millisTime = tp.time_since_epoch().count();
    return millisTime;

}


#endif //ORTOOLSSCHEDULER_UTILS_H
