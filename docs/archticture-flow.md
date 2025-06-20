# MiniGit Architecture Flow Documentation

## Table of Contents

1. [System Architecture](#system-architecture)
2. [Data Structures](#data-structures)
3. [Core Components](#core-components)
4. [Command Flow Diagrams](#command-flow-diagrams)
5. [Storage System](#storage-system)
6. [Hash System](#hash-system)
7. [Implementation Details](#implementation-details)

## System Architecture

MiniGit follows a layered architecture with clear separation of concerns:

```
┌─────────────────────────────────────┐
│           CLI Interface             │  main.cpp
├─────────────────────────────────────┤
│         Command Layer               │  src/commands/*
├─────────────────────────────────────┤
│          Core Layer                 │  src/core/*
├─────────────────────────────────────┤
│         Utility Layer               │  src/utils/*
├─────────────────────────────────────┤
│       File System Layer            │  .minigit/*
└─────────────────────────────────────┘
```

### Architecture Principles

**Command Pattern**: Each Git command is implemented as a separate function
**Repository Pattern**: Central Repository class manages all operations
**Content Addressing**: Objects stored by their SHA-1 hash
**Immutable Storage**: Objects never change once created

## Data Structures

### Commit Object

```cpp
struct Commit {
    std::string hash;                           // SHA-1 of entire commit
    std::string parent;                         // Parent commit hash
    std::string message;                        // User message
    std::string timestamp;                      // ISO 8601 timestamp
    std::map<std::string, std::string> files;  // filename → blob_hash
};
```

### Repository Structure

```cpp
class Repository {
    std::string mgitDir;      // ".minigit"
    std::string commitsDir;   // ".minigit/commits"
    std::string headFile;     // ".minigit/HEAD"
};
```

### Staging Area (JSON)

```json
{
  "filename1.txt": "blob_hash_1",
  "filename2.txt": "blob_hash_2"
}
```

### Branch Reference (Text File)

```
abc123def456...  // commit hash
```

## Core Components

### Repository Manager (`src/core/repository.cpp`)

- **Purpose**: Central coordinator for all repository operations
- **Key Methods**:
  - `init()`: Creates repository structure
  - `add()`: Stages files
  - `commit()`: Creates commits
  - `isInitialized()`: Checks repository state

### Commit Manager (`src/core/commit.cpp`)

- **Purpose**: Handles commit object serialization/deserialization
- **Format**: JSON with hash, parent, message, timestamp, files
- **Chain**: Forms linked list through parent pointers

### Blob Manager (`src/core/blob.cpp`)

- **Purpose**: Content-addressable file storage
- **Deduplication**: Same content = same hash = single storage
- **Location**: `.minigit/blobs/{hash}`

### Branch Manager (`src/core/branch.cpp`)

- **Purpose**: Branch creation and management
- **Storage**: Simple text files in `.minigit/branches/`
- **Content**: Latest commit hash for each branch

## Command Flow Diagrams

### `./minigit init`

```
┌─────────────┐    ┌──────────────────┐    ┌─────────────────┐
│ User Input  │ -> │ Create Dirs      │ -> │ Initialize      │
│ init        │    │ .minigit/        │    │ HEAD & branches │
└─────────────┘    │ branches/        │    └─────────────────┘
                   │ commits/         │
                   │ blobs/           │
                   └──────────────────┘
```

### `./minigit add file.txt`

```
┌─────────────┐    ┌──────────────┐    ┌─────────────┐    ┌──────────────┐
│ User Input  │ -> │ Read File    │ -> │ Create Blob │ -> │ Update       │
│ add file    │    │ Content      │    │ SHA-1 Hash  │    │ Staging Area │
└─────────────┘    └──────────────┘    └─────────────┘    └──────────────┘
```

### `./minigit commit -m "msg"`

```
┌─────────────┐    ┌──────────────┐    ┌─────────────┐    ┌──────────────┐
│ User Input  │ -> │ Read Staging │ -> │ Create      │ -> │ Update HEAD  │
│ commit      │    │ Area         │    │ Commit Obj  │    │ & Branch     │
└─────────────┘    └──────────────┘    └─────────────┘    └──────────────┘
```

### `./minigit checkout branch`

```
┌─────────────┐    ┌──────────────┐    ┌─────────────┐
│ User Input  │ -> │ Read Branch  │ -> │ Update HEAD │
│ checkout    │    │ Commit Hash  │    │ Pointer     │
└─────────────┘    └──────────────┘    └─────────────┘
```

## Storage System

### Directory Layout

```
.minigit/
├── HEAD                        # Current commit hash
├── staging_area.json          # Staged files mapping
├── branches/                  # Branch references
│   ├── main                   # "abc123..."
│   └── feature               # "def456..."
├── commits/                   # Commit objects
│   ├── abc123...json         # Commit data
│   └── def456...json         # Commit data
└── blobs/                     # File contents
    ├── hash1...              # Raw file content
    └── hash2...              # Raw file content
```

### File Formats

**Commit File** (`.minigit/commits/{hash}.json`):

```json
{
  "hash": "abc123def456...",
  "parent": "def456ghi789...",
  "message": "Add new feature",
  "timestamp": "2024-01-15T10:30:00Z",
  "files": {
    "src/main.cpp": "blob_hash_1",
    "README.md": "blob_hash_2"
  }
}
```

**Branch File** (`.minigit/branches/main`):

```
abc123def456ghi789...
```

**HEAD File** (`.minigit/HEAD`):

```
abc123def456ghi789...
```

### Storage Characteristics

- **Permanent**: Objects never change once created
- **Content-Addressed**: Files stored by SHA-1 hash
- **No duplication**: Identical content stored only once
- **Human-Readable**: Metadata stored as JSON

## Hash System

### SHA-1 Implementation

- **Library**: OpenSSL (`#include <openssl/sha.h>`)
- **Input**: Raw file content or serialized commit object
- **Output**: 40-character hexadecimal string

### Hash Usage

**Blob Hashing**:

```cpp
std::string content = readFile("file.txt");
std::string hash = calculateSHA1(content);
// Stores in: .minigit/blobs/{hash}
```

**Commit Hashing**:

```cpp
std::string commitJson = commit.serialize();
std::string hash = calculateSHA1(commitJson);
commit.hash = hash;
// Stores in: .minigit/commits/{hash}.json
```

### Hash Properties

- **Deterministic**: Same content always produces same hash
- **Unique**: Different content produces different hashes
- **Integrity**: Detects corruption automatically
- **Reference**: Used as unique identifier for all objects

## Implementation Details

### Technology Stack

- **Language**: C++17
- **JSON Library**: nlohmann/json
- **Hashing**: OpenSSL SHA-1
- **Filesystem**: std::filesystem
- **Build System**: Make

### Key Implementation Choices

**HEAD Storage**: Stores commit hash directly (unlike Git which stores branch reference)

```cpp
// MiniGit approach
std::ofstream(".minigit/HEAD") << commit.hash;

// Git approach (not implemented)
std::ofstream(".minigit/HEAD") << "ref: refs/heads/main";
```

**Simplified Checkout**: Updates pointers only, doesn't modify working directory

```cpp
void handleCheckoutCommand() {
    // Read target commit hash
    string targetCommit = bm.getBranchHead(branchName);
    // Update HEAD only
    ofstream headFile(repoPath + "HEAD");
    headFile << targetCommit;
    // Note: Working directory unchanged
}
```

**JSON Metadata**: All metadata stored in human-readable JSON format

```cpp
std::string Commit::serialize() const {
    json j;
    j["hash"] = hash;
    j["parent"] = parent;
    j["message"] = message;
    j["timestamp"] = timestamp;
    j["files"] = files;
    return j.dump(4);  // Pretty-printed JSON
}
```

### Error Handling

- **File Operations**: Exception-based error handling
- **Command Returns**: Integer return codes (0 = success, 1 = error)
- **Validation**: Input validation before operations
- **Graceful Degradation**: Handles missing files/directories

### Performance Considerations

- **Content Deduplication**: Identical files stored only once
- **Minimal I/O**: Only reads necessary files
- **Memory Efficiency**: Streams large files instead of loading entirely
- **Fast Hashing**: Optimized SHA-1 implementation from OpenSSL
