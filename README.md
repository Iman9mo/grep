# grep file search

The purose of this project is implementing a program like grep program in linux using multi-thread. This tool is using for searching words in text files.

## project description

This C++ program traverses a directory tree, searches for a specified string or regular expression in text files, and reports the matching lines with their corresponding file names, line numbers, and character positions. Additionally, it measures the time taken to process each file and outputs the duration along with the file information.

### features

```
1.Multithreading: The program uses pthreads for parallel processing of files within a directory. Each file is processed concurrently to improve performance.

2.Search Options: You can perform a simple string search or a search using a regular expression. The choice is made by providing a regular expression as a command-line argument. If "noRegex" is provided, a simple string search is performed.

3.Output Format: Matching lines are reported in the format: filename:line_number:character_position. The matched lines are stored in a set to eliminate duplicates.

4.Execution Time: The program measures the execution time for processing each file and outputs the duration along with the file information. This information is stored in a separate set.

5.Directory Traversal: The program recursively traverses subdirectories, creating child processes to handle each subdirectory concurrently.
```

### output

![Sample Image](https://github.com/Iman9mo/grep/blob/37366b74fb9605deca5beddf5374531a25d2f29d/Screenshot%20from%202024-01-30%2020-54-24.png)

> [!NOTE]
> Make sure to compile the program with C++11 support.
> The pthread library is required.
> The program uses the clock() function to measure execution time, which may not provide precise timing information. Consider using more accurate timers for production-level profiling.

### overview of the main components and functionality:

**Global Variables**:
pthread_mutex_t readWriteMutex: Mutex for protecting the critical section where the matches set is updated.
pthread_mutex_t discoverFileMutex: Mutex for protecting the critical section where the discoveredFiles set is updated.
set<string> matches: Set to store the matched results (file paths, line numbers, and positions).
set<string> discoveredFiles: Set to store the duration and file paths of discovered files.

**searchInFile Function**:
This function is executed by each thread to search for a string or pattern in a specific file.
It takes a filename, search string, and search regex as parameters.
If searchRegex is set to "noRegex," it searches for the string using string::find.
If searchRegex is not "noRegex," it uses regular expressions (std::regex) to find matches.
The matching results are added to the matches set.
The duration of the search is calculated and added to the discoveredFiles set.

**processDirectory Function**:
This function processes a directory and its subdirectories recursively.
For each regular file, a new thread is created to search for the specified string/pattern using pthread_create.
For each subdirectory, a new process is created using fork, and the search is performed recursively in the child process.
Inter-process communication is achieved using pipes (pipe function) to exchange the results between parent and child processes.
The matching results from child processes are added to the matches set, and the discovered files' duration and paths are added to the discoveredFiles set.
The function utilizes mutexes to protect shared data structures (matches and discoveredFiles sets).

**main Function**:
The main function takes command-line arguments for the directory path, search regex, and searched string.
It calls the processDirectory function to initiate the search.
After the search is complete, it prints the matching results and discovered files' information.
