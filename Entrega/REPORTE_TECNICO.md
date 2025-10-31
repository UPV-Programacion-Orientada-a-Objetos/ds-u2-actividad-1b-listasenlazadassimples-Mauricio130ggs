# REPORTE TÉCNICO
## Sistema de Gestión Polimórfica de Sensores para IoT

---

# 1. INTRODUCCIÓN

## 1.1 Contexto del Proyecto

En la era del Internet de las Cosas (IoT), la gestión eficiente de múltiples sensores heterogéneos representa un desafío técnico significativo. Los sistemas modernos requieren soluciones que permitan:

- **Escalabilidad**: Agregar nuevos tipos de sensores sin modificar el código base
- **Eficiencia**: Gestión óptima de recursos de memoria
- **Flexibilidad**: Procesamiento específico para cada tipo de sensor
- **Integración**: Comunicación en tiempo real con hardware físico

Este proyecto implementa un sistema de gestión polimórfica que aborda estos desafíos mediante el uso de técnicas avanzadas de Programación Orientada a Objetos (POO) en C++.

## 1.2 Objetivo General

Desarrollar un sistema de bajo nivel para registrar, almacenar y procesar lecturas de múltiples tipos de sensores (temperatura, presión, vibración) de manera unificada, implementando conceptos avanzados de POO, estructuras de datos y comunicación con hardware embebido.

## 1.3 Objetivos Específicos

1. **Implementar jerarquía polimórfica** usando clases abstractas y herencia
2. **Desarrollar estructuras de datos** (listas enlazadas) sin uso de STL
3. **Aplicar programación genérica** mediante templates de C++
4. **Gestionar memoria manualmente** siguiendo la Regla de los Tres/Cinco
5. **Integrar hardware real** (Arduino) mediante comunicación serial
6. **Garantizar modularidad** y extensibilidad del sistema

## 1.4 Alcance del Proyecto

### Funcionalidades Implementadas:

✅ **Gestión Polimórfica de Sensores**
- Clase base abstracta `SensorBase`
- Clases derivadas: `SensorTemperatura` (float), `SensorPresion` (int)
- Métodos virtuales puros para procesamiento específico

✅ **Estructuras de Datos Genéricas**
- Lista enlazada simple implementada manualmente
- Uso de templates para independencia de tipos
- Gestión dinámica de nodos con punteros

✅ **Gestión de Memoria Explícita**
- Implementación de la Regla de los Tres:
  - Constructor de copia
  - Operador de asignación
  - Destructor
- Sin uso de STL (std::list, std::vector)
- Control total sobre asignación/liberación de memoria

✅ **Integración con Arduino**
- Lectura directa del puerto serial
- Procesamiento en tiempo real
- Detección automática de tipos de datos

✅ **Sistema de Compilación**
- CMake para portabilidad
- Compilación con g++ estándar C++11

### Limitaciones y Restricciones:

⚠️ **No se utiliza STL**: Implementación manual de todas las estructuras
⚠️ **Tipos limitados**: Solo temperatura (float) y presión (int)
⚠️ **Puerto serial**: Compatible con Linux/Mac (limitado en Windows)

## 1.5 Tecnologías Utilizadas

| Tecnología | Versión | Propósito |
|------------|---------|-----------|
| **C++** | C++11 | Lenguaje de programación principal |
| **g++** | 13.3.0+ | Compilador |
| **CMake** | 3.10+ | Sistema de construcción |
| **Arduino IDE** | 2.x | Programación del hardware |
| **Doxygen** | 1.9+ | Generación de documentación |
| **Linux/macOS** | - | Sistema operativo objetivo |

## 1.6 Metodología de Desarrollo

El proyecto siguió una metodología iterativa con las siguientes fases:

### Fase 1: Diseño de Arquitectura
- Definición de la jerarquía de clases
- Identificación de patrones de diseño
- Especificación de interfaces

### Fase 2: Implementación Base
- Desarrollo de clases fundamentales
- Implementación de estructuras de datos
- Pruebas unitarias básicas

### Fase 3: Integración con Hardware
- Desarrollo de comunicación serial
- Pruebas con Arduino físico
- Optimización de lectura en tiempo real

