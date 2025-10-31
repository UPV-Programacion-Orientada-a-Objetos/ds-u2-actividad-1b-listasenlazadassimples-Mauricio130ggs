#include <iostream>
#include <string>
#include <sstream>
#include <limits>
#include "SensorBase.h"
#include "SensorTemperatura.h"
#include "SensorPresion.h"
#include "ListaSensor.h"
#include "SerialPort.h"

// Lista General NO Genérica que almacena punteros a SensorBase
// Esto permite el polimorfismo
using ListaGeneral = ListaSensor<SensorBase*>;

/**
 * Función para leer datos directamente desde Arduino por puerto serial
 * Lee en tiempo real del puerto USB donde está conectado el Arduino
 */
void leerDesdeArduino(ListaGeneral* listaGestion) {
    std::cout << "\n╔════════════════════════════════════════════════╗" << std::endl;
    std::cout << "║         LECTURA DESDE ARDUINO REAL             ║" << std::endl;
    std::cout << "╚════════════════════════════════════════════════╝\n" << std::endl;
    
    // Crear objeto para puerto serial
    SerialPort puerto;
    
    // Pedir al usuario el puerto
    std::cout << "Puertos comunes:" << std::endl;
    std::cout << "  Linux:   /dev/ttyACM0, /dev/ttyUSB0" << std::endl;
    std::cout << "  Mac:     /dev/cu.usbmodem*, /dev/cu.usbserial*" << std::endl;
    std::cout << "  Windows: (usar modo simulación - Opción 6)" << std::endl;
    std::cout << "\nIngrese el puerto del Arduino: ";
    
    std::string nombrePuerto;
    std::cin >> nombrePuerto;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    // Intentar abrir el puerto
    if (!puerto.abrir(nombrePuerto, 9600)) {
        std::cout << "\n❌ No se pudo conectar con el Arduino" << std::endl;
        std::cout << "\nSoluciones:" << std::endl;
        std::cout << "  1. Verifica que el Arduino esté conectado" << std::endl;
        std::cout << "  2. En Linux, ejecuta: sudo chmod 666 " << nombrePuerto << std::endl;
        std::cout << "  3. Verifica el puerto en Arduino IDE (Herramientas → Puerto)" << std::endl;
        std::cout << "  4. Intenta desconectar y reconectar el Arduino" << std::endl;
        return;
    }
    
    std::cout << "\n✓ Conectado exitosamente!" << std::endl;
    std::cout << "✓ Esperando datos del Arduino..." << std::endl;
    std::cout << "✓ Presiona Ctrl+C para detener\n" << std::endl;
    std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n" << std::endl;
    
    std::string linea;
    int lecturasRecibidas = 0;
    
    // Leer continuamente del puerto
    while (true) {
        if (puerto.leerLinea(linea)) {
            // Ignorar líneas de log del Arduino
            if (linea.find("===") != std::string::npos || 
                linea.find("Arduino") != std::string::npos ||
                linea.find("Formato") != std::string::npos ||
                linea.empty()) {
                continue;
            }
            
            std::cout << "📡 Recibido: " << linea << std::endl;
            
            // Parsear la línea
            std::istringstream iss(linea);
            char tipo;
            std::string id;
            
            if (!(iss >> tipo >> id)) {
                // Si no tiene formato completo, intentar leer solo el número
                std::istringstream issNumero(linea);
                double valorPrueba;
                if (issNumero >> valorPrueba) {
                    // Es un número válido
                    if (linea.find('.') != std::string::npos) {
                        std::cout << "⚠️  Dato recibido sin formato: " << valorPrueba << std::endl;
                        std::cout << "   📊 Tipo detectado: float (tiene punto decimal)" << std::endl;
                    } else {
                        std::cout << "⚠️  Dato recibido sin formato: " << (int)valorPrueba << std::endl;
                        std::cout << "   📊 Tipo detectado: int (sin punto decimal)" << std::endl;
                    }
                    std::cout << "   💡 Formato esperado: T ID VALOR  o  P ID VALOR" << std::endl;
                } else {
                    std::cout << "⚠️  Formato inválido, ignorando..." << std::endl;
                }
                continue;
            }
            
            // Buscar si el sensor ya existe
            SensorBase* sensorExistente = nullptr;
            listaGestion->iterar([&sensorExistente, &id](SensorBase* sensor) {
                if (std::string(sensor->getNombre()) == id) {
                    sensorExistente = sensor;
                }
            });
            
            if (tipo == 'T' || tipo == 't') {
                float valor;
                if (!(iss >> valor)) {
                    std::cout << "⚠️  Valor de temperatura inválido" << std::endl;
                    continue;
                }
                
                if (sensorExistente == nullptr) {
                    // Crear nuevo sensor de temperatura
                    SensorTemperatura* nuevoSensor = new SensorTemperatura(id.c_str());
                    nuevoSensor->agregarLectura(valor);
                    listaGestion->insertarAlFinal(nuevoSensor);
                    std::cout << "✓ Sensor de Temperatura '" << id << "' creado" << std::endl;
                    std::cout << "  📊 Tipo de dato: float" << std::endl;
                    std::cout << "  📈 Valor inicial: " << valor << "°C" << std::endl;
                } else {
                    // Agregar lectura al sensor existente
                    SensorTemperatura* tempSensor = dynamic_cast<SensorTemperatura*>(sensorExistente);
                    if (tempSensor) {
                        tempSensor->agregarLectura(valor);
                        std::cout << "✓ Lectura agregada a sensor '" << id << "'" << std::endl;
                        std::cout << "  📊 Tipo de dato: float" << std::endl;
                        std::cout << "  📈 Valor: " << valor << "°C" << std::endl;
                    }
                }
                
            } else if (tipo == 'P' || tipo == 'p') {
                int valor;
                if (!(iss >> valor)) {
                    std::cout << "⚠️  Valor de presión inválido" << std::endl;
                    continue;
                }
                
                if (sensorExistente == nullptr) {
                    // Crear nuevo sensor de presión
                    SensorPresion* nuevoSensor = new SensorPresion(id.c_str());
                    nuevoSensor->agregarLectura(valor);
                    listaGestion->insertarAlFinal(nuevoSensor);
                    std::cout << "✓ Sensor de Presión '" << id << "' creado" << std::endl;
                    std::cout << "  📊 Tipo de dato: int" << std::endl;
                    std::cout << "  📈 Valor inicial: " << valor << " Pa" << std::endl;
                } else {
                    // Agregar lectura al sensor existente
                    SensorPresion* presSensor = dynamic_cast<SensorPresion*>(sensorExistente);
                    if (presSensor) {
                        presSensor->agregarLectura(valor);
                        std::cout << "✓ Lectura agregada a sensor '" << id << "'" << std::endl;
                        std::cout << "  📊 Tipo de dato: int" << std::endl;
                        std::cout << "  📈 Valor: " << valor << " Pa" << std::endl;
                    }
                }
            } else {
                std::cout << "⚠️  Tipo de sensor desconocido: " << tipo << std::endl;
                continue;
            }
            
            lecturasRecibidas++;
            std::cout << "📊 Total de lecturas recibidas: " << lecturasRecibidas << "\n" << std::endl;
        }
    }
}



