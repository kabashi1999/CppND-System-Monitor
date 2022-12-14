#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

// TODO: Return this process's ID
int Process::Pid() { return pid_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { return cpu_utilization_; }

// TODO: Return the command that generated this process
string Process::Command() { return command_; }

// TODO: Return this process's memory utilization
string Process::Ram() { return ram_; }

// TODO: Return the user (name) that generated this process
string Process::User() { return user_; }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return up_time_; }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const { 
return this->cpu_utilization_ < a.cpu_utilization_;
}

bool Process::operator>(Process const& a) const { 
return this->cpu_utilization_ > a.cpu_utilization_;
}

void Process::Pid(int pid) {
  pid_ = pid;
}

void Process::User(int pid) {
  user_ = LinuxParser::User(pid);
}

void Process::Command(int pid) {
  command_ = LinuxParser::Command(pid);
}

void Process::CpuUtilization(int pid) {
  float a_process = static_cast<float>(LinuxParser::ActiveJiffies(pid));
  float a_processor = static_cast<float>(LinuxParser::ActiveJiffies());
  cpu_utilization_ = a_process / a_processor;
}

void Process::Ram(int pid) {
  int kbytes = std::atoi(LinuxParser::Ram(pid).c_str());
  int mbytes = kbytes / 1000; // in MB
  ram_ = std::to_string(mbytes);
}

void Process::UpTime(int pid){
  up_time_ = LinuxParser::UpTime(pid);
}