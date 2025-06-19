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
