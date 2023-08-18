# Image to ASCII 🏞️

<img src="img2ascii.png" align="right" width="200">

[![Made with C](https://img.shields.io/badge/Made%20with-C-blue.svg)](https://opensource.org/licenses/MIT)
[![License: MIT](https://img.shields.io/badge/License-MIT-orange.svg)](https://opensource.org/licenses/MIT)
[![PRs: welcome](https://img.shields.io/badge/PRs:-welcome-limegreen.svg)](https://shields.io/)
[![GitHub stars](https://img.shields.io/github/stars/JosefVesely/Image-to-ASCII.svg?style=social&label=Star&maxAge=2592000)](https://github.com/JosefVesely/Image-to-ASCII)


A command-line tool for converting images to ASCII art

## Installation 🛠️

```sh
# Clone the repository
git clone https://github.com/JosefVesely/Image-to-ASCII

# Enter the project directory
cd Image-to-ASCII

# Compile the program using Make
make
```

## Usage 📋

```js
Usage: img2ascii --input=<FILE> [options]

A command-line tool for converting images to ASCII art

Options:
    -i, --input=<FILE>      Path of the input image file (required)
    -o, --output=<FILE>     Path of the output file (default is "output.txt")
    -w, --width=<NUMBER>    Width of the output
    -c, --chars=<STRING>    Characters to be used for the ASCII image
    -p, --print             Print the output to the console
    -r, --reverse           Reverse the string of characters
    
GitHub Repository: https://github.com/JosefVesely/Image-to-ASCII
```

## Examples 🌟

|                     Input                     |                                 Output                                 |
|-----------------------------------------------|------------------------------------------------------------------------|
| <img src="images/c.png" width="300">          | <img src="examples/ascii-c.png" width="500">                           |
|                                               | `img2ascii --input=images/c.png --width=40 --print`                    |
| <img src="images/monroe.jpg" width="300">     | <img src="examples/ascii-monroe.png" width="500">                      |
|                                               | `img2ascii --input=images/monroe.jpg --width=35 --print --reverse`     |
| <img src="images/pink_floyd.jpg" width="300"> | <img src="examples/ascii-pink_floyd.png" width="500">                  |
|                                               | `img2ascii --input=images/pink_floyd.jpg --width=80 --print --reverse` |
| <img src="images/pepsi.png" width="300">      | <img src="examples/ascii-pepsi.png" width="500">                       |
|                                               | `img2ascii --input=images/pepsi.png --width=35 --print --reverse`      |
| <img src="images/aperture.jpg" width="300">   | <img src="examples/ascii-aperture.png" width="500">                    |
|                                               | `img2ascii.exe --input=images/aperture.jpg --width=50 --print`         |

## Contributing 🤝

Contributions are welcome! If you have any improvements or bug fixes, feel free to submit a pull request

## License 📄

This project is licensed under the MIT License - see the [LICENSE](https://github.com/JosefVesely/Image-to-ASCII/blob/main/LICENSE) file for details