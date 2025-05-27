# WynPlan

  

**WynPlan** es un sistema para gestionar la asignación de horarios desarrollado con **C++**, **Qt 6**, y la biblioteca externa **DeSiGNAR**.

Este proyecto está configurado con CMake y no requiere Qt Creator para compilarse.

  

## Requisitos del Sistema

  

- Sistema operativo Linux (probado en Ubuntu/Debian)

- CMake (versión 3.16 o superior)

- Compilador C++ compatible con C++17

- Qt 6 (con los módulos base y widgets)

  

Antes de compilar el proyecto, asegúrese de tener instaladas las siguientes dependencias:

  
```
sudo apt install -y git cmake build-essential libgl1-mesa-dev

sudo apt install -y qt6-base-dev qt6-tools-dev qt6-declarative-dev qt6-quick3d-dev
```
  

## Clonar y Compilar

```
git clone https://github.com/GleymarP/WynPlan.git
```
```
cd WynPlan
```
  
```
cmake -B build
```
```
cmake --build build
```
  #### Ejecuta:
```
./WynPlan
```

  
WynPlan depende de la biblioteca externa DeSiGNAR. Si la biblioteca no ha sido compilada, CMake la compilará automáticamente durante la configuración del proyecto. Para más información sobre DeSiGNAR: [Repositorio de DeSiGNAR](https://github.com/R3mmurd/DeSiGNAR/tree/v2.0.1?tab=License-1-ov-file)

  

## Estructura del Proyecto

  

WynPlan/

- resources.qrc # Archivo de recursos Qt

- CMakeLists.txt # Script de compilación principal

|Directorio                |Descripción                        |
|----------------|-------------------------------|
|include|`Archivos de cabecera`            |
|src          |`Archivos de código fuente`            |
| ui          |`Archivos de interfaz Qt Designer`|
|resources       |`Archivos JSON y recursos`|
