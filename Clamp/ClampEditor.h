#include <conio.h>
#include <iostream>
#include <vector>
#include <string>

// Funci�n para establecer el tema del editor
void EditorTheme() {
    /*
    color 09 para las l�neas no seleccionadas
    y 8b para la l�nea seleccionada
    */
    system("color 09");
}

// Funci�n para manejar la entrada de teclado y simular nuevas l�neas con Enter
void manejarEntrada() {
    std::vector<std::string> lineas;
    std::string lineaActual;
    char ch;

    std::cout << "Clamp editor [Exit (esc key)]" << std::endl;

    while (true) {
        ch = _getch();  // Capturar la entrada de teclado sin eco

        if (ch == 27) {  // C�digo ASCII para 'ESC'
            break;
        }
        else if (ch == '\r') {  // C�digo ASCII para Enter (Retorno de carro)
            lineas.push_back(lineaActual);
            lineaActual.clear();
            std::cout << "\n";
        }
        else if (ch == '\b') {  // Manejar tecla de retroceso
            if (!lineaActual.empty()) {
                lineaActual.pop_back();
                std::cout << "\b \b";  // Mover el cursor atr�s y borrar el car�cter
            }
        }
        else {
            lineaActual += ch;
            std::cout << ch;
        }
    }

    // Mostrar todas las l�neas al final
    std::cout << "\nContenido del editor:\n";
    for (const auto& linea : lineas) {
        std::cout << linea << "\n";
    }
}

// Funci�n del editor principal
void editor() {
    system("cls");
    EditorTheme();
    manejarEntrada();
}