from PIL import Image

# https://www.sigmdel.ca/michel/program/misc/gfxfont_8bit_en.html

CHAR_W = 4
CHAR_H = 7
FONT_FILE = "font/font4x7v2.png"
CHAR_PER_ROW = 26
NB_OF_ROW = 3

def is_black(pixel):
    return pixel[3] == 255


def get_char(img, x, y):
    x_pos = x * (CHAR_W + 1)
    y_pos = y * (CHAR_H + 1)
    out = []
    for yy in range(CHAR_H):
        out.append([])
        for xx in range(CHAR_W):
            out[yy].append(is_black(img.getpixel((x_pos + xx, y_pos + yy))))
    return out


def array_to_bytes(char):
    out = []
    current = 0
    i = 0
    for row in char:
        for c in row:
            current = (current << 1) | (1 if c else 0)
            i += 1
            if i == 8:
                out.append(current)
                current = 0
                i = 0
    if i != 0:
        for _ in range(i, 8):
            current = current << 1
        out.append(current)
    return out


def get_char_size(char):
    start_x = -1
    start_y = -1
    end_x = -1
    end_y = -1
    for y in range(0, CHAR_H):
        for x in range(0, CHAR_W):
            if char[y][x]:
                if start_x == -1 or x < start_x:
                    start_x = x
                if start_y == -1 or y < start_y:
                    start_y = y
                if x > end_x:
                    end_x = x
                if y > end_y:
                    end_y = y
    return (end_x - start_x + 1, end_y - start_y + 1)


def generate_data(font_data):
    string = "const uint8_t font7pt[] PROGMEM = {\n    "
    line_w = 0
    for b in font_data:
        string += "0x{:02X}, ".format(b)
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


def generate_glyph_string(glyphs):
    string = "const GFXglyph fontGlyphs[] PROGMEM = {\n"
    for (c, bitmap_offset, w, h, xadv, x_offset, y_offset) in glyphs:
        string += "    {" + "{}, {}, {}, {}, {}, {}".format(bitmap_offset, w, h, xadv, x_offset, y_offset) + "}," + " // '{}'\n".format(c)
    string += "};\n"
    return string


def convert_font():
    with open("font.ino", "w+") as ofile:
        ofile.write("#pragma once\n")
        ofile.write("#include <Arduino.h>\n")
        ofile.write("#include <Adafruit_GFX.h>\n")
        ofile.write("\n")
        font_data = []
        glyphs = []
        bitmap_offset = 0
        with Image.open(FONT_FILE) as img:
            for c in range(ord(" "), ord("~") + 1):
                i = c - ord(" ")
                x = i % CHAR_PER_ROW
                y = int(i / CHAR_PER_ROW)
                char = get_char(img, x, y)
                font_data.extend(array_to_bytes(char))
                glyphs.append((chr(c), bitmap_offset, CHAR_W, CHAR_H, CHAR_W + 1, 0, 0))
                bitmap_offset += 4
        ofile.write(generate_data(font_data))
        ofile.write("\n")
        ofile.write(generate_glyph_string(glyphs))
        ofile.write("\n")
        ofile.write("const GFXfont font PROGMEM = {\n")
        ofile.write("  (uint8_t  *)font7pt,\n")
        ofile.write("  (GFXglyph *)fontGlyphs,\n")
        ofile.write("  {}, {}, {}".format(ord(" "), ord("~"), CHAR_H + 2) + "\n};\n")


if __name__ == "__main__":
    convert_font()