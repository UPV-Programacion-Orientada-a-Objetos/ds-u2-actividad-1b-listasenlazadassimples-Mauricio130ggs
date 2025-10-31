#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <string>
#include <fstream>
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <cstring>

/**
 * Clase para leer datos del puerto serial (Arduino)
 * Compatible con Linux/Mac
 */
class SerialPort {
private:
    int fd;  // File descriptor del puerto
    bool isOpen;
    
public:
    SerialPort() : fd(-1), isOpen(false) {}
    
    /**
     * Abre el puerto serial
     * @param puerto Ruta del puerto (ej: /dev/ttyACM0)
     * @param baudRate Velocidad (por defecto 9600)
     */
    bool abrir(const std::string& puerto, int baudRate = 9600) {
        fd = open(puerto.c_str(), O_RDONLY | O_NOCTTY);
        
        if (fd < 0) {
            std::cerr << "Error: No se pudo abrir el puerto " << puerto << std::endl;
            std::cerr << "Verifica que:" << std::endl;
            std::cerr << "  1. El Arduino esté conectado" << std::endl;
            std::cerr << "  2. Tengas permisos (sudo chmod 666 " << puerto << ")" << std::endl;
            return false;
        }
        
        // Configurar puerto serial
        struct termios options;
        tcgetattr(fd, &options);
        
        // Configurar velocidad
        speed_t baud;
        switch(baudRate) {
            case 9600:  baud = B9600;  break;
            case 19200: baud = B19200; break;
            case 38400: baud = B38400; break;
            case 57600: baud = B57600; break;
            case 115200: baud = B115200; break;
            default:    baud = B9600;  break;
        }
        
        cfsetispeed(&options, baud);
        cfsetospeed(&options, baud);
        
        // Configurar formato: 8N1
        options.c_cflag &= ~PARENB;  // Sin paridad
        options.c_cflag &= ~CSTOPB;  // 1 bit de parada
        options.c_cflag &= ~CSIZE;
        options.c_cflag |= CS8;      // 8 bits de datos
        
        options.c_cflag |= (CLOCAL | CREAD);  // Habilitar receptor
        
        // Modo raw
        options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
        options.c_iflag &= ~(IXON | IXOFF | IXANY);
        options.c_oflag &= ~OPOST;
        
        // Aplicar configuración
        tcsetattr(fd, TCSANOW, &options);
        
        // Limpiar buffers
        tcflush(fd, TCIOFLUSH);
        
        isOpen = true;
        std::cout << "✓ Puerto " << puerto << " abierto correctamente a " 
                  << baudRate << " bps" << std::endl;
        
        // Esperar un momento para que Arduino se estabilice
        sleep(2);
        
        return true;
    }
    
    /**
     * Lee una línea del puerto serial
     * @param linea String donde se guardará la línea leída
     * @return true si se leyó correctamente, false si hay error
     */
    bool leerLinea(std::string& linea) {
        if (!isOpen || fd < 0) {
            return false;
        }
        
        linea.clear();
        char c;
        ssize_t bytesRead;
        
        while (true) {
            bytesRead = read(fd, &c, 1);
            
            if (bytesRead < 0) {
                std::cerr << "Error al leer del puerto serial" << std::endl;
                return false;
            }
            
            if (bytesRead == 0) {
                // No hay datos disponibles, continuar esperando
                usleep(10000);  // Esperar 10ms
                continue;
            }
            
            // Si es fin de línea, retornar
            if (c == '\n' || c == '\r') {
                if (!linea.empty()) {
                    return true;
                }
                continue;
            }
            
            // Agregar carácter a la línea
            linea += c;
        }
    }
    
    /**
     * Verifica si el puerto está abierto
     */
    bool estaAbierto() const {
        return isOpen;
    }
    
    /**
     * Cierra el puerto serial
     */
    void cerrar() {
        if (isOpen && fd >= 0) {
            close(fd);
            fd = -1;
            isOpen = false;
            std::cout << "Puerto serial cerrado" << std::endl;
        }
    }
    
    /**
     * Destructor
     */
    ~SerialPort() {
        cerrar();
    }
};

#endif // SERIALPORT_H
