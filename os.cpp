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
#include <vector>

using namespace std;

pthread_mutex_t readWriteMutex = PTHREAD_MUTEX_INITIALIZER;
vector<string> matches;


void* searchInFile(void* filename);
void processDirectory(const char* dirPath, const string& searchString);


int main(int argc, char *argv[]) {
    string searchedString = argv[1];
    const char* directoryPath = argv[2];

    processDirectory(directoryPath, searchedString);

    for (int i = 0; i < matches.size(); i++) {
        cout << matches[i] << endl;
    }

    return 0;
}

void* searchInFile(void* params) {
    string* file = (string*)params;
    ifstream fileStream(*file);

    if (!fileStream) return NULL;
    
    string line;
    int lineNum = 1;
    string searchString = file[2];
    string processId = file[1];

    while (getline(fileStream, line))
    {
        size_t foundedIndex = line.find(searchString);

        if (foundedIndex != string::npos) {
            string result = *file + ":" + to_string(lineNum) + ":" + to_string(foundedIndex);

            pthread_mutex_lock(&readWriteMutex);
            matches.push_back(result);
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

            pid_t currentProcessId = getpid();

            string params[] = {filename, to_string(currentProcessId), searchString};

            pthread_create(&tid, NULL, searchInFile, (void*)params);
            pthread_join(tid, NULL);
        }
        else if (entry->d_type == DT_DIR)
        {
            if (entry->d_name[0] != '.') {
                string subDirectortPath = string(dirPath) + "/" + entry->d_name;

                int pipeLine[2];
                pipe(pipeLine);

                pid_t pid = fork();


                if (pid < 0) {
                    perror("fork failed");

                    exit(1);
                } else if (pid == 0) {
                    close(pipeLine[0]);

                    processDirectory(subDirectortPath.c_str(), searchString);

                    for (int i = 0; i < matches.size(); i++) {
                        write(pipeLine[1], matches[i].c_str(), (strlen(matches[i].c_str()) + 1));
                    }
                    
                    close(pipeLine[1]);

                    exit(0);
                }
                else
                {
                    close(pipeLine[1]);
                    int status;

                    char readBuffer[200];
                    int nbytes;

                    while ((nbytes = read(pipeLine[0], readBuffer, sizeof(readBuffer))) > 0)
                    {
                        matches.push_back(readBuffer);
                    }

                    wait(&status);
                    
                    close(pipeLine[0]);
                }
            }
        }
    }
    closedir(dp);
}