### Fase 4: Documentación y Entrega
- Generación de documentación con Doxygen
- Creación de manuales técnicos
- Pruebas de integración finales

---

# 2. MANUAL TÉCNICO

## 2.1 Diseño del Sistema

### 2.1.1 Arquitectura General

El sistema utiliza una arquitectura en capas con separación clara de responsabilidades:

```
┌─────────────────────────────────────────────────────────┐
│                    Capa de Aplicación                   │
│              (main.cpp - Menú Interactivo)              │
└─────────────────────────────────────────────────────────┘
                           ↓
┌─────────────────────────────────────────────────────────┐
│                 Capa de Gestión de Sensores             │
│    (ListaGeneral - ListaSensor<SensorBase*>)            │
└─────────────────────────────────────────────────────────┘
                           ↓
┌───────────────────────────┬─────────────────────────────┐
│   Capa de Sensores        │   Capa de Comunicación      │
│   - SensorBase (Abstract) │   - SerialPort              │
│   - SensorTemperatura     │   - Arduino Interface       │
│   - SensorPresion         │                             │
└───────────────────────────┴─────────────────────────────┘
                           ↓
┌─────────────────────────────────────────────────────────┐
│              Capa de Estructuras de Datos               │
│         (ListaSensor<T>, Nodo<T> - Templates)           │
└─────────────────────────────────────────────────────────┘
```

### 2.1.2 Patrones de Diseño Implementados

#### **Template Method Pattern**
- Clase: `SensorBase`
- Propósito: Define el esqueleto de operaciones dejando detalles a subclases
- Implementación: Métodos virtuales puros `procesarLectura()` e `imprimirInfo()`

#### **Strategy Pattern** (Implícito)
- Diferentes estrategias de procesamiento según el tipo de sensor
- `SensorTemperatura`: Calcula mínimo
- `SensorPresion`: Calcula promedio

#### **Factory Method** (Simplificado)
- Creación dinámica de sensores según tipo recibido del Arduino
- Permite extensión sin modificar código existente

### 2.1.3 Diagrama de Clases

```
                    ┌──────────────────┐
                    │   SensorBase     │
                    │   (Abstract)     │
                    ├──────────────────┤
                    │ - nombre[50]     │
                    ├──────────────────┤
                    │ + SensorBase()   │
                    │ + ~SensorBase()  │
                    │ + procesarLectura() = 0 │
                    │ + imprimirInfo() = 0    │
                    │ + getNombre()    │
                    └────────┬─────────┘
                             │
                ┌────────────┴────────────┐
                │                         │
       ┌────────▼─────────┐    ┌─────────▼────────┐
       │ SensorTemperatura│    │  SensorPresion   │
       ├──────────────────┤    ├──────────────────┤
       │ - historial:     │    │ - historial:     │
       │   ListaSensor    │    │   ListaSensor    │
       │   <float>*       │    │   <int>*         │
       ├──────────────────┤    ├──────────────────┤
       │ + procesarLectura()│  │ + procesarLectura()│
       │ + imprimirInfo() │    │ + imprimirInfo() │
       │ + agregarLectura()│   │ + agregarLectura()│
       └──────────────────┘    └──────────────────┘

┌────────────────────────────────────────┐
│        ListaSensor<T>                  │
│        (Template Class)                │
├────────────────────────────────────────┤
│ - cabeza: Nodo<T>*                     │
│ - tamanio: int                         │
├────────────────────────────────────────┤
│ + insertarAlFinal(T)                   │
│ + buscar(T): Nodo<T>*                  │
│ + iterar(Funcion)                      │
│ + limpiar()                            │
└────────────────────────────────────────┘
                    │
                    │ usa
                    ▼
         ┌──────────────────┐
         │    Nodo<T>       │
         │  (Template)      │
         ├──────────────────┤
         │ + dato: T        │
         │ + siguiente:     │
         │   Nodo<T>*       │
         └──────────────────┘
```

### 2.1.4 Flujo de Datos

