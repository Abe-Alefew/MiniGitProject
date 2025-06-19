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