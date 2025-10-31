# Sistema de Gestión Polimórfica de Sensores para IoT

## Contenido de la Entrega

1. **codigo/** - Código fuente con comentarios Doxygen
2. **Doxyfile** - Configuración para generar documentación
3. **docs/** - Documentación Doxygen (generada)
4. **REPORTE_TECNICO.md** - Reporte completo
5. **pantallazos/** - Imágenes de la implementación

## Compilar y Ejecutar

```bash
cd codigo
g++ -std=c++11 -o SistemaIoT main.cpp
./SistemaIoT
```

## Generar Documentación Doxygen

```bash
doxygen Doxyfile
# La documentación se genera en: docs/html/index.html
```


