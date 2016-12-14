//
// Created by metis on 12/7/16.
//
#include <grpc/grpc.h>
#include <grpc++/server.h>
#include <grpc++/server_builder.h>
#include <grpc++/server_context.h>
#include "../proto/scheduler.pb.h"
#include "../proto/scheduler.grpc.pb.h"
#include "../OrtoolsScheduler.h"


using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerReader;
using grpc::ServerReaderWriter;
using grpc::ServerWriter;
using grpc::Status;





class Scheduler : public Schedule::Service {

public:

    Scheduler(){

    }


    Status GetSchedule(ServerContext* context, const Data* data, ServerWriter<Pair>* writer  ) override {


        std::vector<Worker> workers;
        std::vector<Job> jobs;
        std::vector<std::pair<Worker,Job>> includeConstraints;
        std::vector<std::pair<Worker,Job>> excludeConstraints;
        std::vector<std::pair<Worker,Job>> priorityConstraints;

        std::copy(data->workers().begin(), data->workers().end(), std::back_inserter(workers));
        std::copy(data->jobs().begin(), data->jobs().end(), std::back_inserter(jobs));




        for (int i = 0; i < data->includeconstraints().pair_size(); ++i) {
            std::pair<Worker, Job> p(data->includeconstraints().pair(i).worker(), data->includeconstraints().pair(i).job());

            includeConstraints.push_back(p);
        }

        for (int i = 0; i < data->excludeconstraints().pair_size(); ++i) {
            std::pair<Worker, Job> p(data->excludeconstraints().pair(i).worker(), data->excludeconstraints().pair(i).job());

            excludeConstraints.push_back(p);
        }

        for (int i = 0; i < data->priorityconstraints().pair_size(); ++i) {
            std::pair<Worker, Job> p(data->priorityconstraints().pair(i).worker(), data->priorityconstraints().pair(i).job());

            priorityConstraints.push_back(p);
        }



        OrtoolsScheduler scheduler(data->schedulingsettingsprovider());
        std::vector<std::pair<Worker, Job>> scheduled = scheduler.Schedule(workers, jobs,
                                                                            includeConstraints,
                                                                            excludeConstraints,
                                                                             priorityConstraints);


        for (int i = 0; i < scheduled.size(); ++i) {
            Worker w = scheduled.at(i).first;
            Job j = scheduled.at(i).second;

            Pair p;
            p.mutable_job()->CopyFrom(j);
            p.mutable_worker()->CopyFrom(w);

            writer->Write(p);
        }
        return  Status::OK;

    }

private:

    std::vector<Pair> pairs_list_;

};


int main() {

    std::string server_address("localhost:50051");
    Scheduler service;

    ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);
    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;
    server->Wait();


    return 0;
}