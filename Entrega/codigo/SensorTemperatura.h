#ifndef SENSORTEMPERATURA_H
#define SENSORTEMPERATURA_H

#include "SensorBase.h"
#include "ListaSensor.h"
#include <iostream>
#include <iomanip>

/**
 * Clase concreta SensorTemperatura
 * Representa un sensor que maneja lecturas de tipo float (temperatura)
 */
class SensorTemperatura : public SensorBase {
private:
    ListaSensor<float>* historial;  // Lista interna de lecturas
    
public:
    // Constructor
    SensorTemperatura(const char* id = "T-000") : SensorBase(id) {
        historial = new ListaSensor<float>();
        std::cout << "[Sensor Temperatura] Creado: " << nombre << std::endl;
    }
    
    // Destructor
    ~SensorTemperatura() override {
        std::cout << "[Destructor Sensor " << nombre << "]" << std::endl;
        delete historial;
    }
    
    // Agregar una nueva lectura
    void agregarLectura(float valor) {
        historial->insertarAlFinal(valor);
        std::cout << "[Log] Nodo<float> " << std::fixed << std::setprecision(1) 
                  << valor << " agregado" << std::endl;
    }
    
    // Implementación del método virtual puro
    void procesarLectura() override {
        if (historial->estaVacia()) {
            std::cout << "[Sensor Temp] No hay lecturas para procesar" << std::endl;
            return;
        }
        
        // Calcular el valor más bajo
        float minimo = 999999.0f;
        historial->iterar([&minimo](float valor) {
            if (valor < minimo) {
                minimo = valor;
            }
        });
        
        std::cout << "[Sensor Temp] Lectura minima calculada: " 
                  << std::fixed << std::setprecision(1) << minimo << std::endl;
    }
    
    // Implementación del método virtual puro
    void imprimirInfo() const override {
        std::cout << "\n=== Información del Sensor ===" << std::endl;
        std::cout << "Tipo: Sensor de Temperatura" << std::endl;
        std::cout << "ID: " << nombre << std::endl;
        std::cout << "Lecturas almacenadas: " << historial->getTamanio() << std::endl;
        
        if (!historial->estaVacia()) {
            std::cout << "Historial de lecturas: ";
            historial->iterar([](float valor) {
                std::cout << std::fixed << std::setprecision(1) << valor << "°C ";
            });
            std::cout << std::endl;
        }
        std::cout << "==============================\n" << std::endl;
    }
    
    // Obtener el historial
    ListaSensor<float>* getHistorial() {
        return historial;
    }
};

#endif // SENSORTEMPERATURA_H