```
Arduino → Puerto Serial → SerialPort.h → main.cpp
                                              ↓
                                    [Parseo de datos]
                                              ↓
                                    TIPO ID VALOR
                                    /           \
                               T (float)     P (int)
                                  ↓              ↓
                          SensorTemperatura  SensorPresion
                                  ↓              ↓
                          ListaSensor<float> ListaSensor<int>
                                  ↓              ↓
                          procesarLectura() (polimórfico)
                                  ↓
                             [Resultados]
```

## 2.2 Componentes

### 2.2.1 SensorBase.h - Clase Base Abstracta

**Propósito:** Define la interfaz común para todos los sensores.

**Características Clave:**
```cpp
class SensorBase {
protected:
    char nombre[50];  // No usa std::string - requisito del proyecto
    
public:
    virtual void procesarLectura() = 0;  // Virtual puro
    virtual void imprimirInfo() const = 0;  // Virtual puro
    virtual ~SensorBase();  // Destructor virtual
};
```

**Decisiones de Diseño:**
- ✅ Array de char en lugar de std::string (sin STL)
- ✅ Métodos virtuales puros para forzar implementación
- ✅ Destructor virtual para correcta liberación de memoria

### 2.2.2 SensorTemperatura.h - Sensor Concreto

**Propósito:** Implementa sensor para lecturas de temperatura (float).

**Características:**
```cpp
class SensorTemperatura : public SensorBase {
private:
    ListaSensor<float>* historial;
    
public:
    void agregarLectura(float valor);
    void procesarLectura() override;  // Calcula mínimo
};
```

**Lógica de Procesamiento:**
- Itera sobre todas las lecturas almacenadas
- Encuentra el valor mínimo registrado
- Útil para detectar temperaturas anormalmente bajas

### 2.2.3 SensorPresion.h - Sensor Concreto

**Propósito:** Implementa sensor para lecturas de presión (int).

**Características:**
```cpp
class SensorPresion : public SensorBase {
private:
    ListaSensor<int>* historial;
    
public:
    void agregarLectura(int valor);
    void procesarLectura() override;  // Calcula promedio
};
```

**Lógica de Procesamiento:**
- Suma todas las lecturas
- Calcula el promedio
- Útil para monitoreo de tendencias de presión

### 2.2.4 ListaSensor.h - Lista Enlazada Genérica

**Propósito:** Implementa una lista enlazada simple genérica sin STL.

**Características:**
```cpp
template <typename T>
class ListaSensor {
private:
    Nodo<T>* cabeza;
    int tamanio;
    
public:
    // Regla de los Tres
    ListaSensor();                              // Constructor
    ListaSensor(const ListaSensor& otra);       // Constructor de copia
    ListaSensor& operator=(const ListaSensor&); // Operador de asignación
    ~ListaSensor();                             // Destructor
    
    // Operaciones
    void insertarAlFinal(T dato);
    Nodo<T>* buscar(T dato) const;
    void limpiar();
};
```

**Gestión de Memoria:**
- ✅ Constructor de copia: Copia profunda de todos los nodos
- ✅ Operador de asignación: Evita auto-asignación
- ✅ Destructor: Libera todos los nodos recursivamente

### 2.2.5 SerialPort.h - Comunicación con Arduino

**Propósito:** Maneja la comunicación serial con Arduino.

**Características:**
```cpp
class SerialPort {
private:
    int fd;  // File descriptor del puerto
    
public:
    bool abrir(const std::string& puerto, int baudRate = 9600);
    bool leerLinea(std::string& linea);
    void cerrar();
};
```

**Configuración del Puerto:**
- Velocidad: 9600 bps
- Bits de datos: 8
- Paridad: Ninguna
- Bits de parada: 1
- Control de flujo: Ninguno

### 2.2.6 main.cpp - Aplicación Principal

**Propósito:** Punto de entrada del sistema con menú interactivo.

**Funcionalidades:**
1. Crear sensores manualmente
2. Registrar lecturas individuales
3. **Leer desde Arduino en tiempo real**
4. Procesar lecturas (polimórficamente)
5. Liberar memoria correctamente

**Función Principal - leerDesdeArduino():**
```cpp
void leerDesdeArduino(ListaGeneral* listaGestion) {
    // 1. Abrir puerto serial
    // 2. Leer líneas continuamente
    // 3. Parsear: TIPO ID VALOR
    // 4. Crear/actualizar sensores dinámicamente
    // 5. Mostrar tipo de dato (float/int)
}
```

