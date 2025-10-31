/**
 * @file SensorBase.h
 * @brief Clase base abstracta para todos los sensores del sistema IoT
 * @author Sistema de Gestión IoT
 * @date Octubre 2025
 * @version 3.0
 * 
 * Este archivo define la interfaz común que deben implementar todos los
 * sensores del sistema. Utiliza polimorfismo para permitir el tratamiento
 * uniforme de diferentes tipos de sensores.
 */

#ifndef SENSORBASE_H
#define SENSORBASE_H

#include <iostream>
#include <cstring>

/**
 * @class SensorBase
 * @brief Clase base abstracta para todos los sensores
 * 
 * Define la interfaz común que deben implementar todos los sensores
 * del sistema. Esta clase implementa el patrón de diseño Template Method
 * y permite el polimorfismo en tiempo de ejecución.
 * 
 * @details
 * Características principales:
 * - Métodos virtuales puros para garantizar implementación en clases derivadas
 * - Destructor virtual para correcta liberación de memoria
 * - Identificador único para cada sensor
 * 
 * @note Esta clase no puede ser instanciada directamente
 * 
 * @see SensorTemperatura
 * @see SensorPresion
 */
class SensorBase {
protected:
    char nombre[50];  ///< Identificador único del sensor (máx. 50 caracteres)
    
public:
    /**
     * @brief Constructor de la clase base
     * @param id Identificador único del sensor (por defecto "SENSOR")
     * @details Inicializa el nombre del sensor con el ID proporcionado
     */
    SensorBase(const char* id = "SENSOR") {
        std::strncpy(nombre, id, 49);
        nombre[49] = '\0';
    }
    
    /**
     * @brief Destructor virtual
     * @details Garantiza la correcta destrucción de objetos derivados
     */
    virtual ~SensorBase() {
        std::cout << "[Destructor SensorBase] Liberando sensor: " << nombre << std::endl;
    }
    
    /**
     * @brief Procesa las lecturas almacenadas en el sensor
     * @details Método virtual puro que debe ser implementado por las clases derivadas.
     *          Cada tipo de sensor implementa su propia lógica de procesamiento.
     * @note Este método es virtual puro (= 0), por lo que debe ser implementado
     */
    virtual void procesarLectura() = 0;
    
    /**
     * @brief Imprime la información del sensor
     * @details Método virtual puro para mostrar información del sensor.
     *          Cada clase derivada define qué información mostrar.
     * @note Este método es virtual puro (= 0), por lo que debe ser implementado
     */
    virtual void imprimirInfo() const = 0;
    
    /**
     * @brief Obtiene el nombre/ID del sensor
     * @return const char* Puntero al nombre del sensor
     * @details Método público para acceder al identificador del sensor
     */
    const char* getNombre() const {
        return nombre;
    }
};

#endif // SENSORBASE_H
