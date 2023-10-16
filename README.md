# KIID
A configurable search bar for launching applications.

## Installation
Installation should, hopefully, be very simple. Most dependencies are included as submodules with the exception of qt.

* `git clone git@github.com:mhydesf/kiid.git`
* `cd kiid`
* `git submodules update --init --recursive`
* `sudo apt install qt6-base-dev qt6-wayland`
* `mkdir build && cd build`
* `cmake ../`
* `make`

### Dependencies
* `Qt6`
* `nlohmann/json`

### Notes
* `sudo apt install qt6-base-dev qt6-wayland`
* Header files are installed to /usr/include/x86_64-linux-gnu/qt6/
* Set -fPIC compile flag

## Configuration
All configuration options have defaults - reference `config.h`.

```json
{
    "font": {
        "font_name": "Monospace"
    },
    "screen": {
        "p_horizontal": -1,
        "p_vertical": 500
    },
    "search_box": {
        "width": 800,
        "height": 80,
        "brd_width": 1,
        "brd_radius": 5,
        "font_size": 20,
        "padding": 5,
        "bg_color": {
            "r": 255,
            "g": 255,
            "b": 255,
            "opacity": 0.8
        },
        "brd_color": {
            "r": 0,
            "g": 0,
            "b": 0,
            "opacity": 0.8
        },
        "text": " Kiid Search"
    },
    "results_view": {
        "brd_width": 1,
        "brd_radius": 5,
        "font_size": 20,
        "padding": 5,
        "num_items": 5,
        "bg_color": {
            "r": 255,
            "g": 255,
            "b": 255,
            "opacity": 0.8
        },
        "brd_color": {
            "r": 0,
            "g": 0,
            "b": 0,
            "opacity": 0.8
        },
        "v_scroll_bar": false,
        "h_scroll_bar": false
    }
}
```

