<br />
<p align="center">
   <a href="https://github.com/StarPandaBeg/GooseVF-CLI">
    <img src=".github/logo.png" alt="Logo" width="128" />
   </a>

   <h3 align="center">GooseVF CLI - Command Line Tool</h3>

   <p align="center">
      Tool for GooseVF honk archives
   </p>
</p>

---

This is a CLI tool for GooseVF library that provides ability to create & extrach `.honk` archives.

See more on [GooseVF GitHub page](https://github.com/StarPandaBeg/GooseVF).

> This project is designed primarily for educational purposes, although it may have practical uses as well

# Usage

```bash
goosevf-cli [OPTION...] <archive_name> [files or directories to archive]

   -h, --help                    Show help
   -c, --create                  Create archive
   -x, --extract                 Extract archive
   -t, --list                    List files inside archive
   -k, --keep-old-files          Do not replace existing files when extracting
   -C, --directory <path>        Change to DIR before performing any operations
   --file-version <version>      File content version (default: 0)
```

## Examples

1. Create archive

   ```bash
   goosevf-cli -c archive.honk test.txt otherdir/
   ```

1. View archive contents

   ```bash
   goosevf-cli -t archive.honk
   ```

1. Extract archive to current folder
   ```bash
   goosevf-cli -x archive.honk
   ```

# Build

### Requirements

- CMake >= 3.16

To build the project please follow the instructions below:

```bash
git clone https://github.com/StarPandaBeg/GooseVF-CLI.git
cd GooseVF-CLI
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

# License

Distributed under the MIT License.  
See `LICENSE` for more information.

# Disclaimer

This project is created by C++ beginner developer. It may contain errors or inaccuracies. I would appreciate your feedback and suggestions for improvement. Thanks! 💗
