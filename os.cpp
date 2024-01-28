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

using namespace std;

pthread_mutex_t readWriteMutex = PTHREAD_MUTEX_INITIALIZER;
set<string> matches;


void* searchInFile(void* filename);
void processDirectory(const char* dirPath, const string& searchString);


int main(int argc, char *argv[]) {
    string searchedString = argv[1];
    const char* directoryPath = argv[2];

    processDirectory(directoryPath, searchedString);

    for (auto i : matches) {
        cout << i << endl;
    }

    return 0;
}
void *searchInFile(void *params)
{
    string* file = (string*)params;
    ifstream fileStream(*file);

    if (!fileStream) return NULL;
    
    string line;
    int lineNum = 1;
    string searchString = file[1];

    while (getline(fileStream, line))
    {
        int lastFoundedIndex = 0;
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

        lineNum++;
    }

    fileStream.close();
    pthread_exit(NULL);
}

void processDirectory(const char* dirPath, const string& searchString) {
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

            string params[] = {filename, searchString};

            pthread_create(&tid, NULL, searchInFile, (void*)params);
            pthread_join(tid, NULL);
        }
        else if (entry->d_type == DT_DIR)
        {
            if (entry->d_name[0] != '.') {
                string subDirectortPath = string(dirPath) + "/" + entry->d_name;

                int lengthPipe[2];
                int resultPipe[2];

                pipe(lengthPipe);
                pipe(resultPipe);

                pid_t pid = fork();


                if (pid < 0) {
                    perror("fork failed");

                    exit(1);
                } else if (pid == 0) {
                    close(lengthPipe[0]);
                    close(resultPipe[0]);

                    processDirectory(subDirectortPath.c_str(), searchString);

                    string writeBuffer;

                    for (auto i : matches)
                    {
                        writeBuffer += i + "&&";
                    }

                    int bufferSize = strlen(writeBuffer.c_str()) + 1;

                    write(lengthPipe[1], to_string(bufferSize).c_str(), strlen(to_string(bufferSize).c_str()) + 1 );
                    write(resultPipe[1], writeBuffer.c_str(), bufferSize);

                    close(lengthPipe[1]);
                    close(resultPipe[1]);

                    exit(0);
                }
                else
                {
                    close(lengthPipe[1]);
                    close(resultPipe[1]);

                    int status;
                    wait(&status);

                    char lengthBuffer[255];
                    read(lengthPipe[0], lengthBuffer, sizeof(lengthBuffer));

                    char readBuffer[stoi(lengthBuffer)];
                    read(resultPipe[0], readBuffer, sizeof(readBuffer));

                    char *readPointer = strtok(readBuffer, "&&");

                    pthread_mutex_lock(&readWriteMutex);
                    while (readPointer != NULL)  
                    {
                        matches.insert(readPointer);
                        readPointer = strtok(NULL, "&&");
                    } 
                    pthread_mutex_unlock(&readWriteMutex);

                    close(lengthBuffer[0]);
                    close(resultPipe[0]);
                }
            }
        }
    }
    closedir(dp);
}