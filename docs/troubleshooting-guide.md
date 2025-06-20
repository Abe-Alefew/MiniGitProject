# MiniGit Troubleshooting Guide

## Table of Contents

1. [Installation & Build Issues](#installation--build-issues)
2. [Runtime Problems](#runtime-problems)
3. [Repository Corruption](#repository-corruption)
4. [Performance Issues](#performance-issues)
5. [Common Mistakes](#common-mistakes)
6. [Emergency Recovery](#emergency-recovery)
7. [Getting Help](#getting-help)

## Installation & Build Issues

### "OpenSSL not found" Error

**Error:** `fatal error: openssl/sha.h: No such file or directory`

**Fix:**

```bash
# macOS
brew install openssl

# Ubuntu/Debian
sudo apt install libssl-dev

# CentOS/RHEL
sudo yum install openssl-devel
```

### Compiler Too Old

**Error:** `'filesystem' is not a member of 'std'`

**Fix:** You need C++17 support (GCC 7+ or Clang 5+)

```bash
# Check version
g++ --version

# Update if needed (Ubuntu)
sudo apt install gcc-9 g++-9
```

### Build Fails

**Quick fixes:**

```bash
# Clean and rebuild
make clean && make all

# Fix permissions
chmod +x minigit

# Check available space
df -h
```

## Runtime Problems

### "Repository not initialized"

**Fix:** Run `./minigit init` in your project directory.

### "File not found"

**Causes & Fixes:**

- File doesn't exist: `ls -la filename.txt`
- Wrong directory: `pwd` to check location
- Wrong path: Use `./minigit add ./path/to/file.txt`

### "Nothing to commit"

**Fix:** Add files first:

```bash
./minigit add yourfile.txt
./minigit commit -m "Your message"
```

### Staging area broken

**Error:** JSON parsing errors

**Fix:** Reset the staging area:

```bash
echo '{}' > .minigit/staging_area.json
```

## Repository Corruption

### Repository seems broken

**Symptoms:** Crashes, missing data, invalid commits

**Fix:** Backup and restart:

```bash
# Backup first
cp -r .minigit .minigit.backup

# Check what's there
ls -la .minigit/
ls -la .minigit/commits/

# If badly corrupted, restart
make clean-data
./minigit init
```

### HEAD pointer issues

**Fix:** Reset HEAD:

```bash
echo "main" > .minigit/HEAD
echo "" > .minigit/branches/main
```

### Branch problems

**Error:** "Branch does not exist"

**Fix:**

```bash
# List branches
./minigit branch

# Check branch files
ls -la .minigit/branches/

# Create missing branch
./minigit branch branch-name
```

## Performance Issues

### Commands are slow

**Check these:**

```bash
# Repository size
du -sh .minigit/

# Available space
df -h

# Large files in blobs
ls -lah .minigit/blobs/
```

**Solutions:**

- Remove large unnecessary files
- Start fresh if repository is corrupted
- Check available disk space

## Common Mistakes

### Wrong command order

Wrong: `./minigit commit -m "message"` (nothing staged)  
Right: `./minigit add file.txt` then `./minigit commit -m "message"`

### Forgetting to stage

Wrong: Edit file → commit directly  
Right: Edit file → add → commit

### Bad commit messages

Wrong:  `./minigit commit -m "stuff"`  
Right:  `./minigit commit -m "Add user authentication"`

## Emergency Recovery

### Start completely over

```bash
# Backup your files first!
cp important-file.txt ../backup/

# Remove and reinitialize
rm -rf .minigit
./minigit init
./minigit add important-file.txt
./minigit commit -m "Recover important file"
```

### Recover from backup

If you made a backup earlier:

```bash
rm -rf .minigit
cp -r .minigit.backup .minigit
```

## Getting Help

### Check repository state

```bash
# Basic info
ls -la .minigit/
cat .minigit/HEAD
cat .minigit/staging_area.json

# Branch info
ls -la .minigit/branches/
cat .minigit/branches/main

# Recent commits
ls -la .minigit/commits/
```

### Debug output

Add debug prints to understand what's happening:

```bash
# Before operations
echo "Current directory: $(pwd)"
echo "Repository exists: $(ls -d .minigit 2>/dev/null && echo yes || echo no)"

# After operations
echo "Staging area:"
cat .minigit/staging_area.json
```

### When all else fails

1. Back up your actual project files
2. Delete `.minigit` folder
3. Run `./minigit init`
4. Start fresh

Remember: MiniGit only manages the `.minigit` folder - your actual project files are safe!
