/**
 * @file Nodo.h
 * @brief Estructura de nodo genérico para listas enlazadas
 * @author Sistema de Gestión IoT
 * @date Octubre 2025
 * @version 3.0
 * 
 * Define la estructura básica de un nodo para implementar
 * listas enlazadas simples de manera genérica usando templates.
 */

#ifndef NODO_H
#define NODO_H

/**
 * @struct Nodo
 * @brief Estructura de nodo genérico para Lista Enlazada Simple
 * @tparam T Tipo de dato que almacena el nodo
 * 
 * @details
 * Esta estructura implementa un nodo básico para listas enlazadas.
 * Utiliza templates de C++ para ser independiente del tipo de dato,
 * permitiendo crear listas de enteros, flotantes, punteros, etc.
 * 
 * Características:
 * - Genérico mediante templates
 * - Contiene un dato de tipo T
 * - Apunta al siguiente nodo de la lista
 * - Constructor simplificado
 * 
 * @note El uso de templates permite reutilización de código
 * 
 * Ejemplo de uso:
 * @code
 * Nodo<int>* nodoEntero = new Nodo<int>(42);
 * Nodo<float>* nodoFlotante = new Nodo<float>(3.14);
 * Nodo<SensorBase*>* nodoSensor = new Nodo<SensorBase*>(miSensor);
 * @endcode
 */
template <typename T>
struct Nodo {
    T dato;                 ///< Dato almacenado en el nodo (tipo genérico T)
    Nodo<T>* siguiente;     ///< Puntero al siguiente nodo en la lista
    
    /**
     * @brief Constructor del nodo
     * @param valor Valor a almacenar en el nodo
     * @details Inicializa el dato con el valor proporcionado y
     *          el puntero siguiente a nullptr
     */
    Nodo(T valor) : dato(valor), siguiente(nullptr) {}
};

#endif // NODO_H
