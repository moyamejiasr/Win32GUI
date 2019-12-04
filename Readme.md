# About the project
Win32 OOP version of the WinAPI for creating Windows Forms. 
I developed this tool for building more lightweight UI programs for Windows with no need of any third party frameworks.

⚠ *This project is still in development and may lack some controls or functionalities.*

### Win32GUI approach
Since the Win32 API is a very complex functional tool Win32GUI handles some aspects of it by itself like the WNDCLASS registration.
In here every Thread of the program has associated a local WNDCLASS from which derivates all of the possible windows forms created by the system.

# Examples
## Single Window example
Here we create a WindowsForm of 800x600 and assign a textview to it.
Text controls will change size automatically if not specified previously.
```c++
#include "Win32/Manager.h"

int main() {
    // Create hidden window
    Window window("Hello world", 800, 600);
    // Create label
    TextView lbl(&window, "This is an autosize label"); 
    // Show Window
    window.show(); 

    // Join the WindProc & lock thread
    window.join();
}
```

## Double Window example
This example is very similar to the previous one but in here we join from the second window instead of the first one and create another window at 500:300.
```c++
#include "Win32/Manager.h"

int main() {
    Window window("Hello world", 800, 600);
    TextView lbl(&window, "This is an autosize label");
    window.show();

    Window window1("Another Window");
    window1.setLocation(500, 300);
    window1.show();

    // It does not matter from
    // where we join from.
    window1.join();
}
```

# Development
- Window 95%
- General Control struct 90%
- TextView 100%
- EditText 100%
- PictureBox 100%
- Button 100%
- CheckBox 100%
- RadioButton 100%
- GroupBox 100%
- ImageButton 100%
- ImageList 100%
- TabControl 100%
- ListBox 100%
- TreeView 100%
- StatusBar 0% *Ignored for now*
- ComboBox 100%
- ProgressBar 100%
- TrackBar 100%
- TopMenuBar 100%
- ContextMenu 100%
- OGLWindow 100%

# To-Do
- Add controls constructor without creation
- Add getters & change names
- Add mouse click atts enums for window call
- Add child mouse & key detections