#ifndef LISTASENSOR_H
#define LISTASENSOR_H

#include "Nodo.h"
#include <iostream>

/**
 * Clase de Lista Enlazada Simple Genérica
 * Implementa operaciones básicas de inserción, búsqueda y liberación
 * Cumple con la Regla de los Tres/Cinco para gestión de memoria
 */
template <typename T>
class ListaSensor {
private:
    Nodo<T>* cabeza;  // Puntero al primer nodo
    int tamanio;      // Número de elementos en la lista
    
public:
    // Constructor por defecto
    ListaSensor() : cabeza(nullptr), tamanio(0) {
        std::cout << "[ListaSensor] Constructor - Lista creada" << std::endl;
    }
    
    // Constructor de copia
    ListaSensor(const ListaSensor& otra) : cabeza(nullptr), tamanio(0) {
        std::cout << "[ListaSensor] Constructor de copia" << std::endl;
        copiar(otra);
    }
    
    // Operador de asignación
    ListaSensor& operator=(const ListaSensor& otra) {
        std::cout << "[ListaSensor] Operador de asignación" << std::endl;
        if (this != &otra) {
            limpiar();
            copiar(otra);
        }
        return *this;
    }
    
    // Destructor
    ~ListaSensor() {
        std::cout << "[Destructor ListaSensor] Liberando lista..." << std::endl;
        limpiar();
    }
    
    // Insertar al final de la lista
    void insertarAlFinal(T dato) {
        Nodo<T>* nuevo = new Nodo<T>(dato);
        
        if (cabeza == nullptr) {
            cabeza = nuevo;
        } else {
            Nodo<T>* actual = cabeza;
            while (actual->siguiente != nullptr) {
                actual = actual->siguiente;
            }
            actual->siguiente = nuevo;
        }
        tamanio++;
        std::cout << "[Log] Insertando Nodo<" << typeid(T).name() << ">" << std::endl;
    }
    
    // Buscar un elemento en la lista
    Nodo<T>* buscar(T dato) const {
        Nodo<T>* actual = cabeza;
        while (actual != nullptr) {
            if (actual->dato == dato) {
                return actual;
            }
            actual = actual->siguiente;
        }
        return nullptr;
    }
    
    // Obtener el tamaño de la lista
    int getTamanio() const {
        return tamanio;
    }
    
    // Verificar si la lista está vacía
    bool estaVacia() const {
        return cabeza == nullptr;
    }
    
    // Obtener la cabeza de la lista
    Nodo<T>* getCabeza() const {
        return cabeza;
    }
    
    // Iterar sobre todos los elementos y aplicar una función
    template <typename Funcion>
    void iterar(Funcion f) const {
        Nodo<T>* actual = cabeza;
        while (actual != nullptr) {
            f(actual->dato);
            actual = actual->siguiente;
        }
    }
    
    // Limpiar toda la lista
    void limpiar() {
        while (cabeza != nullptr) {
            Nodo<T>* temp = cabeza;
            cabeza = cabeza->siguiente;
            std::cout << "[Log] Nodo<" << typeid(T).name() << "> liberado" << std::endl;
            delete temp;
            tamanio--;
        }
    }
    
private:
    // Función auxiliar para copiar otra lista
    void copiar(const ListaSensor& otra) {
        if (otra.cabeza == nullptr) {
            return;
        }
        
        Nodo<T>* actualOtra = otra.cabeza;
        while (actualOtra != nullptr) {
            insertarAlFinal(actualOtra->dato);
            actualOtra = actualOtra->siguiente;
        }
    }
};

#endif // LISTASENSOR_H
