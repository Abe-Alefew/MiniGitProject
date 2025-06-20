# MiniGit: A Custom Version Control System

A lightweight Git-like version control system implemented in C++ that tracks changes to files, supports commits, branching, and log history — all from scratch without using external version control libraries.

## 📋 Project Overview

MiniGit simulates a local-only Git-like experience via command-line operations, demonstrating how real-world systems like Git are underpinned by data structure principles such as hashing, trees, and DAGs (Directed Acyclic Graphs).

## 🎯 Learning Objectives

This project demonstrates:

- **Complex Data Structure Implementation**: Custom DAGs and tree structures for version control
- **Git Internals Understanding**: How Git works under the hood with objects, refs, and staging
- **File I/O and Persistence**: Efficient file handling and data serialization using JSON
- **Command-Line Interface Development**: Robust CLI with multiple commands and error handling
- **Software Engineering Practices**: Modular code organization, versioning, and system design
- **Cryptographic Hashing**: SHA-1 implementation for content integrity and deduplication

## ✅ Implemented Features

### Core Functionality

- ✅ **Repository Initialization (`init`)**: Creates `.minigit/` directory structure with proper organization
- ✅ **File Staging (`add <filename>`)**: Stages files with SHA-1 content hashing and blob storage
- ✅ **Commit System (`commit -m <message>`)**: Creates commit objects with metadata and file snapshots
- ✅ **Commit History (`log`)**: Traverses and displays commit history from HEAD backward
- ✅ **Branch Management (`branch [branch-name]`)**: Create and list branches with proper reference management
- ✅ **Branch Switching (`checkout <branch-name>`)**: Switch between branches and update HEAD pointer

### Data Structures & Storage

- **Commit Objects**: Structured with hash, parent, message, timestamp, and file mappings
- **Blob Storage**: Content-addressable storage using SHA-1 hashes for deduplication
- **Branch References**: File-based branch pointers stored in `.minigit/branches/`
- **Staging Area**: JSON-based staging mechanism for tracking pending changes
- **HEAD Management**: Proper HEAD pointer management for current branch/commit tracking

## 🏗️ Architecture & Implementation

### Directory Structure

```
.minigit/
├── HEAD                    # Points to current branch/commit
├── branches/              # Branch reference files
│   └── main              # Default branch pointer
├── commits/              # Commit objects (JSON format)
│   └── <hash>.json       # Individual commit files
├── blobs/                # File content storage
│   └── <hash>            # Content-addressable blobs
└── staging_area.json     # Staged files tracking
```
### Core Components

#### 1. Repository Class (`src/core/repository.cpp`)

- Central coordination for all repository operations
- Handles initialization, file operations, and state management
- Implements static methods for command execution

#### 2. Commit System (`src/core/commit.cpp`)

- Commit object serialization/deserialization
- Parent-child relationship management
- Metadata handling (timestamp, message, file mappings)

#### 3. Blob Management (`src/core/blob.cpp`)

- Content-addressable storage using SHA-1 hashing
- Automatic deduplication of identical file content
- Efficient file content management

#### 4. Branch Management (`src/core/branch.cpp`)

- Branch creation and listing functionality
- Branch existence validation
- HEAD pointer management for branch switching

#### 5. Cryptographic Hashing (`src/utils/hashing.cpp`)

- SHA-1 implementation using OpenSSL
- Content integrity verification
- Unique identifier generation for objects

### Command Implementation

| Command    | File                        | Status             | Description                       |
| ---------- | --------------------------- | ------------------ | --------------------------------- |
| `init`     | `src/commands/init.cpp`     | ✅ Complete        | Initialize repository structure   |
| `add`      | `src/commands/add.cpp`      | ✅ Complete        | Stage files for commit            |
| `commit`   | `src/commands/commit.cpp`   | ✅ Complete        | Create commit with staged changes |
| `log`      | `src/commands/log.cpp`      | ✅ Complete        | Display commit history            |
| `branch`   | `src/commands/branch.cpp`   | ✅ Complete        | Branch creation and listing       |
| `checkout` | `src/commands/checkout.cpp` | ✅ Complete        | Switch branches                   |
| `merge`    | `src/commands/merge.cpp`    | ✅ Complete        | Merge branches                    |
| `diff`     | `src/utils/diff.cpp`        | ✅ Complete        | Show Differences Between Files    |

