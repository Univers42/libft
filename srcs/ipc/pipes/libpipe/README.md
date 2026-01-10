# Pipeline Utilities Library

## Overview
The `libpipe` library provides a set of utilities for managing inter-process communication (IPC) using pipes in a C programming environment. It simplifies the creation, management, and usage of pipes, allowing developers to focus on building their applications without worrying about the underlying complexities of pipe management.

## Features
- **Pipe Creation**: Easily create pipes with the `pipe_create` function, which returns a structure containing the read and write file descriptors.
- **Pipe Management**: Functions to close both ends of a pipe or a specific end, ensuring proper resource management.
- **Robust I/O Operations**: Utilities for reading and writing data to pipes, handling interruptions and ensuring complete data transfer.
- **Memory Management**: Integrated memory management functions to handle dynamic memory allocation and deallocation safely.
- **Non-blocking and Close-on-exec Flags**: Functions to set file descriptors to non-blocking mode and to set the close-on-exec flag, enhancing control over file descriptor behavior.

## Installation
To install the `libpipe` library, clone the repository and build the project using CMake or Makefile. Ensure that you have the necessary development tools installed on your system.

```bash
git clone <repository-url>
cd libpipe
mkdir build
cd build
cmake ..
make
```

## Usage
Refer to the `docs/usage.md` file for detailed usage instructions and examples on how to utilize the library's features in your applications.

## Testing
The library includes unit tests for critical functions to ensure reliability and correctness. You can run the tests using the provided CMake configuration in the `tests` directory.

## Contributing
Contributions are welcome! Please submit a pull request or open an issue to discuss improvements or bug fixes.

## License
This project is licensed under the MIT License. See the `LICENSE` file for more details.