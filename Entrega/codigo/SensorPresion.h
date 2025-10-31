#ifndef SENSORPRESION_H
#define SENSORPRESION_H

#include "SensorBase.h"
#include "ListaSensor.h"
#include <iostream>
#include <iomanip>

/**
 * Clase concreta SensorPresion
 * Representa un sensor que maneja lecturas de tipo int (presión)
 */
class SensorPresion : public SensorBase {
private:
    ListaSensor<int>* historial;  // Lista interna de lecturas
    
public:
    // Constructor
    SensorPresion(const char* id = "P-000") : SensorBase(id) {
        historial = new ListaSensor<int>();
        std::cout << "[Sensor Presion] Creado: " << nombre << std::endl;
    }
    
    // Destructor
    ~SensorPresion() override {
        std::cout << "[Destructor Sensor " << nombre << "]" << std::endl;
        delete historial;
    }
    
    // Agregar una nueva lectura
    void agregarLectura(int valor) {
        historial->insertarAlFinal(valor);
        std::cout << "[Log] Nodo<int> " << valor << " agregado" << std::endl;
    }
    
    // Implementación del método virtual puro
    void procesarLectura() override {
        if (historial->estaVacia()) {
            std::cout << "[Sensor Presion] No hay lecturas para procesar" << std::endl;
            return;
        }
        
        // Calcular el promedio de las lecturas
        int suma = 0;
        int contador = 0;
        
        historial->iterar([&suma, &contador](int valor) {
            suma += valor;
            contador++;
        });
        
        double promedio = static_cast<double>(suma) / contador;
        std::cout << "[Sensor Presion] Promedio calculado: " 
                  << std::fixed << std::setprecision(2) << promedio << std::endl;
    }
    
    // Implementación del método virtual puro
    void imprimirInfo() const override {
        std::cout << "\n=== Información del Sensor ===" << std::endl;
        std::cout << "Tipo: Sensor de Presión" << std::endl;
        std::cout << "ID: " << nombre << std::endl;
        std::cout << "Lecturas almacenadas: " << historial->getTamanio() << std::endl;
        
        if (!historial->estaVacia()) {
            std::cout << "Historial de lecturas: ";
            historial->iterar([](int valor) {
                std::cout << valor << " Pa ";
            });
            std::cout << std::endl;
        }
        std::cout << "==============================\n" << std::endl;
    }
    
    // Obtener el historial
    ListaSensor<int>* getHistorial() {
        return historial;
    }
};

#endif // SENSORPRESION_H