## 2.3 Desarrollo

### 2.3.1 Compilación

#### Método 1: g++ directo (Recomendado)

```bash
# En el directorio del proyecto
g++ -std=c++11 -Wall -Wextra -o SistemaIoT main.cpp

# Ejecutar
./SistemaIoT
```

#### Método 2: CMake

```bash
mkdir build
cd build
cmake ..
make
./SistemaIoT
```

**Requisitos del Compilador:**
- g++ versión 4.8.1 o superior
- Soporte para C++11 mínimo
- POSIX-compliant (para SerialPort.h)

### 2.3.2 Estructura de Archivos

```
Act1_U2/
├── main.cpp                      # Programa principal
├── SensorBase.h                  # Clase abstracta base
├── SensorTemperatura.h           # Sensor de temperatura
├── SensorPresion.h               # Sensor de presión
├── ListaSensor.h                 # Lista enlazada genérica
├── Nodo.h                        # Estructura de nodo
├── SerialPort.h                  # Comunicación serial
├── CMakeLists.txt                # Configuración CMake
├── Doxyfile                      # Configuración Doxygen
├── arduino_sensor_simulator.ino  # Código Arduino
├── README.md                     # Documentación general
└── docs/                         # Documentación Doxygen
    └── html/
        └── index.html            # Página principal
```

### 2.3.3 Generación de Documentación

#### Paso 1: Instalar Doxygen

```bash
# Ubuntu/Debian
sudo apt-get install doxygen graphviz

# macOS
brew install doxygen graphviz
```

#### Paso 2: Generar Documentación

```bash
# En el directorio del proyecto
doxygen Doxyfile

# La documentación se genera en docs/html/
firefox docs/html/index.html  # Linux
open docs/html/index.html     # macOS
```

### 2.3.4 Uso con Arduino

#### Paso 1: Cargar Código en Arduino

1. Abrir `arduino_sensor_simulator.ino` en Arduino IDE
2. Conectar Arduino por USB
3. Seleccionar puerto: Herramientas → Puerto
4. Cargar código (botón →)

#### Paso 2: Identificar Puerto

```bash
# Linux
ls /dev/tty* | grep -E "USB|ACM"
# Salida: /dev/ttyUSB0 o /dev/ttyACM0

# Dar permisos
sudo chmod 666 /dev/ttyUSB0
```

#### Paso 3: Ejecutar Sistema

```bash
./SistemaIoT
# Seleccionar Opción 6
# Ingresar puerto: /dev/ttyUSB0
```

### 2.3.5 Formato de Datos Arduino

El Arduino debe enviar datos en el formato:

```
TIPO ID VALOR
```

**Ejemplos:**
```
T T-001 42.3    # Temperatura 42.3°C
P P-105 87      # Presión 87 Pa
```

**Código Arduino (extracto):**
```cpp
Serial.print("T ");
Serial.print(TEMP_ID);
Serial.print(" ");
Serial.println(temperatura, 1);
```

### 2.3.6 Pruebas Realizadas

#### Test 1: Creación de Sensores
```
✅ SensorTemperatura creado correctamente
✅ SensorPresion creado correctamente
✅ Cada sensor tiene su propia lista interna
```

#### Test 2: Inserción de Lecturas
```
✅ Float insertado en SensorTemperatura
✅ Int insertado en SensorPresion
✅ Nodos creados dinámicamente
```

#### Test 3: Procesamiento Polimórfico
```
✅ Iteración sobre lista heterogénea
✅ procesarLectura() ejecutado polimórficamente
✅ SensorTemperatura calcula mínimo
✅ SensorPresion calcula promedio
```

#### Test 4: Gestión de Memoria
```
✅ Destructores llamados en orden correcto
✅ No hay fugas de memoria (verificado con valgrind)
✅ Nodos liberados correctamente
```

#### Test 5: Comunicación Arduino
```
✅ Puerto serial abierto correctamente
✅ Datos recibidos en tiempo real
✅ Sensores creados automáticamente
✅ Tipo de dato detectado (float/int)
```

