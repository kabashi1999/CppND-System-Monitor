#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>

#include "linux_parser.h"
#include "format.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

//my defined functions
string LinuxParser::ValueFromKey(string search_key,string filePath){
  std::string line, key, str_value;
  std::ifstream fileStream(filePath);

  if (fileStream) {
    while (getline(fileStream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);

      while (linestream >> key >> str_value) {
        if (key == search_key) return str_value;
      }
    }
  }
  fileStream.close();
  return "";

}

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

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  vector<string> memory;
  string line,extra,value;
  float total_memory,free_memory,utilized_memory_percentage=0;
   std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if(stream.is_open()){
    for (int i = 0; i < 2; i++)
    {
      std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> extra >> value>>extra; 
    memory.push_back(value);
    }   
  }
   total_memory = std::stof(memory[0]);
   free_memory = std::stof(memory[1]);
  utilized_memory_percentage = (total_memory-free_memory)/total_memory;
  return utilized_memory_percentage; 
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
    string line, up_time;
  long time = 0;

  std::ifstream stream(kProcDirectory + kUptimeFilename);

  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> up_time;
    time = std::atol(up_time.c_str());
  }
  stream.close();
  return time;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  vector<string> jiffies = CpuUtilization();
long total_jiffies = 0;
for (string jiffie : jiffies)
{
  if(jiffie != "")
    total_jiffies += std::stol(jiffie);
}
return total_jiffies;
 }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) { 
  long proc_active_jiffies = 0;
string user_code_time, kernel_code_time, line, extra;
std::ifstream stream(kProcDirectory + std::to_string(pid)+ kStatFilename);
if (stream.is_open())
{
    std::getline(stream, line);
    std::istringstream linestream(line);
    for (int i = 0; i < 13; ++i)
    {
        linestream >> extra;
    }
    linestream >> user_code_time >> kernel_code_time;
}
  proc_active_jiffies = std::atol(user_code_time.c_str()) + std::atol(kernel_code_time.c_str());
//proc_active_jiffies = std::stol(user_code_time) + std::stol(kernel_code_time);
return proc_active_jiffies;
 }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  long s_active_jiffies = 0;
s_active_jiffies = Jiffies() - IdleJiffies();
return s_active_jiffies;
 }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  vector<string> jiffies = CpuUtilization();
long idle_jiffies = 0;
  long iowait = std::atol(jiffies[4].c_str());
  long idle = std::atol(jiffies[3].c_str());
idle_jiffies = idle + iowait;
return idle_jiffies;
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  vector<string>aggregated_time;
  string line,cpu,time;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if(stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream>>cpu;
    for (int i = 0; i < 10; i++)
    {
      linestream>>time;
      aggregated_time.push_back(time);
    }
  }
    
  return aggregated_time; }

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  std::string line, key;
  int value;
  std::ifstream filestream(kProcDirectory + kStatFilename);

  if (filestream) {
    while (getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes") return value;
      }
    }
  }
  filestream.close();
  return 0;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string line, key;
  int value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream) {
    while (getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running") return value;
      }
    }
  }

  filestream.close();
  return 0;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
  std::string command;

  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if (filestream) 
    getline(filestream, command);
  filestream.close();
  return command;
 }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
  string ram = "",path = kProcDirectory+std::to_string(pid)+kStatusFilename;
  ram = LinuxParser::ValueFromKey("VmSize",path);
  auto value_in_mb = std::atol(ram.c_str())/1024;
  return std::to_string(value_in_mb);
 }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
  string uid = "",path = kProcDirectory+std::to_string(pid)+kStatusFilename;
  uid = LinuxParser::ValueFromKey("Uid",path);
  return uid;
 }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 
  string key = LinuxParser::Uid(pid);
  string line, uid_str, user;

  std::ifstream fileStream(kPasswordPath);

  if (fileStream) {
    while (getline(fileStream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), 'x', ' ');
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);

      while (linestream >> user >> uid_str) {
        if (uid_str == key) return user;
      }
    }
  }
  fileStream.close();
  return user;
 }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 
  long ticks = 0;
  string temp;
  string line,extra;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line); 
    for(int i = 0; i < 21; ++i) {
      linestream >> extra;
    }
    linestream >> temp;
  }
  ticks = std::atol(temp.c_str())/sysconf(_SC_CLK_TCK);
  return ticks;}
