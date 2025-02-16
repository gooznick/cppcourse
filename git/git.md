---
title: Git Mastery 🚀
author: Eranbu
date: 6/2024
marp: true
theme: gaia
nobackgroundColor: lightblue
---

# 🛠️ Git – Advanced Usage

Some **tips** to level up your Git skills!

---

# 🔥 Git Basics

```bash
git init
echo "hello" > hello.txt
git add hello.txt
git commit -m "Adding new file"
```
✅ **Initialize, stage, and commit** a file

<!---
Always work within git repo, even when it's very small project.
[When it begins to be bigger - open gitlab project]
-->

---


# **SCM - Source Control Management** 🛠️  

* **Source control** = ניהול תצורה ? 🤔  
* **Source** 📄  
  * Small 📏  
  * Comparable 🔍  

<!---
Make everything text : 
  markdown/marp
  dot/mermaid
  latex
-->
---

### **Version Control Evolution**  

📂 **SourceSafe** → 
🏛 **ClearCase** → 
🔄 **SVN** → 
🌀 **Mercurial (Hg)** → 
🐙 **Git**  


---

# 📌 Basic Actions

<img src="images/git_repo.png" width="1000" style="display: flex;" />

```bash
git reset HEAD -- myfile.cpp
```

<!---
Missing arrow - from staging back to working
Missing stash
-->

---

# 📦 Stash – Save Work in Progress


<img src="images/stash.svg" alt="Local Image" width="500" />

🔹 **Temporarily save uncommitted changes**  


<!---
May skip this
-->

---

# 🌱 Branch

<img src="images/branches.png" alt="Local Image" width="500"  />

  ```bash
  git checkout -b feature-xyz
  ```

<!---
[Many flavours of same command]
Lightweight.
Branch == Mission
-->

---

# **Branch == Single Task** 🚀

- Each branch should **focus on one task** ✅
- Keeps changes **isolated** 🔍
- Makes **code reviews easier** 👀
- Allows **parallel development** 🔄

<!---
feature, bugfix, squash
-->

---


<img src="images/git-merge.gif" width="400" />

- Merge **Frequently** ✅
- Use **common repository** 🤝

---

# 📜 Git Flow 

<img src="images/flow.png" width="1000" />

---


<img src="images/git_force.jpg" width="700" />

<!---
Protect the important branches !
main/master/release
-->

---

# **🚀 Tips**  

- **Small commits** – Easier debugging  
- **Pre-commit hooks** – Automate checks  
- **GitLab best practices**:
  - **Protected branches**
  - **CI/CD Pipelines**
  - **Efficient workflows**

---

# ✅ Pre-Commit Hooks

<img src="images/precommit.png" width="1000" style="display: flex;" />

💡 `.pre-commit-config.yaml`

---

# 🛡️ Useful Pre-Commit Hooks

| 🛠️ Hook | 🔍 Purpose |
|---------|-----------|
| **Clang-Format** | Ensures consistent C++ style |
| **Clang-Tidy** | Finds potential bugs |
| **CMake-Format** | Enforces clean CMake syntax |
| **EOF Fixer** | Adds missing newline at end of file |
| **Trailing Space Fix** | Removes unnecessary spaces |
| **Large File Detector** | Prevents committing large files |
| **ShellCheck** | Validates shell scripts |

---

# ⚠️ Git Dangers! 🚨

❌ **Avoid these risky actions unless you're sure!**  
- **Force push (`git push --force`)** 🔥  
- **Rewriting history (e.g., `git rebase -i`)** 🕰️  
- **Accidentally deleting a branch** 🗑️  
- **`git reset --hard` (Loses changes!)** 😱  
- **Committing large files (Use Git LFS)** 📂  

<!---
May skip this !
-->
---

# 🏗️ Working with Submodules

🛠️ **Managing external repositories inside your repo**  
- **Choose SSH or HTTPS**  
- **Access rights matter!**  
- **Version/Branch/tip**  

<!---
In order to share code, we have to split into small repositories.
-->

---

# 🏗️ Alternatives

| **Scenario** | **Alternative** |
|-------------|----------------------|
| Managing **multiple repositories** | ✅ **Google Repo, gitman** |
| Get **precompiled** binaries | ✅ **Manual Download** |
| Working with **third-party libraries** | ✅ **Package Manager** |
| Get other repo **into mine** | ✅ **Git Subtree** |

<!---
The other locations must be very stable !
-->


---

# 📦 Large Files

**Common large file types**:
- 📊 Data files
- 🏗 3rd-party binaries
- 🔧 Precompiled libraries  

---

# Solutions for Large Files

- ✅ Use **Git LFS**
- ✅ Store in **Artifactory**
- ✅ Keep in a **shared directory**
- ✅ Commit directly  

---

# 🎯 More Git Tips & Tricks

- **New project?** → `git add . && git commit -m "Initial commit"`
- **Cherry-pick commits** from another branch  
- **Git Worktree** – Work on multiple branches at once  
- **Partial Clone** – Speed up large repo clones  
- **`git bisect`** – Find the commit that introduced a bug  

---


# Some company's specific problems

- Availabe space in the servers
- Access rights 
  - gitlab ssh
  - Project user
- Network drive access rights (mount linux/windows)

---

# Questions? 🤔