## 🚀 Build & Usage

### Prerequisites

- C++17 compatible compiler (g++)
- OpenSSL development libraries
- Make build system

### Compilation

```bash
make clean
make clean-data
make 
```

### Basic Usage

```bash
# Initialize repository
./minigit init

# Stage files
./minigit add <filename>

# Commit changes
./minigit commit -m "Your commit message"

# View commit history
./minigit log

# List branches
./minigit branch

# Create new branch
./minigit branch <branch-name>

# Switch branches
./minigit checkout <branch-name>

# View file changes before committing
./minigit diff <file-name>

# Merge  branch into main
./minigit merge <branch-name>
```

### Running Tests

```bash
make test
```

## 📊 Implementation Status

### ✅ Fully Implemented (Core Features)

1. **Repository Initialization** - Complete with proper directory structure
2. **File Staging** - SHA-1 hashing and blob storage
3. **Commit System** - Full metadata and parent tracking
4. **Commit History** - Traversal and display functionality
5. **Branch Management** - Creation and listing
6. **Branch Switching** - HEAD pointer management
7. **Object Storage** - Content-addressable blob system
8. **Merge Functionality** - Three-way merge, conflict detection, and LCA finding
9. **Diff Viewer** - Line-by-line difference display (Optional Feature)

### ❌ Pending Implementation (1/8 Core Features)


## 🛠️ Technical Implementation Details

### Hashing Strategy

- **Algorithm**: SHA-1 (160-bit)
- **Purpose**: Content deduplication and integrity verification
- **Implementation**: OpenSSL-based for cryptographic security

### Data Persistence

- **Format**: JSON for structured data (commits, staging)
- **Binary Storage**: Raw blob content for efficiency
- **File System**: Hierarchical organization mimicking Git

### Memory Management

- **RAII Principles**: Automatic resource management
- **Standard Containers**: std::map, std::vector for data structures
- **File Streams**: Efficient I/O operations

## 🎯 Key Learning Outcomes Achieved

1. **Data Structure Design**: Implemented DAG structure for commit history
2. **System Architecture**: Modular design with clear separation of concerns
3. **File System Operations**: Comprehensive file I/O and directory management
4. **Cryptographic Applications**: Practical use of hashing for integrity
5. **Command-Line Interface**: Robust argument parsing and error handling
6. **Version Control Concepts**: Deep understanding of Git internals

## 🚧 Future Enhancements

###  Enhanced Functionality

- [ ] Add commit amendment functionality
- [ ] Create comprehensive error handling

###  Advanced Features

- [ ] Implement tag system
- [ ] Add stash functionality
- [ ] Create interactive rebase
- [ ] Implement cherry-pick operations

## 📈 Performance Characteristics

- **Time Complexity**: O(1) for most operations due to hash-based lookups
- **Space Complexity**: Efficient through content deduplication
- **Scalability**: Suitable for small to medium-sized projects
- **Memory Usage**: Minimal due to on-disk storage strategy

## 🏆 Project Achievements

This implementation successfully demonstrates:

- **Complete Version Control Workflow**: From init to branching
- **Production-Quality Code**: Modular, well-organized C++ codebase
- **Git-Compatible Concepts**: Similar internal structure to real Git
- **Educational Value**: Clear demonstration of VCS principles
- **Practical Application**: Fully functional CLI tool

---


**Codebase Quality**: Production-ready with comprehensive error handling  
**Documentation**: Comprehensive with clear usage examples
