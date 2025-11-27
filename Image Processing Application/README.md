# 📁 Image Processing Application

This folder contains an image processing application implemented in C++. It includes the main source file and necessary libraries for image handling.

## 🚀 Getting Started

To compile and run the image processing application:

1. Navigate to the `Image Processing Application` directory.
2. Compile the source code:
   
   ```bash
   g++ image.cpp libraries/*.h -o image_processor
   ./image_processor
   ```

---

## 🚀 Features

* 📥 Import images (PNG, JPG, JPEG)
* 🧮 Import matrices from console or file
* 🖤 Convert images to grayscale
* 🎚️ Dithering techniques:

  * Native threshold
  * Brightness-based threshold
  * Floyd–Steinberg error diffusion
  * Ordered dithering with customizable masks
  * Pattern dithering
* 🎞️ Image transition effects:

  * Cross dissolve
  * Dither dissolve
* 📤 Export result as an image or matrix

---

## 📦 Dependencies

This project uses the following libraries:

* [`stb_image.h`](https://github.com/nothings/stb) – for image loading
* [`stb_image_write.h`](https://github.com/nothings/stb) – for image saving

Make sure to place both headers in a `libraries/` directory beside the source file.

---

## 🛠️ Compilation

To compile the project:

```bash
g++ image.cpp libraries/stb_image.h libraries/stb_image_write.h -o image_app
```

Then run it:

```bash
./image_app
```

---

## 📂 Input Sources

You can provide input via:

* A standard image file (`.png`, `.jpg`, `.jpeg`)
* A matrix entered manually from the console
* A matrix read from a text file

Output can also be exported:

* As a new image file (PNG)
* As printed matrix to console
* As saved matrix in a file

---

## 📸 Sample Operations

After running the program, choose from the menu:

```
1. Convert Image to Grayscale
2. Apply Native Threshold Dithering
3. Apply Brightness-based Threshold Dithering
4. Apply Floyd-Steinberg Error Diffusion
5. Apply Ordered Dithering
6. Apply Pattern Dithering
7. Apply Cross Dissolve Transition
8. Apply Dither Dissolve Transition
9. INFO
10. EXIT
```

---

## 🧠 Author

**Ahmed Khaled**
Version `2.01`
CLI-based image editing tool built for educational and experimental use.

---

## 📄 License

This project is released for educational and non-commercial use only. Please do not use this code in plagiarism or academic misconduct.