### 2.3.7 Requisitos Funcionales Cumplidos

| # | Requisito | Estado |
|---|-----------|--------|
| RF1 | Simulación mediante Arduino | ✅ |
| RF2 | Registro polimórfico | ✅ |
| RF3 | Inserción genérica (float/int) | ✅ |
| RF4 | Procesamiento polimórfico | ✅ |
| RF5 | Operaciones de lista | ✅ |
| RF6 | Uso de CMake | ✅ |

### 2.3.8 Requisitos No Funcionales Cumplidos

| # | Requisito | Estado |
|---|-----------|--------|
| RNF1 | Exclusividad de punteros (sin STL) | ✅ |
| RNF2 | POO Avanzado | ✅ |
| RNF3 | Regla de los Tres/Cinco | ✅ |

---

# 3. PANTALLAZOS DE IMPLEMENTACIÓN

## 3.1 Compilación Exitosa

```
$ g++ -std=c++11 -o SistemaIoT main.cpp
[Compilación exitosa sin errores]
```

## 3.2 Menú Principal

```
╔════════════════════════════════════════════════╗
║  SISTEMA DE GESTIÓN POLIMÓRFICA DE SENSORES   ║
║            PARA IoT (Genérico)                 ║
╚════════════════════════════════════════════════╝

==================================
Sistema IoT de Monitoreo
==================================
Opción 1: Crear Sensor (Tipo T: Temperatura)
Opción 2: Crear Sensor (Tipo P: Presión)
Opción 3: Registrar Lectura
Opción 4: Ejecutar Procesamiento
Opción 5: Cerrar Sistema (Liberar Memoria)
Opción 6: 🔌 Leer desde Arduino (Puerto Serial)
==================================
```

## 3.3 Lectura desde Arduino

```
Seleccione una opción: 6

╔════════════════════════════════════════════════╗
║         LECTURA DESDE ARDUINO REAL             ║
╚════════════════════════════════════════════════╝

Ingrese el puerto del Arduino: /dev/ttyUSB0

✓ Puerto /dev/ttyUSB0 abierto correctamente a 9600 bps
✓ Conectado exitosamente!
✓ Esperando datos del Arduino...

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

📡 Recibido: T T-001 42.3
✓ Sensor de Temperatura 'T-001' creado
  📊 Tipo de dato: float
  📈 Valor inicial: 42.3°C
📊 Total de lecturas recibidas: 1

📡 Recibido: P P-105 87
✓ Sensor de Presión 'P-105' creado
  📊 Tipo de dato: int
  📈 Valor inicial: 87 Pa
📊 Total de lecturas recibidas: 2
```

## 3.4 Procesamiento Polimórfico

```
Seleccione una opción: 4

--- Procesando Sensores ---

-> Procesando Sensor T-001...
[Sensor Temp] Lectura minima calculada: 42.1

-> Procesando Sensor P-105...
[Sensor Presion] Promedio calculado: 82.50

--- Procesamiento Completado ---
```

## 3.5 Liberación de Memoria

```
Seleccione una opción: 5

--- Liberación de Memoria en Curso ---
[Destructor General] Liberando Lista de Gestión...
[Destructor Sensor T-001]
[Destructor ListaSensor] Liberando lista...
[Log] Nodo<float> liberado
[Log] Nodo<float> liberado
[Destructor SensorBase] Liberando sensor: T-001
[Destructor Sensor P-105]
[Destructor ListaSensor] Liberando lista...
[Log] Nodo<int> liberado
[Log] Nodo<int> liberado
[Destructor SensorBase] Liberando sensor: P-105

Sistema Cerrado. Memoria Liberada.
```

---

# 4. CONCLUSIONES

## 4.1 Logros Obtenidos

### ✅ Técnicos

1. **Implementación completa de POO Avanzada**
   - Jerarquía de clases con polimorfismo
   - Métodos virtuales puros correctamente implementados
   - Destructores virtuales para gestión de memoria

2. **Programación Genérica Efectiva**
   - Templates aplicados en estructuras de datos
   - Reutilización de código sin sacrificar eficiencia
   - Independencia de tipos de datos