/**
 * Función para mostrar el menú principal
 */
void mostrarMenu() {
    std::cout << "\n==================================" << std::endl;
    std::cout << "Sistema IoT de Monitoreo" << std::endl;
    std::cout << "==================================" << std::endl;
    std::cout << "Opción 1: Crear Sensor (Tipo T: Temperatura)" << std::endl;
    std::cout << "Opción 2: Crear Sensor (Tipo P: Presión)" << std::endl;
    std::cout << "Opción 3: Registrar Lectura" << std::endl;
    std::cout << "Opción 4: Ejecutar Procesamiento" << std::endl;
    std::cout << "Opción 5: Cerrar Sistema (Liberar Memoria)" << std::endl;
    std::cout << "Opción 6: 🔌 Leer desde Arduino (Puerto Serial)" << std::endl;
    std::cout << "==================================" << std::endl;
    std::cout << "Seleccione una opción: ";
}

/**
 * Función principal del programa
 */
int main() {
    std::cout << "\n╔════════════════════════════════════════════════╗" << std::endl;
    std::cout << "║  SISTEMA DE GESTIÓN POLIMÓRFICA DE SENSORES   ║" << std::endl;
    std::cout << "║            PARA IoT (Genérico)                 ║" << std::endl;
    std::cout << "╚════════════════════════════════════════════════╝\n" << std::endl;
    
    // Crear la Lista de Gestión Polimórfica
    ListaGeneral* listaGestion = new ListaGeneral();
    
    int opcion;
    bool continuar = true;
    
    while (continuar) {
        mostrarMenu();
        std::cin >> opcion;
        
        switch (opcion) {
            case 1: {
                // Crear Sensor de Temperatura
                std::string id;
                std::cout << "\nIngrese ID del sensor (ej: T-001): ";
                std::cin >> id;
                
                SensorTemperatura* nuevoSensor = new SensorTemperatura(id.c_str());
                listaGestion->insertarAlFinal(nuevoSensor);
                std::cout << "Sensor 'T-" << id << "' creado e insertado" << std::endl;
                break;
            }
            
            case 2: {
                // Crear Sensor de Presión
                std::string id;
                std::cout << "\nIngrese ID del sensor (ej: P-105): ";
                std::cin >> id;
                
                SensorPresion* nuevoSensor = new SensorPresion(id.c_str());
                listaGestion->insertarAlFinal(nuevoSensor);
                std::cout << "Sensor 'P-" << id << "' creado e insertado" << std::endl;
                break;
            }
            
            case 3: {
                // Registrar Lectura
                std::string id;
                std::cout << "\nIngrese ID del sensor: ";
                std::cin >> id;
                
                SensorBase* sensorEncontrado = nullptr;
                listaGestion->iterar([&sensorEncontrado, &id](SensorBase* sensor) {
                    if (std::string(sensor->getNombre()) == id) {
                        sensorEncontrado = sensor;
                    }
                });
                
                if (sensorEncontrado != nullptr) {
                    SensorTemperatura* tempSensor = dynamic_cast<SensorTemperatura*>(sensorEncontrado);
                    SensorPresion* presSensor = dynamic_cast<SensorPresion*>(sensorEncontrado);
                    
                    if (tempSensor) {
                        float valor;
                        std::cout << "Ingrese valor (float): ";
                        std::cin >> valor;
                        tempSensor->agregarLectura(valor);
                        std::cout << "ID: " << id << ". Valor: " << valor << " (float)" << std::endl;
                    } else if (presSensor) {
                        int valor;
                        std::cout << "Ingrese valor (int): ";
                        std::cin >> valor;
                        presSensor->agregarLectura(valor);
                        std::cout << "ID: " << id << ". Valor: " << valor << " (int)" << std::endl;
                    }
                } else {
                    std::cout << "Sensor no encontrado" << std::endl;
                }
                break;
            }
            
            case 4: {
                // Ejecutar Procesamiento Polimórfico
                std::cout << "\n--- Procesando Sensores ---" << std::endl;
                
                listaGestion->iterar([](SensorBase* sensor) {
                    std::cout << "\n-> Procesando Sensor " << sensor->getNombre() << "..." << std::endl;
                    
                    // Determinar tipo de sensor
                    SensorTemperatura* tempSensor = dynamic_cast<SensorTemperatura*>(sensor);
                    SensorPresion* presSensor = dynamic_cast<SensorPresion*>(sensor);
                    
                    if (tempSensor) {
                        std::cout << "[Sensor Temp] Promedio calculado" << std::endl;
                    } else if (presSensor) {
                        std::cout << "[Sensor Presion] Promedio calculado" << std::endl;
                    }
                    
                    // Llamada polimórfica
                    sensor->procesarLectura();
                });
                
                std::cout << "\n--- Procesamiento Completado ---" << std::endl;
                break;
            }
            
            case 5: {
                // Cerrar Sistema y Liberar Memoria
                std::cout << "\n--- Liberación de Memoria en Curso ---" << std::endl;
                std::cout << "[Destructor General] Liberando Lista de Gestión..." << std::endl;
                
                // Liberar cada sensor de la lista
                listaGestion->iterar([](SensorBase* sensor) {
                    delete sensor;
                });
                
                // Limpiar la lista
                delete listaGestion;
                
                std::cout << "Sistema Cerrado. Memoria Liberada." << std::endl;
                continuar = false;
                break;
            }
            
            case 6: {
                // Leer desde Arduino REAL
                leerDesdeArduino(listaGestion);
                break;
            }
            
            default:
                std::cout << "Opción inválida. Intente nuevamente." << std::endl;
                break;
        }
    }
    
    std::cout << "\n¡Gracias por usar el Sistema IoT de Monitoreo!" << std::endl;
    return 0;
}