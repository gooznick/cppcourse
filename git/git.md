---
title: git
author: eranbu
date: 6/2024
marp: true
theme: gaia
no+backgroundColor: lightblue

---


# git

Some advanced usage tips

---

# Basics

``` bash
git init
echo hello > hello.txt
git add hello.txt
git commit -m "Adding new file"
```

---

# Basic actions 


<img src="images/git_repo.png" width="1000"  style="display: flex;"/>

* `git reset HEAD myfile.cpp`  # Unstage, keep changes
--- 

# Stash


<img src="images/stash.svg" alt="Local Image" width="500" height="400" />

---

# Branch


<img src="images/branches.png" alt="Local Image" width="500" height="400" />


---

# Merge and Rebase


<img src="images/merge_rebase.png" width="1000"  style="display: flex;"/>


---

# Flow


<img src="images/flow.png" width="1000"  style="display: flex;"/>


---

# Tips

* Git 
  * Small commits
  * Compilation = commit
  * Precommit hooks
* Gitlab 
  * Protected branches
  * CI + Automatic deploy
  * Flow


---

# precommit

<img src="images/precommit.png" width="1000"  style="display: flex;"/>

* refer `.pre-commit-config.yaml`

---

| Hook | Purpose |
|------|---------|
| **Clang-Format** | Ensures consistent C++ code style |
| **Clang-Tidy** | Detects potential bugs in C++ |
| **CMake-Format** | Enforces proper CMake syntax |
| **Prettier** | Formats YAML, Markdown, JSON |
| **End-of-File Fixer** | Ensures all files end with a newline |
| **Trailing Whitespace Fix** | Removes unnecessary spaces |
| **Detect Large Files** | Prevents committing huge files |
| **ShellCheck** | Checks shell scripts for errors |


---

# Danger !

* History change (Confidential, large files)
* Force push
* Adding commit after wrong merge
* Reset --hard
* Git clean
* Delete a branch

---

# Submodules

* Clone address type (ssh/https)
* Access rights
* Target version

![Image](images/submodules.png)


---

# Large files

* Types :
  * Data files
  * 3rd party binaries
  * Precompiled files (other library)
* Solutions :
  * Lfs
  * Artifactory
  * Shared directory
  * commit

---

# etc.

* New project – add all, then force push
* Gitlab packages
* Cherry-pick
* Format at precommit hooks
* Git worktree
* Partial clone
* bisect
