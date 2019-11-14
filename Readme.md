# About the project
Win32 OOP version of the WinAPI for creating Windows Forms. 
I developed this tool for building more lightweight UI programs for Windows with no need of any third party frameworks.

*This project is still in an early stage of development and may lack some controls or functionalities.*

### Win32GUI approach
Since the Win32 API is a very complex functional tool Win32GUI handles some aspects of it by itself like the WNDCLASS registration.
In here every Thread of the program has associated a local WNDCLASS from which derivates all of the possible windows forms created by the system.

# Examples
## Single Window example
Here we create a Windows From of 800x600 and assign a textview to it.
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
- Window 90%
- TextView 95%
- EditText 90%
- PictureBox 0%
- Button 90%
- ImageButton 0%
- CheckBox 90%
- GroupBox 0%
- ProgressBar 0%
- ListBox 0%
- TreeView 0%
- ComboBox 0%
- TrackBar 0%
- TopMenuBar 50%
- ContextMenu 70%