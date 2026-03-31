#include <iostream>

int main() {
    // --- SECCIÓN 1: LA PILA (STACK) ---
    // Variable local normal
    int myAge = 19;
    // Puntero que guarda la dirección de una variable en la pila
    int* myPointer = &myAge;

    std::cout << "--- STACK ---" << std::endl;
    std::cout << "Value: " << myAge;
    std::cout << " Direction: " << myPointer << std::endl;

    // --- SECCIÓN 2: EL MONTÓN (HEAP) ---
    // Pedimos memoria dinámica usando 'new'
    int* pointer = new int(50);
    
    // Modificamos el valor directamente en la dirección de memoria
    *pointer = 100;

    std::cout << "\n--- HEAP ---" << std::endl;
    std::cout << "Value: " << *pointer;
    std::cout << " Direction: " << pointer << std::endl;

    // REGLA DE ORO: Liberar la memoria pedida en el Heap
    delete pointer;

    return 0;
}
