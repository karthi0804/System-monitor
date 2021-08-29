#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <iostream>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// DONE: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  std::ifstream meminfo_stream(kProcDirectory + kMeminfoFilename);
  string line, key, value, unit;
  float total{0}, free{0};
  if (meminfo_stream.is_open()){
    for (int i =0; i<2; i++){  
      std::getline(meminfo_stream, line);
      std::istringstream linestream{line};            
      linestream >> key >> value>> unit;
      if(key == "MemTotal:")total = stof(value);
      else if (key=="MemFree:")free = stof(value);    
    }
  }
  float util = (total - free)/(total+0.00001);
  return util;
}

// DONE: Read and return the system uptime
long LinuxParser::UpTime() {  
  long uptime=0;
  std::ifstream uptimefile{kProcDirectory + kUptimeFilename};
  string line, sus_time, idle_time;
  if (uptimefile.is_open()){
    std::getline(uptimefile, line);
    std::istringstream linestream(line);
    linestream >> sus_time >> idle_time;
    double time_ = std::stod(sus_time);
    uptime = (long) lround(time_);
  }
  return uptime; 
}

// NOTNEEDED: Read and return the number of jiffies for the system
//long LinuxParser::Jiffies() { return 0; }

// NOTNEEDED: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
//long LinuxParser::ActiveJiffies(int pid) { return 0; }

// NOTNEEDED: Read and return the number of active jiffies for the system
// long LinuxParser::ActiveJiffies() { return 0; }

// NOTNEEDED: Read and return the number of idle jiffies for the system
// long LinuxParser::IdleJiffies() { return 0; }

// DONE: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  vector<string> result;
  std::ifstream filestream{kProcDirectory + kStatFilename};
  string line, value;
  std::getline(filestream, line);
  std::istringstream linestream{line};
  while(linestream >> value){
    result.push_back(value);
  }
  return result; 
}

// DONE: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  int total_procs = 0;
  string line, key, value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while(linestream >> key >> value){
        if (key == "processes"){
          total_procs = std::stoi(value);
        }
      }
    }
  }
  return total_procs; 
  }

// DONE: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string line, key, value;
  int procs_running;
  std::ifstream statstream(kProcDirectory + kStatFilename);
  if (statstream.is_open()){
    while(std::getline(statstream, line)){
      std::istringstream linestream(line);
      while(linestream >> key >> value){
        if (key == "procs_running"){
          procs_running = std::stoi(value);
        }
      }
    }
  }
  return procs_running; }

// DONE: Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  std::string line;
  if (filestream.is_open()){
    std::getline(filestream, line);
    }
  return line;}

// DONE: Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  std::string line, key, value;
  std::ostringstream ram;
  ram.precision(2);
  if (filestream.is_open()){
    while(std::getline(filestream, line)){
      std::istringstream linestream(line);
      while(linestream >> key >> value){
        if(key == "VmSize:"){          
          ram << std::fixed << std::stof(value)*0.001;
        }
      }
    }
  }
  return ram.str(); }

// DONE: Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  std::string line, word, uid;
  if (filestream.is_open()){
    while(std::getline(filestream, line)){
      std::istringstream linestream(line);
      while(linestream >> word){
        if(word == "Uid:"){
          linestream >> uid;
        }
      }
    }
  }
  return uid; }

// DONE: Read and return the user associated with a process
string LinuxParser::User(int pid) {
  string line;
  string word;
  string user;
  std::vector<string> words;
  string uid = LinuxParser::Uid(pid);
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), ':', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> word) {
        words.push_back(word);
      }
      if (words[2] == uid && words[1] == "x") {
        std::replace(words[2].begin(), words[2].end(), '_', ' ');
        user = words[0];
        break;
      }
    }
  }
  return user;
 }

// DONE: Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatFilename);
  string line;
  string word;
  long uptime;
  std::vector<string> words;
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);    
    while (linestream >> word) {
        words.push_back(word);
      }
    double time_ = std::stod(words[21]);
    uptime = (long) lround(time_);
  }
  return uptime; }

// DONE: Read and return the cpu utilization of a process
double LinuxParser::CpuProcess(int pid){
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatFilename);
  string line;
  string word;
  double cpu;
  double uptime = (double) LinuxParser::UpTime(pid);
  std::vector<string> words;
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);    
    while (linestream >> word) {
        words.push_back(word);
      }
    double total_time = (std::stod(words[13]) + std::stod(words[14]) + std::stod(words[15]) + std::stod(words[16])) / sysconf(_SC_CLK_TCK);
    double elapsed_time = uptime - (double) (std::stod(words[21]) / sysconf(_SC_CLK_TCK));
    cpu = 100 * (total_time/elapsed_time);
  }
  return cpu; }
