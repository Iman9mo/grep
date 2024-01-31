# grep file search

The purpose of this project is implementing a program like grep program in linux using multi-thread. This tool is using for searching words in text files.

## project description

This C++ program traverses a directory tree, searches for a specified string or regular expression in text files, and reports the matching lines with their corresponding file names, line numbers, and character positions. Additionally, it measures the time taken to process each file and outputs the duration along with the file information.

### features

```
1.Multithreading: The program uses pthreads for parallel processing of files within a directory. Each file is processed concurrently to improve performance.

2.Search Options: You can perform a simple string search or a search using a regular expression. The choice is made by providing a regular expression as a command-line argument. If "noRegex" is provided, a simple string search is performed.

3.Output Format: Matching lines are reported in the format: filename:line_number:character_position. The matched lines are stored in a set to eliminate duplicates.

4.Execution Time: The program measures the execution time for processing each file and outputs the duration along with the file information. This information is stored in a separate set.

5.Directory Traversal: The program recursively traverses subdirectories, creating child processes to handle each subdirectory concurrently.

6.Search with regex: you can  search about a special pattern with regex.

7.IPC: Inter-Process Communication (IPC) is a crucial concept in computing that refers to the mechanisms and techniques used for communication and data exchange between separate processes running on a computer system. IPC is essential for enabling collaboration and coordination between different processes, allowing them to share information, synchronize activities, and achieve more complex tasks collectively.

Key Aspects of IPC:

1. **Communication Between Processes:**
   - IPC facilitates communication between processes, which can be running on the same or different machines. Processes may need to exchange data, share resources, or coordinate actions, and IPC provides the means to achieve these interactions.

2. **Inter-Thread vs. Inter-Process Communication:**
   - While threads within the same process can communicate through shared memory and direct function calls, IPC is specifically designed for communication between separate processes. IPC mechanisms are crucial for processes that operate independently and may have different memory spaces.

3. **Types of IPC Mechanisms:**
   - IPC encompasses various mechanisms, including message passing, shared memory, sockets, pipes, and more. Each mechanism has its strengths and use cases. For instance, message passing is useful for asynchronous communication, while shared memory is efficient for rapid data exchange.

4. **Synchronization and Coordination:**
   - IPC helps in synchronizing the activities of different processes to avoid conflicts and ensure orderly execution. Techniques like semaphores, mutexes, and condition variables are often used to coordinate the timing of processes.

5. **Named Pipes and Sockets:**
   - Named pipes and sockets are common IPC mechanisms for communication between processes on the same or different machines. They provide a channel for data to flow between processes, and protocols like TCP/IP can be employed for communication over networks.

6. **Security and Access Control:**
   - IPC mechanisms often include security features and access control to regulate which processes can communicate with each other. This helps prevent unauthorized access and ensures the integrity and confidentiality of the exchanged data.

7. **IPC in Modern Software Development:**
   - In contemporary software development, IPC is fundamental to building distributed systems, microservices architectures, and other complex applications where multiple processes need to collaborate. Technologies like Remote Procedure Call (RPC) and Message Queueing are also common in IPC scenarios.

8. **Error Handling and Reliability:**
   - IPC mechanisms often incorporate error handling mechanisms to deal with communication failures and ensure the reliability of data exchange. This is crucial for maintaining the integrity of applications, especially in distributed environments.

```

### about UI framework

Electron is an open-source framework that enables developers to build cross-platform desktop applications using web technologies such as HTML, CSS, and JavaScript. It combines the Chromium rendering engine and the Node.js runtime, providing a powerful and flexible platform for creating desktop applications with web technologies.

Key Features of Electron:

1. **Cross-Platform Development:**

   - Electron allows developers to create applications that run seamlessly on major operating systems, including Windows, macOS, and Linux, using a single codebase.

2. **Web Technologies:**

   - Developers can leverage their existing web development skills to build desktop applications. Electron applications are essentially web applications wrapped in a native shell, providing a familiar development environment.

3. **Chromium Engine:**

   - Electron utilizes the Chromium rendering engine, the same engine that powers the Google Chrome browser. This ensures consistent and high-performance rendering of web content in desktop applications.

4. **Node.js Integration:**

   - Electron seamlessly integrates with Node.js, enabling developers to use server-side JavaScript to handle backend logic and perform file system operations. This integration allows for a wide range of capabilities beyond traditional web applications.

5. **Rich Desktop Capabilities:**

   - Electron applications have access to native desktop features and APIs, allowing developers to create applications with rich user interfaces, system notifications, file system access, and more.

6. **Extensibility:**

   - Electron supports a vibrant ecosystem of plugins and extensions that enhance the functionality of applications. This extensibility makes it easy for developers to integrate third-party libraries and tools into their projects.

7. **Developer Tools:**

   - Electron includes built-in developer tools, similar to those found in web browsers, making it easier to debug and optimize applications during development.

8. **Active Community and Documentation:**
   - Electron has a large and active community of developers, contributing to ongoing improvements, bug fixes, and the development of additional tools. Extensive documentation and community support are available for developers at all skill levels.

By leveraging Electron, developers can create powerful, cross-platform desktop applications with a familiar web development workflow, making it an excellent choice for projects ranging from small utilities to complex, feature-rich applications.

## output

![Sample Image](https://github.com/Iman9mo/grep/blob/37366b74fb9605deca5beddf5374531a25d2f29d/Screenshot%20from%202024-01-30%2020-54-24.png)

> [!NOTE]
> Make sure to compile the program with C++11 support.
> The pthread library is required.
> The program uses the clock() function to measure execution time, which may not provide precise timing information. Consider using more accurate timers for production-level profiling.

## overview of the main components and functionality:

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

## how to run project

### Install the dependencies

```bash
yarn
# or
npm install
```

### Start the app in development mode (hot-code reloading, error reporting, etc.)

```bash
quasar dev
```

### Lint the files

```bash
yarn lint
# or
npm run lint
```

### Format the files

```bash
yarn format
# or
npm run format
```

### Build the app for production

```bash
quasar build
```

### Customize the configuration

See [Configuring quasar.config.js](https://v2.quasar.dev/quasar-cli-vite/quasar-config-js).
