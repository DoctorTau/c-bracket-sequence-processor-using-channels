#ifndef FILE_IO_H
#define FILE_IO_H

#include <string.h>

/// @brief Read string from file.
/// @param file_name file name
/// @param buffer_string string to write content from file
void readFromFile(const char* file_name, char* buffer_string);

/// @brief Write string to file.
/// @param file_name file name
/// @param buffer_string string to write to file
void writeToFile(const char* file_name, const char* buffer_string);

#endif  // FILE_IO_H
