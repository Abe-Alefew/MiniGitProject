# MiniGit Quick Reference

## Commands at a Glance

| Command    | What it does            | Usage                           |
| ---------- | ----------------------- | ------------------------------- |
| `init`     | Start a new repository  | `./minigit init`                |
| `add`      | Stage a file for commit | `./minigit add <file>`          |
| `commit`   | Save your changes       | `./minigit commit -m "message"` |
| `log`      | See commit history      | `./minigit log`                 |
| `branch`   | Create or list branches | `./minigit branch [name]`       |
| `checkout` | Switch branches         | `./minigit checkout <branch>`   |

## Getting Started (3 steps)

```bash
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
