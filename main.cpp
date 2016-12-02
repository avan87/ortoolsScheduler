#include <iostream>
#include "worker.h"
#include "Job.h"
#include "Assignment.h"
#include "Assignments.h"
#include <ctime>


    int main() {


        std::cout << "Hello, World!" << std::endl;

        operations_research::Solver solver("solver");

        SchedulingParameters schedulingParameters(true, true, 3, "id");

        std::vector<Worker> workers{
                Worker ("111", 2, "w", 2.0, 3.0, schedulingParameters),
                Worker ("1213", 1, "w", 2.0, 3.0, schedulingParameters),
                Worker ("1223", 12, "w", 2.0, 3.0, schedulingParameters),
                Worker ("1233", 112, "w", 2.0, 3.0, schedulingParameters)
        };

        std::vector<Job> jobs{
                Job("1111", 12, "name", 13, 31, schedulingParameters),
                Job("1231", 12, "name", 13, 31, schedulingParameters),
                Job("1232", 12, "name", 13, 31, schedulingParameters),
                Job("1233", 12, "name", 13, 31, schedulingParameters),
                Job("1234", 12, "name", 13, 31, schedulingParameters)
        };

        Job job("123", 12, "name", 13, 31, schedulingParameters);


        std::vector<std::pair<Worker, Job>> excludeConstraints{
                std::make_pair( Worker ("111", 2, "w", 2.0, 3.0, schedulingParameters),
                               Job("1111", 12, "name", 13, 31, schedulingParameters))
        };
        std::vector<std::pair<Worker, Job>> includeConstraints;
        std::vector<std::pair< Worker, Job>> priorityConstraints;

        SchedulingSettingsProvider schedulingSettingsProvider(true,1, 1, 1, 1, 1, 1, 1);

        Assignments assignments(workers, jobs, includeConstraints,
        excludeConstraints, priorityConstraints, schedulingSettingsProvider);

     //   assignments.addConstraints(excludeConstraints, includeConstraints, priorityConstraints);


        for (int i = 0; i < assignments.getMatrix().size(); ++i) {
            for (int j = 0; j < assignments.getMatrix().at(0).size(); ++j) {

                    std::cout << assignments.getMatrix()[i][j].getName() << " ";

            }
            std::cout <<  std::endl;

        }

        std::chrono::time_point<std::chrono::system_clock> time = std::chrono::system_clock::now();

        time_t localtime = std::chrono::system_clock::to_time_t(time);

        std::cout << ctime(&localtime);//






        return 0;
    }
