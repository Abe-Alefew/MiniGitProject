# MiniGit API Reference

## Table of Contents

1. [Core Classes](#core-classes)
2. [Branch Operations](#branch-operations)
3. [Utility Functions](#utility-functions)
4. [Usage Examples](#usage-examples)
5. [Error Handling](#error-handling)
6. [JSON Format](#json-format)
7. [Public Interface](#public-interface)

## Core Classes

### Repository Class

Main interface for all repository operations.

**Header**: `include/core/repository.hpp`

```cpp
namespace mgit {
    class Repository {
    public:
        Repository();

        // Instance methods
        bool isInitialized();
        Commit getHeadCommit();
        Commit getCommitById(const std::string& hash);

        // Static commands (use these for CLI functionality)
        static void init();
        static void add(const std::string& file);
        static int commit(const std::string& message);
        static int logCommits();
    };
}
```

#### Key Methods

**`isInitialized()`** - Returns true if `.minigit/` exists and is valid.

**`getHeadCommit()`** - Gets the current commit. Returns empty commit if none exists.

**`init()`** - Creates a new repository in current directory.

**`add(file)`** - Stages a file for commit. Creates blob and updates staging area.

**`commit(message)`** - Creates commit with staged files. Returns 0 on success.

**`logCommits()`** - Prints commit history to stdout.

### Commit Structure

```cpp
namespace mgit {
    struct Commit {
        std::string hash;        // Unique commit ID
        std::string parent;      // Previous commit hash (empty for first)
        std::string message;     // User's commit message
        std::string timestamp;   // ISO 8601 timestamp
        std::map<std::string, std::string> files; // filename → blob_hash

        std::string serialize() const;
        static Commit deserialize(const std::string& content);
    };
}
```

### Blob Class

```cpp
namespace mgit {
    class Blob {
    public:
        static std::string createBlob(const std::string& filePath);
        static std::string hashContent(const std::string& content);
    };
}
```

**`createBlob(filePath)`** - Reads file, hashes content, stores blob. Returns hash.

**`hashContent(content)`** - Calculates SHA-1 hash of string content.

## Branch Operations

```cpp
namespace mgit {
    class Branch {
    public:
        static void createBranch(const std::string& name);
        static void listBranches();
        static bool switchBranch(const std::string& name);
        static std::string getCurrentBranch();
    };
}
```

## Utility Functions

### File I/O

```cpp
namespace mgit {
    // Read entire file into string
    std::string readFile(const std::string& path);

    // Write string to file
    void writeFile(const std::string& path, const std::string& content);

    // Check if file/directory exists
    bool exists(const std::string& path);

    // Create directory (and parents)
    void createDirectory(const std::string& path);
}
```

### Hashing

```cpp
namespace mgit {
    // Calculate SHA-1 hash of string
    std::string calculateSHA1(const std::string& input);

    // Calculate SHA-1 hash of file
    std::string calculateFileSHA1(const std::string& filepath);
}
```

## Usage Examples

### Basic Repository Operations

```cpp
#include "core/repository.hpp"

// Check if repo exists
mgit::Repository repo;
if (!repo.isInitialized()) {
    mgit::Repository::init();
}

// Add file and commit
mgit::Repository::add("myfile.txt");
mgit::Repository::commit("Add myfile.txt");

// Show history
mgit::Repository::logCommits();
```

### Working with Commits

```cpp
// Get current commit
mgit::Repository repo;
mgit::Commit head = repo.getHeadCommit();

if (!head.hash.empty()) {
    std::cout << "Current commit: " << head.message << std::endl;
    std::cout << "Files in this commit: " << head.files.size() << std::endl;
}

// Load specific commit
mgit::Commit older = repo.getCommitById("abc123...");
```

### Branch Management

```cpp
// Create and switch branches
mgit::Branch::createBranch("feature-branch");
mgit::Branch::switchBranch("feature-branch");

// See current branch
std::string current = mgit::Branch::getCurrentBranch();
std::cout << "On branch: " << current << std::endl;
```

### Custom File Operations

```cpp
#include "utils/file_io.hpp"
#include "utils/hashing.hpp"

// Read and hash a file
std::string content = mgit::readFile("data.txt");
std::string hash = mgit::calculateSHA1(content);

// Create blob manually
std::string blobHash = mgit::Blob::createBlob("myfile.txt");
```

## Error Handling

Most functions throw exceptions or return error codes:

- `Repository::commit()` returns 0 on success, 1 on error
- File operations may throw `std::runtime_error`
- Invalid paths or missing files will cause exceptions

Always check return values and wrap calls in try-catch blocks for production code.

## JSON Format

### Commit Object

```json
{
  "hash": "a1b2c3d4...",
  "parent": "e5f6g7h8...",
  "message": "Your commit message",
  "timestamp": "2024-01-15T10:30:00Z",
  "files": {
    "file1.txt": "blob_hash_1",
    "file2.txt": "blob_hash_2"
  }
}
```

### Staging Area

```json
{
  "file1.txt": "blob_hash_1",
  "file2.txt": "blob_hash_2"
}
```

## Public Interface

### Command Functions

These functions provide the main command-line interface functionality:

#### `initCommand()`

```cpp
int initCommand()
```

**File**: `src/commands/init.cpp`  
**Returns**: 0 on success  
**Description**: Wrapper for repository initialization

**Implementation**:

```cpp
int initCommand() {
    mgit::Repository::init();
    return 0;
}
```

#### `addCommand()`

```cpp
int addCommand(const std::string& filename)
```

**File**: `src/commands/add.cpp`  
**Parameters**:

- `filename`: File to add to staging area

**Returns**: 0 on success, 1 on error  
**Description**: Wrapper for file staging

**Example Usage**:

```cpp
int result = addCommand("myfile.txt");
if (result != 0) {
    std::cerr << "Failed to add file" << std::endl;
}
```

#### `commitCommand()`

```cpp
int commitCommand(const std::string& message)
```

**File**: `src/commands/commit.cpp`  
**Parameters**:

- `message`: Commit message

**Returns**: 0 on success, 1 on error  
**Description**: Wrapper for commit creation

**Example Usage**:

```cpp
int result = commitCommand("Add new feature");
```

#### `logCommits()`

```cpp
int logCommits()
```

**File**: `src/commands/log.cpp`  
**Returns**: 0 on success  
**Description**: Displays commit history

#### Branch Management Functions

##### `handleBranchCommand()`

```cpp
void handleBranchCommand(const std::string& repoPath, const std::vector<std::string>& args)
```

**File**: `src/commands/branch.cpp`  
**Parameters**:

- `repoPath`: Path to repository (typically ".minigit/")
- `args`: Command arguments vector

**Description**: Handles both branch listing and creation

- If `args.size() == 1`: List all branches
- If `args.size() == 2`: Create new branch with `args[1]` as name

**Usage Patterns**:

```cpp
std::vector<std::string> listArgs = {"branch"};
handleBranchCommand(".minigit/", listArgs);  // List branches

std::vector<std::string> createArgs = {"branch", "feature-new"};
handleBranchCommand(".minigit/", createArgs);  // Create branch
```

##### `handleCheckoutCommand()`

```cpp
void handleCheckoutCommand(const std::string& repoPath, const std::vector<std::string>& args)
```

**File**: `src/commands/checkout.cpp`  
**Parameters**:

- `repoPath`: Path to repository
- `args`: Command arguments vector

**Description**: Switches to specified branch

- Validates branch exists
- Updates HEAD to point to branch

**Usage Pattern**:

```cpp
std::vector<std::string> checkoutArgs = {"checkout", "main"};
handleCheckoutCommand(".minigit/", checkoutArgs);
```
