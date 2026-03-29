# LookUpTheSkies

A star detection tool that analyzes sky/space images to automatically locate and count celestial objects. The core engine is written in C++ with OpenCV for fast image processing, wrapped in a Python GUI built with CustomTkinter.

## How It Works

1. The GUI lets you select a JPG or PNG image
2. The C++ engine divides the image into a 10×10 grid and processes each section in parallel threads
3. Each section is converted to grayscale, thresholded, and scanned for contours
4. Detected objects are circled in red and the result is saved as `output.jpg`
5. The total star count is displayed in the GUI

## Requirements

**Python:**
- Python 3.x
- [CustomTkinter](https://github.com/TomSchimansky/CustomTkinter): `pip install customtkinter`

**C++:**
- OpenCV 2/3+
- C++11-capable compiler (GCC, Clang, MSVC)

## Build

Compile `LookUpTheSkies.cpp` and link against OpenCV. Example with g++:

```bash
g++ LookUpTheSkies.cpp -o LookUpTheSkies `pkg-config --cflags --libs opencv4` -std=c++11 -pthread
```

On Windows with MSVC, add the OpenCV include and lib directories to your project and link `opencv_world*.lib`.

## Usage

```bash
python launcher.py
```

1. Click **Select file** and choose a sky image (JPG or PNG)
2. Click **Start search** — the C++ engine will process the image
3. The star count appears in the window
4. Click **Open result** to view the annotated image (`output.jpg`)

## Project Structure

```
LookUpTheSkies/
├── launcher.py          # Python GUI (CustomTkinter)
├── LookUpTheSkies.cpp   # C++ image processing engine
├── images/              # Sample sky images for testing
└── LICENSE
```

## Output Files

| File | Description |
|------|-------------|
| `output.jpg` | Annotated image with red circles around detected stars |
| `output_data.txt` | Temporary file containing the raw star count (deleted after display) |

## License

MIT — see [LICENSE](LICENSE)
