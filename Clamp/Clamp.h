// Clamp.h
/*
    Clamp esta hecho por ErickCraftStudios vaya al sitio web llendo a el siguiente enlace
    https://sites.google.com/view/erick-studios-webpage/home-page

    Clamp esta disponible bajo CC-BY aunque es opcional
    pero por lo menos dame creditos si vas a usar la base, conste eh

    como no se de encabezado puedo simplemente copiar el codigo del interprete
    y pegarlo ya para que el archivo prinipal se mas corto
*/

#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>
#include <filesystem>
#include <Windows.h> 
#include <Lmcons.h>
#include "ClampEditor.h"

using namespace std;
namespace fs = std::filesystem;

// emvio de datos
struct SendData {   // Declare data to send struct type
    string name;   // name
    string value;   // value
} SendData;

std::string obtenerNombreDeUsuario() {
    char nombreUsuario[UNLEN + 1]; 
    DWORD tamanoNombreUsuario = UNLEN + 1;
    if (GetUserNameA(nombreUsuario, &tamanoNombreUsuario)) {
        return std::string(nombreUsuario); 
    } else { 
        return "error"; 
    } 
}

std::string Username = obtenerNombreDeUsuario();

std::string real_directory = "C:\\Users"; // Ruta real del usuario
std::string console_directory = "~/";     // Ruta que se muestra en la consola

// Simulación de variables, funciones y plugins
std::map<std::string, std::string> variables;
std::map<std::string, std::vector<std::string>> funciones;
std::map<std::string, bool> plugins;
std::set<std::string> valid_plugins = { "DevTools", "String", "Functions", "Abreviations" }; // Lista de plugins válidos

// Función para definir una variable
void Define(const std::string& nombre, const std::string& valor) {
    variables[nombre] = valor;
}

// Función para normalizar rutas
std::string NormalizePath(const std::string& path) {
    std::vector<std::string> parts;
    std::istringstream iss(path);
    std::string part;
    while (std::getline(iss, part, '\\')) {
        if (part == ".." && !parts.empty()) {
            parts.pop_back();
        }
        else if (!part.empty() && part != ".") {
            parts.push_back(part);
        }
    }

    std::string normalized_path;
    for (const auto& p : parts) {
        normalized_path += "\\" + p;
    }

    // Eliminar barra final redundante excepto para la raíz
    if (normalized_path.size() > 1 && normalized_path.back() == '\\') {
        normalized_path.pop_back();
    }

    return normalized_path;
}

void ShowDirectoryContents() {
    // Verificar si el directorio actual existe
    if (fs::exists(real_directory) && fs::is_directory(real_directory)) {
        std::cout << "" << std::endl;
        std::cout << "Directory contents of: " << real_directory << std::endl;
        std::cout << "" << std::endl;

        // Iterar sobre los archivos y directorios dentro de `real_directory`
        for (const auto& entry : fs::directory_iterator(real_directory)) {
            // Mostrar solo archivos y directorios (sin entrar en subcarpetas)
            if (fs::is_directory(entry.status())) {
                // Si es un directorio, solo mostrar el nombre de la carpeta
                std::cout << "\x1B[36m[folder] " << "\033[0m" << entry.path().filename().string() << std::endl;
            }
            else if (fs::is_regular_file(entry.status())) {
                // Si es un archivo, mostrar el nombre del archivo
                std::cout << "\x1B[36m[file] " << "\033[0m" << entry.path().filename().string() << std::endl;
            }
        }
        std::cout << "" << std::endl;
    }
    else {
        std::cerr << "The current directory doesn't exist or is not a valid directory." << real_directory << std::endl;
    }
}

void ChangeDirectory(const std::string& path) {
    std::string new_real_directory;

    // Eliminar la barra invertida al principio si está presente
    std::string path_to_use = (path[0] == '\\') ? path.substr(1) : path;

    if (path == "\\") {
        new_real_directory = "C:\\Users"; // Reinicia a la raíz del usuario
    }
    else if (path == "..") {
        // Retroceder al directorio padre
        size_t pos = real_directory.find_last_of('\\');
        new_real_directory = (pos <= 9) ? "C:\\Users" : real_directory.substr(0, pos);
    }
    else if (path[0] == '\\') {
        // Ruta absoluta, ya procesada por la eliminación de '\'
        new_real_directory = "C:\\Users" + path_to_use;
    }
    else {
        // Ruta relativa
        new_real_directory = real_directory + "\\" + path_to_use;
    }

    // Normalizar la ruta real
    new_real_directory = NormalizePath(new_real_directory);

    if (new_real_directory.substr(0, 1) == "\\")
    {
        new_real_directory = new_real_directory.substr(1, new_real_directory.length());
    }
    // Actualizar la variable real_directory
    real_directory = new_real_directory;
}

