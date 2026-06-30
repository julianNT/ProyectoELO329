# HMI de Buceo — Monitor de Señales en Tiempo Real

## Contexto

En operaciones de buceo, monitorear presión y temperatura en tiempo real es crítico para la seguridad del buzo. Este proyecto —correspondiente al curso **ELO329**— aborda la necesidad de una interfaz hombre-máquina (HMI) que procese, visualice y alerte sobre estos sensores de forma clara e inmediata.

## Solución Propuesta

Se implementa una **HMI de escritorio en C++ con Qt 6.4+** que:

- **Simula** datos de sensores de presión y temperatura con variación sinusoidal más ruido.
- **Visualiza** ambas señales en gráficos de línea con ventana deslizante de 100 muestras.
- **Evalúa** umbrales configurables (normal, advertencia, crítico) y despliega alarmas con código de colores.
- **Separa** la lógica de negocio (_back/_) de la presentación (_front/_), comunicándose mediante señales/slots de Qt.

La configuración de sensores, rangos y alarmas se realiza mediante un archivo de texto plano (`config.txt`), permitiendo adaptar el sistema sin recompilar.

## Estructura del Proyecto

```
ProyectoELO329/
├── CMakeLists.txt          # Build system (CMake 3.19+, Qt 6.4+)
├── config.txt              # Configuración de sensores y alarmas
├── main.cpp                # Punto de entrada
├── back/
│   ├── include/            # Interfaces del backend
│   └── src/                # Implementación: ConfigReader, DataSimulator, AlarmMonitor
└── front/
    ├── include/            # Interfaces del frontend
    └── src/                # Implementación: MainWindow, DiverWidget
```

## Construcción

```bash
cmake -B build && cmake --build build
./build/HMI_Buceo
```

## Requisitos

- CMake ≥ 3.19
- Qt ≥ 6.4 (Core, Widgets, Charts)