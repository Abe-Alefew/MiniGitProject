# MiniGit Quick Reference

## Commands at a Glance

| Command        | Description                                 | Usage                                  |
|----------------|---------------------------------------------|----------------------------------------|
| `init`         | Start a new repository                      | `./minigit init`                       |
| `add`          | Stage a file for commit                     | `./minigit add <file>`                 |
| `commit`       | Save staged changes                         | `./minigit commit -m "message"`        |
| `log`          | Show commit history                         | `./minigit log`                        |
| `branch`       | Create or list branches                     | `./minigit branch [branch-name]`       |
| `checkout`     | Switch to another branch                    | `./minigit checkout <branch-name>`     |
| `merge`        | Merge target branch into current branch     | `./minigit merge <branch-name>`        |
| `diff`         | Show changes since last commit (single file)| `./minigit diff <file>`                |
| `diff-files`   | Compare any two files                       | `./minigit diff-files <file1> <file2>` |
## Getting Started (3 steps)

```bash
# Before writing the commands of minigit , make sure to write make clean, make clean-data and make for purpose of initliazing minigit objects
make clean
make clean-data
make
# 1. Create a repository
./minigit init

# 2. Add and commit your first file
./minigit add myfile.txt
./minigit commit -m "My first commit"

# 3. Check what happened
./minigit log
```

## Working with Branches

```bash
# Create a new branch
./minigit branch feature-branch

# Switch to it
./minigit checkout feature-branch

# Make changes, then commit
./minigit add changed-file.txt
./minigit commit -m "Added new feature"

# Go back to main
./minigit checkout main
```
## Merging
# Switch to the new branch
./minigit checkout feature-branch

# Make changes, then commit
./minigit add updated.txt
./minigit commit -m "Updated file on feature branch"

# View file changes before committing
./minigit diff updated.txt

# Switch back to main branch
./minigit checkout main

# Merge the feature branch into main
./minigit merge feature-branch

## What MiniGit Creates

When you run `./minigit init`, you get:

```
.minigit/
├── HEAD              # Current branch pointer
├── branches/         # Branch references
├── commits/          # Your commit history
├── blobs/            # File contents
└── staging_area.json # Files ready to commit
```

## Suggestions/ Recommendation

- Always add files before committing: `add` → `commit`
- Use meaningful commit messages
- Check `./minigit log` to see your progress
- Each file gets a unique ID based on its content
- Use `./minigit diff` before commiting to verify your changes
