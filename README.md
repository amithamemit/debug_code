# Image Processing System

A multi-process image processing pipeline built with C++, LodePNG, and TCP-based inter-process communication.

## System Architecture

The system consists of four main processes communicating via a custom TCP-based message queue:

1.  **Format Parser**: Decodes PNG images into raw RGBA data.
2.  **Image Processor**: Applies filters (Grayscale, Flip Vertical/Horizontal, Invert, Brightness) to the raw data.
3.  **Output Router**: Saves the final result to a file or "displays" it on the screen.
4.  **Control**: The user-facing persistent interactive session that triggers processing tasks.

## Building the System

Ensure you have a C++ compiler (e.g., g++ via MinGW) and GNU Make installed.

1.  Open a terminal in the project root.
2.  Run the build command:
    ```bash
    make
    ```
    This creates four executables in their respective directories:
    - `format_parser/format_parser.exe`
    - `image_processor/image_processor.exe`
    - `output_router/output_router.exe`
    - `control/control.exe`

## Running the Pipeline

### Step 1: Start the Back-end Processes
Open three separate terminals and start the listeners. They will wait for a connection and remain persistent.

**Terminal 1 (Output Router):**
```powershell
./output_router/output_router.exe
```

**Terminal 2 (Image Processor):**
```powershell
./image_processor/image_processor.exe
```

**Terminal 3 (Format Parser):**
```powershell
./format_parser/format_parser.exe
```

### Step 2: Use the Interactive Control Session
In a fourth terminal, start the control process:
```powershell
./control/control.exe
```

Once inside the session, you can enter commands in the following format:
`[filename] [filters] [output_method]`

**Example:**
```text
> input.png grayscale,invert file
```

### Commands & Options
- **`help`**: Displays all available filter operations and output methods.
- **`exit`**: Closes the interactive session.
- **Filters**: `grayscale`, `flip_h`, `flip_v`, `invert`, `brightness`, `none`.
- **Output Methods**: `screen` (console metadata), `file` (saves as `[name]_modified.png`).

## Troubleshooting

- **Access Denied/Permission Errors**: Ensure you have stopped any running instances of the executables before re-running `make`.
- **Connection Issues**: The system uses ports in the 5000-6000 range. Ensure these are not blocked by a firewall and no other processes are using them.
- **Missing Input**: Place your input PNG images in the project root directory.