// Función para mostrar un texto
void Echo(const std::string& texto) {
    std::cerr << texto << std::endl;
}

// Función para mostrar el valor de una variable
void EchoVar(const std::string& nombre) {
    if (variables.find(nombre) != variables.end()) {
        Echo(variables[nombre]);
    }
}

// Función para leer un texto ingresado por el usuario
std::string TextInput(const std::string& mensaje) {
    std::string texto;
    std::cout << mensaje << ": ";
    std::getline(std::cin, texto);
    return texto;
}

void ShowSysCommands() {
    Echo("List of SysCommands:");
    Echo("");
    Echo("the SysCommands can be called writing '#'");
    Echo("the next list is a list of the commands can be called by that method");
    Echo("");
    Echo("#add <module>: this command is for add a plugin");
    Echo("#helpp <module>: this command is for get a docummentation of who a specific plugin worcks");
    Echo("#cd <folder>: this command is for change the directory");
    Echo("#dir : this command is for display a list of the files and folders in the current directiry");
}

void SpecificPluginHelp(const std::string& hplugin) {
    if (hplugin == "DevTools") {
        Echo("To use this plugin you need to put the next line");
        Echo("\033[0m\n#add \x1B[36mDevTools\033[0m\n");
        Echo("tools.var \x1B[36m<variable> \033[0m= \x1B[35m<value>\033[0m : this command is use for define a variable , you need to insert the name of variable to edit/create before '=' and after of '=' you need to insert the value of variable to edit/create");
        return;
    }
    else if (hplugin == "String")
    {
        Echo("To use this plugin you need to put the next line");
        Echo("\033[0m\n#add \x1B[36mString\033[0m\n");
        Echo("string.echo:var \x1B[36m<variable>\033[0m : this command is for display a value of a defined variable (use this command with tools.var of plugin 'DevTools' and for more info type '#helpp DevTools')");
        Echo("string.echo:textinput \x1B[36m<ask>\033[0m : this command is request to user to put a text into a textbox of console");
        return;
    }
}

void ShowPluginError(const std::string plugin) {
    std::cerr << "\033[1;31m\nThe command contains the follow errors : \n\nin the line 1 of command at the letter 5 to end of command\n the plugin '" << plugin << "' has dont exist\033[0m\n" << std::endl; // Texto en rojo
}

// Función para configurar plugins
void SetPlugin(const std::string& plugin, bool state) {
    plugins[plugin] = state;
}

// Función para mostrar la lista de comandos disponibles
void ShowHelp() {
    ShowSysCommands();
}

