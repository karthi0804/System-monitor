#include "processor.h"
#include "linux_parser.h"
#include <vector>
#include <string>

// DONE: Return the aggregate CPU utilization
float Processor::Utilization() {
    std::vector<std::string> cpu_vector = LinuxParser::CpuUtilization();
    float idle = std::stof(cpu_vector[4]) + std::stof(cpu_vector[5]);
    float non_idle = std::stof(cpu_vector[1]) + std::stof(cpu_vector[2]) + std::stof(cpu_vector[3]) + 
        std::stof(cpu_vector[6]) + std::stof(cpu_vector[7]) + std::stof(cpu_vector[8]);
    float total = idle + non_idle;
    float totald = total - Processor::prev_total_;
    float idled = idle - Processor::prev_idle_;
    Processor::prev_total_ = total;
    Processor::prev_idle_ = idle;
    return (totald -idled)/totald;
    }