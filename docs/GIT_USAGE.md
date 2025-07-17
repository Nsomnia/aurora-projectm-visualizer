# Basic Git Workflow

This project uses Git for version control. Here are some basic commands to get you started.

## Checking the Status

To see which files have been changed, which are new, and which are staged for the next commit, use:

```bash
git status
```

## Adding New Files

When you create a new source file (e.g., a new `.cpp` or `.h` file), you need to tell Git to track it:

```bash
git add path/to/your/new/file.cpp
```

You can also add all new and modified files at once:

```bash
git add .
```

## Committing Changes

Once you have staged your changes with `git add`, you can commit them to the project's history. A commit is a snapshot of your work at a specific point in time.

```bash
git commit -m "A brief, descriptive message about your changes"
```

A reminder to commit will be printed after each successful build.

## Viewing History

To see the history of commits, you can use:

```bash
git log
```

