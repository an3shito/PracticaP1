// En listas.h
#ifndef LISTA_H
#define LISTA_H
#include <iostream>
#include <string>

/**
 * @struct Clase
 * @brief Representa una clase académica
 * @startuml clase_structure
 * class Clase {
 *   - nombre : string
 *   - secciones : Seccion*
 *   - siguiente : Clase*
 * }
 * @enduml
 */
struct Estudiante {
    std::string nombre;
    float examen;
    float zona;
    Estudiante* siguiente;
};

/**
 * @struct Seccion
 * @brief Representa una sección dentro de una clase
 * @startuml seccion_structure
 * class Seccion {
 *   - nombre : string
 *   - anho : int
 *   - estudiantes : Estudiante*
 *   - siguiente : Seccion*
 * }
 * @enduml
 */
struct Seccion {
    std::string nombre;  // Ej: "Sección A"
    int anho;
    Estudiante* estudiantes;
    Seccion* siguiente;
};


/**
 * @struct Estudiante
 * @brief Representa un estudiante con sus notas
 * @startuml estudiante_structure
 * class Estudiante {
 *   - nombre : string
 *   - zona : float
 *   - examen : float
 *   - siguiente : Estudiante*
 * }
 * @enduml
 */
struct Clase {
    std::string nombre;  // Ej: "Matemáticas"
    Seccion* secciones;
    Clase* siguiente;
};

// Variables globales (como en la Fase I)
extern Clase* listaClases;

// Funciones básicas
void insertarEstudiante(Seccion* seccion, const std::string& nombre, float zona, float examen) {
    if (!seccion) return;
    
    Estudiante* nuevo = new Estudiante;
    nuevo->nombre = nombre;
    nuevo->zona = zona;
    nuevo->examen = examen;
    nuevo->siguiente = seccion->estudiantes;
    seccion->estudiantes = nuevo;
}

void insertarSeccion(Clase* clase, const std::string& nombreSeccion, int anho) {
    if (!clase) return;
    
    Seccion* nueva = new Seccion;
    nueva->nombre = nombreSeccion;
    nueva->anho = anho;
    nueva->estudiantes = nullptr;
    nueva->siguiente = clase->secciones;
    clase->secciones = nueva;
}

void insertarClase(const std::string& nombreClase) {
    Clase* nueva = new Clase;
    nueva->nombre = nombreClase;
    nueva->secciones = nullptr;
    nueva->siguiente = listaClases;
    listaClases = nueva;
}

bool ingresarAlumnoManual(const std::string& clase, 
                         const std::string& seccion,
                         int anho,
                         const std::string& nombre,
                         float zona,
                         float examen) {
    // Buscar la clase especificada
    Clase* claseActual = listaClases;
    while (claseActual != nullptr) {
        if (claseActual->nombre == clase) {
            break;
        }
        claseActual = claseActual->siguiente;
    }
    
    // Si la clase no existe, crearla
    if (claseActual == nullptr) {
        insertarClase(clase);
        claseActual = listaClases; // Apunta a la nueva clase creada
    }
    
    // Buscar la sección especificada
    Seccion* seccionActual = claseActual->secciones;
    while (seccionActual != nullptr) {
        if (seccionActual->nombre == seccion && seccionActual->anho == anho) {
            break;
        }
        seccionActual = seccionActual->siguiente;
    }
    
    // Si la sección no existe, crearla
    if (seccionActual == nullptr) {
        insertarSeccion(claseActual, seccion, anho);
        seccionActual = claseActual->secciones; // Apunta a la nueva sección creada
    }
    
    // Insertar el estudiante
    insertarEstudiante(seccionActual, nombre, zona, examen);
    
    return true;
}

// Funciones de búsqueda y eliminación
Estudiante* buscarEstudiante(const std::string& nombre);
void eliminarEstudiante(Seccion* seccion, const std::string& nombre);

// Funciones de archivos

void generarReporte();
void generarDiagrama();

#endif