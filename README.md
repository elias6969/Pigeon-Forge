# 🐦 PigeonForge

**PigeonForge** is a dark-themed, GUI-powered project scaffolder for C++ developers.  
With a click, it generates fully structured starter projects for:

- 🧱 C++ Console apps  
- 🖼️ wxWidgets GUI applications  
- 🔺 OpenGL graphics projects  

Ideal for quickly setting up new projects with CMake and common directory layouts.

---

## ✨ Features

- ⚡ Instant project generation
- 📁 Automatically creates source, include, assets, and build folders
- 📜 Replaces placeholders like `{{PROJECT_NAME}}` in template files
- 📂 Full OpenGL project tree cloning with custom variables
- 🖥️ Built using **wxWidgets** for native look & feel
- 🎨 Clean dark mode with pigeon mascot branding

---

## 📦 Project Structure

Your generated project will look like this: 
MyProject/ ├── src/ # Source files ├── include/ # Header files ├── assets/ # Placeholder for assets (images, sounds, etc.) ├── build/ # Out-of-source build dir ├── .cache/ # Internal cache if needed ├── CMakeLists.txt # Auto-generated or copied from template └── main.cpp # Based on the selected project type

---

## 🧠 How it works

1. Select a project type from the dropdown.
2. Name your project.
3. Choose a directory to generate it in.
4. Hit **Generate** — the wizard copies and processes template files from:

~/Documents/Pigeon-App/assets/ ├── c++_console/ ├── wxwidgets/ └── OpenGL/

yaml
Copy
Edit

5. For C++ Console and wxWidgets:
   - Only specific template files (`main.cpp.template`, `CMakeLists.txt.template`, etc.) are copied.
6. For OpenGL:
   - The entire directory is cloned and placeholders are replaced recursively.

---

## 🧪 Running

Build with `make` or CMake, then run:

```bash
./ProjectMaker
```
Or install system-wide with a .desktop entry for Rofi/dmenu/i3 launching.

📍 Default Template Location
Put your project templates here:

```bash
~/Documents/Pigeon-App/assets/
```
Each folder under assets/ is a different project type. Templates use {{PROJECT_NAME}} placeholders that will be replaced during generation.

## 💻 Dependencies
C++17

wxWidgets 3.2+

Linux desktop (tested on i3 / EndeavourOS)

Rofi/dmenu (optional, for launcher integration)

## 🐦 Theming
Dark grey background

Light text

Pigeon-blue accent (#588ceb)

Mascot icon (pigeon_64.png) and app icon (pigeon.ico) loaded from runtime folder

## ⚙️ Want to Add Your Own Templates?
Create a new folder under assets/

Add:

main.cpp.template

CMakeLists.txt.template

(Optional) extra .h.template or .cpp.template files

Use {{PROJECT_NAME}} anywhere you want the user’s project name inserted.

## 🤍 Created with love and pigeons 
Please donate to https://www.pigeonrescue.org/ 
