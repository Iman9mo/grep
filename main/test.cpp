#include <iostream>
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
#include <ctime>  
#include <bits/stdc++.h>


using namespace std;

pthread_mutex_t readWriteMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t discoverFileMutex = PTHREAD_MUTEX_INITIALIZER;
set<string> matches;
set<string> discoveredFiles;

void* searchInFile(void* filename);
void processDirectory(const char* dirPath, const string& searchString, const string& searchRegex);


int main(int argc, char *argv[]) {
    const char* directoryPath = argv[1];
    string searchRegex = argv[2];
    string searchedString = argv[3];


    processDirectory(directoryPath, searchedString, searchRegex);

    for (auto i : matches) {
        cout << i << endl;
    }

    cout << "&&" << endl;

    for (auto i: discoveredFiles) {
        cout << i << endl;
    }

    return 0;
}
void *searchInFile(void *params)
{
    int startTime = clock(); 

    string* file = (string*)params;
    ifstream fileStream(*file);

    if (!fileStream) return NULL;
    
    string line;
    int lineNum = 1;
    string searchString = file[1];
    string searchRegex = file[2];

    while (getline(fileStream, line))
    {
        int lastFoundedIndex = 0;
        if(searchRegex == "noRegex") {
            while (true)
            {
                size_t foundedIndex = line.find(searchString);

                if(foundedIndex == string::npos) break;

                line = line.substr(foundedIndex + 1);
                lastFoundedIndex += foundedIndex;

                string result = *file + ":" + to_string(lineNum) + ":" + to_string(lastFoundedIndex);

                lastFoundedIndex++;

                pthread_mutex_lock(&readWriteMutex);
                matches.insert(result);
                pthread_mutex_unlock(&readWriteMutex);
            }
        } else {
            smatch match;
            regex r(searchRegex);
            while (regex_search(line, match, r)) {
                string result = *file + ":" + to_string(lineNum) + ":" + to_string(match.position(0) + 1);
                line = match.suffix().str();

                pthread_mutex_lock(&readWriteMutex);
                matches.insert(result);
                pthread_mutex_unlock(&readWriteMutex);
            }
        }
        lineNum++;
    }

    fileStream.close();

    int endTime = clock();
    int duration = endTime - startTime;

    pthread_mutex_lock(&discoverFileMutex);
    discoveredFiles.insert(to_string(duration) + ":" + *file);
    pthread_mutex_unlock(&discoverFileMutex);

    pthread_exit(NULL);
}

void processDirectory(const char* dirPath, const string& searchString, const string& searchRegex) {
    struct dirent *entry;
    DIR *dp = opendir(dirPath);

    if (dp == NULL) {
        perror("error in opening directory!");

        return;
    }

    while ((entry = readdir(dp))) {
        if (entry->d_type == DT_REG) {
            string filename = string(dirPath) + "/" + entry->d_name;
            pthread_t tid;

            string params[] = {filename, searchString, searchRegex};

            pthread_create(&tid, NULL, searchInFile, (void *)params);
            pthread_join(tid, NULL);
        }
        else if (entry->d_type == DT_DIR)
        {
            if (entry->d_name[0] != '.') {
                string subDirectortPath = string(dirPath) + "/" + entry->d_name;

                int resultLengthPipe[2];
                int discoverLengthPipe[2];
                int resultPipe[2];
                int discoverPipe[2];

                pipe(resultLengthPipe);
                pipe(resultPipe);
                pipe(discoverLengthPipe);
                pipe(discoverPipe);

                pid_t pid = fork();


                if (pid < 0) {
                    perror("fork failed");

                    exit(1);
                } else if (pid == 0) {
                    close(resultLengthPipe[0]);
                    close(resultPipe[0]);
                    close(discoverLengthPipe[0]);
                    close(discoverPipe[0]);

                    processDirectory(subDirectortPath.c_str(), searchString, searchRegex);

                    string resultWriteBuffer;
                    string discoverWriteBuffer;

                    for (auto i : matches)
                    {
                        resultWriteBuffer += i + "&&";
                    }

                    for(auto i : discoveredFiles) {
                        discoverWriteBuffer += i + "&&";
                    }

                    int resultBufferSize = strlen(resultWriteBuffer.c_str()) + 1;
                    int discoverBufferSize = strlen(discoverWriteBuffer.c_str()) + 1;

                    write(resultLengthPipe[1], to_string(resultBufferSize).c_str(), strlen(to_string(resultBufferSize).c_str()) + 1 );
                    write(resultPipe[1], resultWriteBuffer.c_str(), resultBufferSize);

                    write(discoverLengthPipe[1], to_string(discoverBufferSize).c_str(), strlen(to_string(discoverBufferSize).c_str()) + 1 );
                    write(discoverPipe[1], discoverWriteBuffer.c_str(), discoverBufferSize);

                    close(resultLengthPipe[1]);
                    close(resultPipe[1]);
                    close(discoverLengthPipe[1]);
                    close(discoverPipe[1]);

                    exit(0);
                }
                else
                {
                    close(resultLengthPipe[1]);
                    close(resultPipe[1]);
                    close(discoverLengthPipe[1]);
                    close(discoverPipe[1]);

                    int status;
                    wait(&status);

                    char resultLengthBuffer[255];
                    read(resultLengthPipe[0], resultLengthBuffer, sizeof(resultLengthBuffer));

                    char resultReadBuffer[stoi(resultLengthBuffer)];
                    read(resultPipe[0], resultReadBuffer, sizeof(resultReadBuffer));

                    char *resultReadPointer = strtok(resultReadBuffer, "&&");

                    pthread_mutex_lock(&readWriteMutex);
                    while (resultReadPointer != NULL)  
                    {
                        matches.insert(resultReadPointer);
                        resultReadPointer = strtok(NULL, "&&");
                    } 
                    pthread_mutex_unlock(&readWriteMutex);

                    char discoverLengthBuffer[255];
                    read(discoverLengthPipe[0], discoverLengthBuffer, sizeof(discoverLengthBuffer));

                    char discoverReadBuffer[stoi(discoverLengthBuffer)];
                    read(discoverPipe[0], discoverReadBuffer, sizeof(discoverReadBuffer));

                    char *discoverReadPointer = strtok(discoverReadBuffer, "&&");

                    pthread_mutex_lock(&discoverFileMutex);
                    while (discoverReadPointer != NULL)
                    {
                        discoveredFiles.insert(discoverReadPointer);
                        discoverReadPointer = strtok(NULL, "&&");
                    } 
                    pthread_mutex_unlock(&discoverFileMutex);

                    close(resultLengthPipe[0]);
                    close(resultPipe[0]);
                    close(discoverLengthPipe[0]);
                    close(discoverPipe[0]);
                }
            }
        }
    }
    closedir(dp);
}