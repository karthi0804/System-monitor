#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <iostream>
#include <iomanip>

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


template <typename T>
T findValueByKey(std::string const& filename, std::string const& filterkey){
 	string line, key;
    T value;
    std::ifstream filestream(filename);
    if (filestream.is_open()) {
      while (std::getline(filestream, line)) {
        std::istringstream linestream(line);
        while(linestream >> key >> value){
          if (key == filterkey){
          	return value;
          }
        }
      }
    }
  return value;
}


// DONE: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  std::ifstream meminfo_stream(kProcDirectory + kMeminfoFilename);
  float total = findValueByKey<float>(kProcDirectory + kMeminfoFilename, kfilterMemTotalString);
  float free = findValueByKey<float>(kProcDirectory + kMeminfoFilename, kfilterMemFreeString);
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

// DONE: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  vector<string> result;
  std::ifstream filestream{kProcDirectory + kStatFilename};
  string line, value;
  std::getline(filestream, line);
  std::istringstream linestream{line};
  while(linestream >> value){
    result.emplace_back(value);
  }
  return result; 
}

// DONE: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  int total_procs = findValueByKey<int>(kProcDirectory + kStatFilename, kfilterProcesses);
  return total_procs; 
  }

// DONE: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  int procs_running = findValueByKey<int>(kProcDirectory + kStatFilename, kfilterRunningProcesses);
  return procs_running; }

// DONE: Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  std::string line;
  if (filestream.is_open()){
    std::getline(filestream, line);
    }
 	if (line.size() >25){
      line = line.substr(0,22) + "...";
    }
  return line;}

// DONE: Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
  std::ostringstream ram{"0.0"};  // default value
  float proc_mem = findValueByKey<float>(kProcDirectory + std::to_string(pid) + kStatusFilename, kfilterProcMem);
  ram << std::setw(4) << std::setfill(' ')  << (int) ( proc_mem*0.001);
  return ram.str(); }

// DONE: Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  std::string line, word, uid;
  if (filestream.is_open()){
    while(std::getline(filestream, line)){
      std::istringstream linestream(line);
      while(linestream >> word){
        if(word == kfilterUID){
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
  string uid = LinuxParser::Uid(pid);
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      std::vector<string> words;
      while (linestream >> word) {
        words.emplace_back(word);
      }
      if (words[2] == uid && words[1] == "x") {
        std::replace(words[0].begin(), words[0].end(), '_', ' ');
        user = words[0];
        break;
      }
    }
  }
  return user.substr(0,5);
 }

std::vector<std::string> getWordsFromFile(string const& filename) {
  string line, word;
  std::ifstream filestream(filename);
  std::vector<string> words;
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);    
    while (linestream >> word) {
        words.emplace_back(word);
      }
  }
  return words;
}

// DONE: Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {
  std::vector<string> words = getWordsFromFile(kProcDirectory + std::to_string(pid) + kStatFilename);
  double time_ = UpTime() - std::stod(words[21]) /sysconf(_SC_CLK_TCK);
  long uptime = (long) lround(time_);
  return uptime; }

// DONE: Read and return the cpu utilization of a process
double LinuxParser::CpuProcess(int pid){
  std::vector<string> words = getWordsFromFile(kProcDirectory + std::to_string(pid) + kStatFilename);
  double total_time = (std::stod(words[13]) + std::stod(words[14]) + std::stod(words[15]) + std::stod(words[16])) / sysconf(_SC_CLK_TCK);
  double cpu = total_time/UpTime(pid);
  return cpu; }
