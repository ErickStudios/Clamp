#include <conio.h>
#include <iostream>
#include <vector>
#include <string>

// Función para establecer el tema del editor
void EditorTheme() {
    /*
    color 09 para las líneas no seleccionadas
    y 8b para la línea seleccionada
    */
    system("color 09");
}

// Función para manejar la entrada de teclado y simular nuevas líneas con Enter
void manejarEntrada() {
    std::vector<std::string> lineas;
    std::string lineaActual;
    char ch;

    std::cout << "Clamp editor [Exit (esc key)]" << std::endl;

    while (true) {
        ch = _getch();  // Capturar la entrada de teclado sin eco

        if (ch == 27) {  // Código ASCII para 'ESC'
            break;
        }
        else if (ch == '\r') {  // Código ASCII para Enter (Retorno de carro)
            lineas.push_back(lineaActual);
            lineaActual.clear();
            std::cout << "\n";
        }
        else if (ch == '\b') {  // Manejar tecla de retroceso
            if (!lineaActual.empty()) {
                lineaActual.pop_back();
                std::cout << "\b \b";  // Mover el cursor atrás y borrar el carácter
            }
        }
        else {
            lineaActual += ch;
            std::cout << ch;
        }
    }

    // Mostrar todas las líneas al final
    std::cout << "\nContenido del editor:\n";
    for (const auto& linea : lineas) {
        std::cout << linea << "\n";
    }
}

// Función del editor principal
void editor() {
    system("cls");
    EditorTheme();
    manejarEntrada();
}