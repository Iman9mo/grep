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
