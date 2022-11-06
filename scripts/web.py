from math import trunc
import os
import subprocess
import pathlib


BYTES_PER_LINE = 15

def get_fn(name, extension):
    content_type = "text/html"
    if extension == ".css":
        content_type = "text/css"
    elif extension == ".js":
        content_type = "text/javascript"
    out = ""
    out += "void fn_" + name + "(ESP8266WebServer &server) {\n"
    out += "    server.sendHeader(\"Content-Encoding\", \"gzip\");\n"
    out += "    server.send(200, \"" + content_type + "\", (const char*)" + name + ", " + name + "_len);\n"
    out += "}\n"
    return out


# def print_gzip_output(ofile, data):
#     w = 0
#     ofile.write("    ")
#     for b in data:
#         ofile.write("\\x{:02X}".format(b))
#         w += 1
#         if w > BYTES_PER_LINE:
#             w = 0
#             ofile.write("\\\n")
#             ofile.write("    ")


#gzip --stdout ./web/build/200.html | xxd -i
def generate_server_header(file):
    with open(file, mode="w") as ofile:
        ofile.write("//\n")
        ofile.write("// Auto generated file do not edit manualy\n")
        ofile.write("// change will be overwriten\n")
        ofile.write("//\n")
        ofile.write("\n")
        ofile.write("#pragma once\n")
        ofile.write("#include <Arduino.h>\n")
        ofile.write("#include <ESP8266WiFi.h>\n")
        ofile.write("#include <ESP8266WebServer.h>\n")
        ofile.write("#include \"callback.h\"\n")
        ofile.write("\n")

        fns = [("/", "fn_web_index")]
        size = 0
        for file in os.scandir("web/build"):
            if file.is_file():
                path = file.path
                out = subprocess.run("gzip --stdout {} | xxd -i".format(path), shell=True, text=True, capture_output=True)
                name = "web_" + pathlib.Path(path).stem.replace(".", "_").replace("-", "_").replace("~", "_")
                s = out.stdout.count(",") + 1
                ofile.write("static const int {}_len = {};\n".format(name, s))
                size += s
                ofile.write("static const uint8_t {}[] PROGMEM = ".format(name))
                ofile.write("{\n")
                ofile.write(out.stdout)
                ofile.write("};\n")
                ofile.write(get_fn(name, pathlib.Path(path).suffix))
                ofile.write("\n")
                fns.append((path.removeprefix("web/build"), "fn_{}".format(name)))
                print("size: {:5}, path: {}".format(s, path))

        ofile.write("\n")
        ofile.write("void setup_preact(ESP8266WebServer &server) {\n")
        for (path, fn) in fns:
            ofile.write("   server.on(\"{}\", callback(server, {}));\n".format(path, fn))
        ofile.write("}\n")
        print("Total gzip size:", size, "kb")



if __name__ == "__main__":
    print("Building website")
    subprocess.run("npm run build && rm build/*.map", shell=True, cwd="web")
    generate_server_header("include/website.h")