3. **Gestión Manual de Memoria**
   - Regla de los Tres implementada correctamente
   - Sin fugas de memoria (verificado)
   - Control total sobre asignación/liberación

4. **Integración Hardware Exitosa**
   - Comunicación serial funcional
   - Lectura en tiempo real
   - Detección automática de tipos

### ✅ Académicos

1. Comprensión profunda de:
   - Polimorfismo y clases abstractas
   - Templates y programación genérica
   - Gestión de memoria en C++
   - Estructuras de datos fundamentales

2. Aplicación práctica de:
   - Patrones de diseño
   - Buenas prácticas de programación
   - Documentación técnica profesional
   - Metodologías de desarrollo

## 4.2 Desafíos Superados

1. **Comunicación Serial**: Configuración correcta de POSIX termios
2. **Gestión de Memoria**: Implementación correcta de destructores encadenados
3. **Templates**: Manejo de compilación con tipos genéricos
4. **Polimorfismo**: Uso correcto de punteros a clase base

## 4.3 Áreas de Mejora

1. **Portabilidad**: SerialPort.h limitado a POSIX (Linux/Mac)
2. **Tipos de Sensores**: Actualmente solo temperatura y presión
3. **Validación**: Mejorar manejo de datos corruptos del Arduino
4. **Persistencia**: Agregar capacidad de guardar datos en archivo

## 4.4 Lecciones Aprendidas

1. La abstracción mediante clases base facilita la extensibilidad
2. Los templates permiten reutilización sin sacrificar rendimiento
3. La gestión manual de memoria requiere disciplina y cuidado
4. La documentación es tan importante como el código
5. Las pruebas exhaustivas previenen errores en producción

## 4.5 Aplicaciones Prácticas

Este sistema puede ser base para:

- **Monitoreo Industrial**: Sensores en plantas de manufactura
- **Sistemas Domóticos**: Automatización del hogar
- **Agricultura de Precisión**: Monitoreo de condiciones ambientales
- **Investigación Científica**: Recolección de datos experimentales
- **Educación**: Enseñanza de POO y sistemas embebidos

---

# 5. REFERENCIAS

## 5.1 Bibliografía

1. **Stroustrup, B.** (2013). *The C++ Programming Language* (4th ed.). Addison-Wesley.

2. **Meyers, S.** (2014). *Effective Modern C++*. O'Reilly Media.

3. **Gamma, E., Helm, R., Johnson, R., & Vlissides, J.** (1994). *Design Patterns: Elements of Reusable Object-Oriented Software*. Addison-Wesley.

4. **Josuttis, N.** (2012). *The C++ Standard Library: A Tutorial and Reference* (2nd ed.). Addison-Wesley.

## 5.2 Recursos en Línea

- **C++ Reference**: https://en.cppreference.com/
- **Arduino Documentation**: https://www.arduino.cc/reference/en/
- **Doxygen Manual**: https://www.doxygen.nl/manual/
- **CMake Tutorial**: https://cmake.org/cmake/help/latest/guide/tutorial/

## 5.3 Herramientas Utilizadas

- **g++ Compiler**: https://gcc.gnu.org/
- **Visual Studio Code**: https://code.visualstudio.com/
- **Git**: https://git-scm.com/
- **Valgrind** (para detección de fugas): http://valgrind.org/

---

## ANEXOS

### Anexo A: Código Fuente Completo
Ver archivos adjuntos en el proyecto.

### Anexo B: Documentación Doxygen
Disponible en `docs/html/index.html` después de ejecutar `doxygen Doxyfile`.

### Anexo C: Script de Compilación
```bash
#!/bin/bash
g++ -std=c++11 -Wall -Wextra -o SistemaIoT main.cpp
echo "Compilación completada. Ejecuta: ./SistemaIoT"
```

### Anexo D: Formato de Datos Arduino
```
TIPO ID VALOR
T    ID FLOAT
P    ID INT
```

---

**FIN DEL REPORTE**

---

*Este documento fue generado como parte del proyecto académico de Programación Orientada a Objetos Avanzada. Todo el código fuente está disponible en el directorio del proyecto.*
