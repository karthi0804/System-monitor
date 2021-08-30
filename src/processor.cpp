#include "processor.h"
#include "linux_parser.h"
#include <vector>
#include <string>

// DONE: Return the aggregate CPU utilization
double Processor::Utilization() {
    std::vector<std::string> cpu_vector = LinuxParser::CpuUtilization();
    double idle = std::stod(cpu_vector[4]) + std::stod(cpu_vector[5]);
    double non_idle = std::stof(cpu_vector[1]) + std::stod(cpu_vector[2]) + std::stod(cpu_vector[3]) + 
        std::stod(cpu_vector[6]) + std::stod(cpu_vector[7]) + std::stod(cpu_vector[8]);
    double total = idle + non_idle;
    double totald = total - Processor::prev_total_;
    double idled = idle - Processor::prev_idle_;
    Processor::prev_total_ = total;
    Processor::prev_idle_ = idle;
    return (totald -idled)/totald;
    }