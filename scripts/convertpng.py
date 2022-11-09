from PIL import Image
import pathlib


def convert_color(color):
    return ((color[0] & 0xF8) << 8) | ((color[1] & 0xFC) << 3) | (color[2] >> 3)


def convert_png(path):
    colors = []
    name = pathlib.Path(path).stem
    with Image.open(path) as img:
        for y in range(img.height):
            for x in range(img.width):
                colors.append(convert_color(img.getpixel((x, y))))
        return (name, colors, img.width, img.height)


def print_converted_png(data):
    name, colors, w, h = data
    name = "IMG_" + name.upper()
    string = ""
    string += "static const uint8_t " + name + "_W = {};\n".format(w)
    string += "static const uint8_t " + name + "_H = {};\n".format(h)
    string += "static const uint16_t " + name + "[] PROGMEM = {\n"
    string += "    "
    line_w = 0
    for c in colors:
        string += "0x{:04X}, ".format(c)
        line_w += 1
        if line_w >= 15:
            string = string[:-1]
            string += "\n    "
            line_w = 0
    if string.endswith("\n"):
        string += "};\n"
    else:
        string = string[:-1]
        string += "\n};\n"
    return string


def convert_pngs(paths, output_path):
    with open(output_path, "w+") as ofile:
        ofile.write("#pragma once\n")
        ofile.write("#include <Arduino.h>\n")
        ofile.write("\n")
        for path in paths:
            converted_data = convert_png(path)
            string = print_converted_png(converted_data)
            ofile.write(string)
            ofile.write("\n")



if __name__ == "__main__":
    weathers = []
    for f in pathlib.Path("img/weather").iterdir():
        weathers.append(f)
    convert_pngs(weathers, "lib/window/WeatherIcon.h")
    convert_pngs(["img/house_temp.png"], "lib/window/HeaterIcon.h")