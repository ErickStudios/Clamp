// Clamp.h
/*
    Clamp esta hecho por ErickCraftStudios vaya al sitio web llendo a el siguiente enlace
    https://sites.google.com/view/erick-studios-webpage/home-page
    
    Clamp esta disponible bajo CC-BY aunque es opcional
    pero por lo menos dame creditos si vas a usar la base, conste eh

    codigo de la consola de comandos de Clamp
*/

#include "Clamp.h" // incluir el interprete que ejecutara los comandos y los definira

// consola
int main() {
    // lineas
    std::string linea;

    // Mensaje de bienvenida
    Echo("\033[0mWelcome to \x1B[36mClamp idle\033[0m");
    Echo("");
    Echo("Madded by ErickCraftStudiosStudios\x1B[35m https://sites.google.com/view/erick-studios-webpage/home-page\033[0m");
    Echo("made on visual studio and compiled by c++ compiler");
    Echo("");
    Echo("\x1B[32m// type help to get all list of commands");
    Echo("");

    // bucle de la terminal
    while (true) {
        // prefijo
        std::cout << "\x1B[96m@MK\\" << "\\" << "\x1B[36m" << real_directory << "\x1B[35m> \033[0m";
        std::getline(std::cin, linea);

        // ejecutar el comando
        ParseCommand(linea);
    }

    return 0;
}
