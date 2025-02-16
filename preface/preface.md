---
title: Preface
author: eranbu
date: 2/2025
marp: true
theme: gaia
---

# **Welcome to Beyond C++ Course**
Let's go beyond what you already know.  

![bg left](images/cpp.webp)


<!---

-->

---


# **👨‍🏫 About Me**  
🔹 Some years in the organization.  
🔹 **Hobbies**: Writing code, Teaching, Debugging, Learning.  
🔹 **Experience**: Soft real time embedded systems.  

---

# **📌 About This Course**  

🔹 **We’ll Cover**:


1. **🗂️ Version Control with Git and GitLab**  
2. **🔧 C++ Build System and Toolchain**  
3. **⚙️ The C++ Preprocessor**  
4. **🖥️ The C++ Compiler**  
5. **🔗 The Linker**  
6. **📦 Application Binary Interface (ABI)**  
7. **🐞 Debugging C++ Applications**  

---

# **📌 More Advanced Topics**  

8. **💾 Memory Management**  
9. **🧪 Software Testing**  
10. **🧵 Multithreading & Concurrency**  
11. **🔢 Numerical Computation**  
12. **🚦 Error Handling in Modern C++**  
13. **🐍 Integrating C++ with Python**  
14. **🦾 Advanced C++ Features**  
15. **📚 Working with Third-Party C++ Libraries**  


---

# **🛠️ Tools**  

💡 **Each Lecture Will Introduce**:

✔ **A New Tool**  
✔ **מדור פשעי תוכנה** 

---

# **🤖 ChatGPT Helped Too!**  

🎨 **Icons & Inspiration**
💡 **AI-Generated Ideas**  
📝 **Reviewed, Not Auto-Generated!**  

*No, ChatGPT won’t replace your C++ skills… yet!* 😆  

---

# **📂 Where to Find Materials?**  

📌 **GitLab Repository**:  
🔗 `https://gitlab.com/beyondcpp`  

✔ **Slides & Code Examples**  
✔ **Merge Requests & Issues Welcome!**  

---

# **📢 Recordings**  

🎥 **You CANNOT count on recordings!**  
📜 **Slides are NOT a full replacement!**  

🧠 **Be present, ask questions, participate!**  

---

# **⏰ Class Schedule**  

📅 **Start Time**: **08:30**  
📅 **End Time**: **~09:45** (May be shorter or longer)  

---


### ❓ What will this code print?
```cpp
void foo(int i = 5) {std::cout << i;}
int main() {
    void (*fptr)();
    fptr = foo;
    fptr();
}
```
- 🅰 Compilation error  
- 🅱 Undefined behavior  
- 🅲 Prints `5`  
- 🅳 Segmentation fault  

<!-- 
#include <iostream>

✅ Correct Answer: 🅰 (Default arguments are bound at compile-time, but `fptr` has no parameter information.) -->

---

### ❓ What is the output of this program?
```cpp
std::optional<int> get_value(bool flag) {
    if (flag) return 42;
    return std::nullopt;
}
int main() {
    std::cout << get_value(false).value_or(100);
}
```
- 🅰 Compilation error  
- 🅱 Undefined behavior  
- 🅲 `42`  
- 🅳 `100`  

<!-- 
#include <iostream>
#include <optional>
✅ Correct Answer: 🅳 (`value_or(100)` provides a fallback when `std::nullopt` is returned.)
 -->

---

### ❓ What is the output of this code?
```cpp
consteval int square(int x) {
    return x * x;
}
int main() {
    constexpr int a = square(4);
    int b = square(5);
    std::print("{} {}\n", a, b);
}
```
- 🅰 Compilation error  
- 🅱 Undefined behavior  
- 🅲 `16 25`  
- 🅳 Only prints 16, then crashes

<!-- ✅ Correct Answer: 🅲  -->

---

### ❓ What does this code return?
```matlab
x = [1 2 3; 4 5 6; 7 8 9];
y = x(2:end, 1:2);
```
- 🅰 `[1 2; 4 5; 7 8]`  
- 🅱 `[4 5; 7 8]`  
- 🅲 `[4 5; 7 8; 10 11]`  
- 🅳 `[5 6; 8 9]`  

<!-- ✅ Correct Answer: 🅱 (Rows `2:end` and columns `1:2` extract `[[4 5]; [7 8]]`.) -->

---

### ❓ What does this snippet print?
```python
x = [1, 2, 3]
y = x
y += [4, 5]
print(x)
```
- 🅰 `[1, 2, 3]`  
- 🅱 `[1, 2, 3, 4, 5]`  
- 🅲 `TypeError`  
- 🅳 `[1, 2, 3, 4]`  

<!-- ✅ Correct Answer: 🅱 (`y +=` modifies `x` **in-place** instead of creating a new list.) -->

---

### ❓ What happens in this code?
```python
def check(value):
    match value:
        case 1 | 2:
            return "One or Two"
        case _:
            return "Something else"

print(check(2))
```
- 🅰 `"One or Two"`  
- 🅱 `"Something else"`  
- 🅲 `SyntaxError`  
- 🅳 `TypeError`  

<!-- ✅ Correct Answer: 🅰 (Pattern matching in Python 3.10+ supports `|` for multiple cases.) -->

---

### ❓ What does `git checkout HEAD -- file.txt` do?
- 🅰 Switches to the previous branch  
- 🅱 Resets `file.txt` to the latest commit version  
- 🅲 Removes `file.txt` from staging  
- 🅳 Deletes `file.txt`  

<!-- ✅ Correct Answer: 🅱 (Restores `file.txt` from the last committed version without affecting staging.) -->