// Analizador de comandos
void ParseCommand(const std::string& linea) {
    if (linea.empty()) {
        return; // Salir de la función si la línea está vacía
    }

    if (linea.substr(0, 2) == "//") {
        return; // Ignorar comentarios
    }

    std::istringstream iss(linea);
    std::string comando;
    iss >> comando;

    if (comando == "#add") {
        std::string module;
        iss >> module;
        if (valid_plugins.find(module) != valid_plugins.end()) {
            SetPlugin(module, "true");
        }
        else {
            ShowPluginError(module);
        }
    }
    else if (comando == "tools.var") {
        if (plugins["DevTools"]) {
            std::string variable, igual, valor, line;
            iss >> variable >> igual;
            if (igual != "=") {
                std::cerr << "\033[1;31m\nThe command contains the follow errors : \n\nin the line of variable definition the '=' separator is not founded\na valid example is '\n\n\x1B[32m//add plugin for devloper tools\033[0m\n#add \x1B[36mDevTools\n\n\x1B[32m//define variables\n\033[0mtools.var \x1B[36mmy_variable \033[0m= \x1B[35m0\033[0m\n" << std::endl; // Texto en rojo
                return;
            }
            std::getline(iss, valor);
            // Manejar variables de tipo "array"
            if (valor == " [[") {
                int number_of_array = 1; // Inicializar contador de índice para el array
                std::cout << "\x1B[35m";
                while (std::getline(std::cin, line)) {
                    if (line == "]]") {
                        break; // Finalizar si se cierra el array
                    }
                    Define(variable + "." + std::to_string(number_of_array), line); // Almacenar cada línea como un elemento del "array"
                    number_of_array++;
                }
            }
            else {
                // Manejar variables normales (no arrays)
                valor = valor.substr(2, valor.size() - 3); // Remover comillas
                Define(variable, valor);
            }
        }
    }
    else if (comando == "var") {
        if (plugins["Abreviations"]) {
            if (plugins["DevTools"]) {
                std::string variable, igual, valor , line;
                iss >> variable >> igual;
                if (igual != "=") {
                    std::cerr << "\033[1;31m\nThe command contains the follow errors : \n\nin the line of variable definition the '=' separator is not founded\na valid example is '\n\n\x1B[32m//add plugin for devloper tools\033[0m\n#add \x1B[36mDevTools\n\n\x1B[32m//define variables\n\033[0mtools.var \x1B[36mmy_variable \033[0m= \x1B[35m0\033[0m\n" << std::endl; // Texto en rojo
                    return;
                }
                std::getline(iss, valor);
                // Manejar variables de tipo "array"
                if (valor == " [[") {
                    int number_of_array = 1; // Inicializar contador de índice para el array
                    std::cout << "\x1B[35m";
                    while (std::getline(std::cin, line)) {
                        if (line == "]]") {
                            break; // Finalizar si se cierra el array
                        }
                        Define(variable + "." + std::to_string(number_of_array), line); // Almacenar cada línea como un elemento del "array"
                        number_of_array++;
                    }
                }
                else {
                    // Manejar variables normales (no arrays)
                    valor = valor.substr(2, valor.size() - 3); // Remover comillas
                    Define(variable, valor);
                }
            }
        }
    }
    else if (comando == "console.echo") {
        std::string texto;
        std::getline(iss, texto);
        Echo(texto);
    }
    else if (comando == "echo") {
        if (plugins["Abreviations"]) {
            std::string texto;
            std::getline(iss, texto);
            Echo(texto);
        }
    }
    else if (comando == "#function") {
        std::string texto;
        std::getline(iss, texto);
        for (const auto& val : funciones[texto]) {
            ParseCommand(val);
        }
    }
    else if (comando == "#helpp") {
        std::string module;
        iss >> module;
        if (valid_plugins.find(module) != valid_plugins.end()) {
            SpecificPluginHelp(module);
        }
        else {
            ShowPluginError(module);
        }
    }
    else if (comando == "string.echo:var") {
        if (plugins["String"]) {
            std::string nombre;
            iss >> nombre;
            EchoVar(nombre);
        }
    }
    else if (comando == "echo:var") {
        if (plugins["Abreviations"]) {
            if (plugins["String"]) {
                std::string nombre;
                iss >> nombre;
                EchoVar(nombre);
            }
        }
    }
    else if (comando == "string.echo:textinput") {
        if (plugins["String"]) {
            std::string mensaje;
            std::getline(iss, mensaje);
            std::string resultado = TextInput(mensaje);
            Echo(resultado);
        }
    }
    else if (comando == "echo:input") {
        if (plugins["Abreviations"]) {
            if (plugins["String"]) {
                std::string mensaje;
                std::getline(iss, mensaje);
                std::string resultado = TextInput(mensaje);
                Echo(resultado);
            }
        }
    }
    else if (comando == "function") {
        if (plugins["Functions"]) {
            std::string func_name, params, line;
            std::getline(iss, func_name, '(');
            std::getline(iss, params, ')');
            std::vector<std::string> func_body;
            while (std::getline(std::cin, line) && line != "}") {
                func_body.push_back(line);
            }
            funciones[func_name] = func_body;
        }
    }
    else if (comando == "#main:{") {
        std::string line;
        std::vector<std::string> func_body;
        while (std::getline(std::cin, line) && line != "}") {
            func_body.push_back(line);
        }
        funciones["main"] = func_body;
    }
    else if (comando == "help") {
        ShowHelp();
    }
    else if (comando == "#dir") {
        ShowDirectoryContents();
    }
    else if (comando == "#cd") {
        std::string path;
        iss >> path;
        if (path.empty()) {
            std::cerr << "\033[1;31m\nThe command contains the follow errors : \n\nthe directory or folder dont exist\n" << std::endl; // Texto en rojo
        }
        else {
            ChangeDirectory(path);
        }
    }
    else if (comando == "#exc") {
        std::string path;
        iss >> path;  // Obtener la ruta del archivo a ejecutar
    }
    else if (comando == "#") {
        ShowSysCommands();
    }
    else if (comando == "#editor") {
        editor();
    }
    else {
        std::cerr << "\033[1;31m\nThe command contains the follow errors : \n\nthe command dont exist or not defined\nplease insert a valid command or read command docummentation writing 'help'\n" << std::endl; // Texto en rojo
    }
}

// Función para ejecutar el script actual 
void ExecuteProgram(const std::vector<std::string>& commands) {
    for (const auto& val : commands) {
        ParseCommand(val);
    }
}