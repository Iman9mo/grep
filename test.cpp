#include <iostream>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cstring>
#include <string>
#include <fstream>
#include <thread>
#include <mutex>
#include <map>
#include <set>
#include <vector>

#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cstring>
#include <string>
#include <fstream>
#include <pthread.h>
#include <sys/wait.h>
#include <map>
#include <set>
#include <vector>

using namespace std;

mutex readWriteMutex;
set<string> matches;

void searchInFile(const string &filename, const string &searchString);
void processDirectory(const char *dirPath, const string &searchString);

int main(int argc, char *argv[])
{
  string searchedString = argv[1];
  const char *directoryPath = argv[2];

  processDirectory(directoryPath, searchedString);

  for (const auto &i : matches)
  {
    cout << i << endl;
  }

  return 0;
}

void searchInFile(const string &filename, const string &searchString)
{
  ifstream fileStream(filename);

  if (!fileStream)
    return;

  string line;
  int lineNum = 1;

  while (getline(fileStream, line))
  {
    int lastFoundedIndex = 0;
    while (true)
    {
      size_t foundedIndex = line.find(searchString);

      if (foundedIndex == string::npos)
        break;

      line = line.substr(foundedIndex + 1);
      lastFoundedIndex += foundedIndex;

      string result = filename + ":" + to_string(lineNum) + ":" + to_string(lastFoundedIndex);

      lastFoundedIndex++;

      lock_guard<mutex> lock(readWriteMutex);
      matches.insert(result);
    }

    lineNum++;
  }

  fileStream.close();
}

void processDirectory(const char *dirPath, const string &searchString)
{
  struct dirent *entry;
  DIR *dp = opendir(dirPath);

  if (dp == NULL)
  {
    perror("error in opening directory!");
    return;
  }

  vector<thread> threads;
  while ((entry = readdir(dp)))
  {
    if (entry->d_type == DT_REG)
    {
      string filename = string(dirPath) + "/" + entry->d_name;
      threads.emplace_back(searchInFile, filename, searchString);
    }
    else if (entry->d_type == DT_DIR)
    {
      if (entry->d_name[0] != '.')
      {
        string subDirectortPath = string(dirPath) + "/" + entry->d_name;
        processDirectory(subDirectortPath.c_str(), searchString);
      }
    }
  }
  closedir(dp);

  for (auto &thread : threads)
  {
    if (thread.joinable())
    {
      thread.join();
    }
  }
}