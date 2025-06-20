# MiniGit User Guide

## Table of Contents

1. [What is MiniGit?](#what-is-minigit)
2. [Installation](#installation)
3. [Your First Repository](#your-first-repository)
4. [Understanding the Workflow](#understanding-the-workflow)
5. [Working with Branches](#working-with-branches)
6. [Common Tasks](#common-tasks)
7. [Tips for Success](#tips-for-success)
8. [Guides](#Guides)

## What is MiniGit?

MiniGit is a simple version control system that works like Git but is much lighter. It helps you track changes to your files and manage different versions of your project.

## Installation

### Prerequisites

You need a C++ compiler and OpenSSL libraries:

**macOS:**

```bash
xcode-select --install
brew install openssl
```

**Ubuntu/Debian:**

```bash
sudo apt update
sudo apt install build-essential libssl-dev
```

### Building MiniGit

```bash
cd MiniGitProject
make all
```

This creates the `minigit` executable. Test it by running `./minigit` - you should see a usage message.

## Your First Repository

### Step 1: Initialize

```bash
./minigit init
```

This creates a `.minigit` folder to store your project's history.

### Step 2: Add Files

```bash
echo "Hello World" > hello.txt
./minigit add hello.txt
```

This stages your file (prepares it for saving).

### Step 3: Commit

```bash
./minigit commit -m "Add hello file"
```

This permanently saves your changes with a message.

### Step 4: Check History

```bash
./minigit log
```

Shows all your commits.

## Understanding the Workflow

Think of MiniGit like taking snapshots of your project:

1. **Working Directory** - Where you edit files
2. **Staging Area** - Files ready to be saved (use `add`)
3. **Repository** - Permanent history of changes (use `commit`)

```
Your Files → add → Staging → commit → History
```

## Working with Branches

Branches let you work on different features separately:

```bash
# See all branches
./minigit branch

# Create a new branch
./minigit branch my-feature

# Switch to it
./minigit checkout my-feature

# Work normally: add, commit, etc.
./minigit add new-feature.txt
./minigit commit -m "Add new feature"

# Switch back to main
./minigit checkout main
```

## Common Tasks

### Multiple Files

```bash
./minigit add file1.txt
./minigit add file2.txt
./minigit commit -m "Update multiple files"
```

### Check What's Staged

Look at `.minigit/staging_area.json` to see what's ready to commit.

### Start Over

If something goes wrong:

```bash
rm -rf .minigit
./minigit init
```

## Tips for Success

- **Commit often** - Small, frequent commits are better than large ones
- **Write clear messages** - Future you will thank you
- **Use branches** - Keep experimental work separate
- **Check your work** - Use `./minigit log` to see what you've done

## Guides

- Check the [Quick Reference](quick_reference.md) for command syntax
- See [Troubleshooting Guide](troubleshooting-guide.md) if things go wrong
- Look at [API Reference](api_reference.md) for technical details
