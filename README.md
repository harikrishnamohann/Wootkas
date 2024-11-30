# Wootkas
Work in progress... 🛠️🛠️

This is my personal wallpaper engine.

## The way
- [ ] Add more presets

- [ ] (A fundamental change ahead) The preset should self generate. The current logic is to replace placeholders in preset file with desired values; which involves building a function for each preset. That will be a headache if I ever consider to scale the number of wallpapers and it's variable options. Instead **each preset should encapsulate logic/data for building the output within itself**. A preset syntax for doing this maybe...
	- [ ] develop a **preset file structure / syntax** for this
		- variables
		- variable types [theme, random_range(number), svg_tag, svg_tags[n], text, canvas_height, canvas_width...]
	- [ ] Parsing

- [ ] Build an interface for handling wallpaper. It should feature
	- Preset preview
	- Preset selection(basic)
	- customize the behavior of each wallpaper such as
		- An interval to update the image
		- Color range or give an accent color to use (wallpaper specific)
		- resolution options (Display | Custom)
		- An option to render image to a specified location
		- Maybe an option to add custom messages to the image
		- Theme options (wallpaper specific)

## How to build it?
* **Prerequisites:** gcc, make
1. clone the repo ```git clone https://github.com/harikrishnamohann/Wootkas.git```
2. execute ```make run```

## Directory structure
```
Wootkas
├── assets
│	│  - Home for preset files 
│   └── ideas - Contains new image preset ideas
├── include
│	- Header files for lib.
│	- Explanations of my custom functions.
├── lib
│	- All helper functions and out context functions lives here
│	- Mainly string.c and utils.c
├── src
│	- The heart directory.
└── target - compiled binary file goes here.
```

### Demo
![Demo](https://github.com/harikrishnamohann/Wootkas/blob/main/demo/demo.gif)
