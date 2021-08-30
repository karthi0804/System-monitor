#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

System::System(){
  cpu_ = Processor();
  System::Set_Processes();
}

// DONE: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

void System::SortProcesses() {
  sort(processes_.begin(), processes_.end(), [](Process proc_l , Process proc_r) { return proc_l < proc_r;});
  reverse(processes_.begin(), processes_.end());
}

// DONE: Return a container composed of the system's processes
vector<Process>& System::Processes() {
  System::SortProcesses();
  return processes_; }

void System::Set_Processes(){;
    vector<int> pids = LinuxParser::Pids();
    for (auto i : pids){
        processes_.emplace_back(i);
    }
}

// DONE: Return the system's kernel identifier (string)
std::string System::Kernel() { return LinuxParser::Kernel(); }

// DONE: Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// DONE: Return the operating system name
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

// DONE: Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// DONE: Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// DONE: Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }