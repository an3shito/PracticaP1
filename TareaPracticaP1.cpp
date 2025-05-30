#include <fstream>
#include <iomanip>
#include "Listas.h"

using namespace std;


Clase* listaClases = nullptr;

bool cargarArchivos(Clase* clase, Seccion* seccion, const std::string& alumnosFile, const std::string& zonasFile, const std::string& examenesFile) {
    std::ifstream alumnos(alumnosFile);
    std::ifstream zonas(zonasFile);
    std::ifstream examenes(examenesFile);
    
    if (!alumnos.is_open() || !zonas.is_open() || !examenes.is_open()) {
        std::cerr << "Error al abrir los archivos. Verifique que existan:\n";
        std::cerr << "1. " << alumnosFile << "\n";
        std::cerr << "2. " << zonasFile << "\n";
        std::cerr << "3. " << examenesFile << "\n";
        return false; // Indicar fallo
    }
    
    std::string nombre;
    float zona, examen;
    
    while (std::getline(alumnos, nombre)) {
        if (!(zonas >> zona) || !(examenes >> examen)) {
            std::cerr << "Error leyendo notas para: " << nombre << "\n";
            break;
        }
        zonas.ignore();
        examenes.ignore();
        
        insertarEstudiante(seccion, nombre, zona, examen);
    }
    
    alumnos.close();
    zonas.close();
    examenes.close();
    
    return true; // Indicar éxito
}

void generarReporte() {
    float mejorZona = 0, mejorExamen = 0, peorPromedio = 100;
    Estudiante* mejorZonaEst = nullptr;
    Estudiante* mejorExamenEst = nullptr;
    Estudiante* peorPromEst = nullptr;
    
    for (Clase* c = listaClases; c; c = c->siguiente) {
        for (Seccion* s = c->secciones; s; s = s->siguiente) {
            for (Estudiante* e = s->estudiantes; e; e = e->siguiente) {
                float promedio = (e->zona + e->examen) / 2.0;
                
                if (e->zona > mejorZona) {
                    mejorZona = e->zona;
                    mejorZonaEst = e;
                }
                
                if (e->examen > mejorExamen) {
                    mejorExamen = e->examen;
                    mejorExamenEst = e;
                }
                
                if (promedio < peorPromedio) {
                    peorPromedio = promedio;
                    peorPromEst = e;
                }
            }
        }
    }
    
    std::cout << "\n=== REPORTE ===" << std::endl;
    if (mejorZonaEst) {
        std::cout << "Mejor nota de zona: " << mejorZonaEst->nombre << " (" << mejorZonaEst->zona << ")" << std::endl;
    }
    if (mejorExamenEst) {
        std::cout << "Mejor nota de examen: " << mejorExamenEst->nombre << " (" << mejorExamenEst->examen << ")" << std::endl;
    }
    if (peorPromEst) {
        std::cout << "Peor promedio: " << peorPromEst->nombre << " (" << peorPromedio << ")" << std::endl;
    }

}

void generarReporteHTML() {
    std::ofstream html("reporte.html");
    
    if (!html.is_open()) {
        std::cerr << "Error al crear archivo HTML\n";
        return;
    }
    
    html << "<!DOCTYPE html>\n<html>\n<head>\n";
    html << "<title>Reporte Académico</title>\n";
    html << "<style>\n";
    html << "  body { font-family: Arial, sans-serif; }\n";
    html << "  h1 { color: #2c3e50; }\n";
    html << "  table { width: 100%; border-collapse: collapse; }\n";
    html << "  th, td { border: 1px solid #ddd; padding: 8px; }\n";
    html << "  th { background-color: #f2f2f2; text-align: left; }\n";
    html << "  tr:nth-child(even) { background-color: #f9f9f9; }\n";
    html << "</style>\n</head>\n<body>\n";
    html << "<h1>Reporte Académico</h1>\n";
    
    for (Clase* c = listaClases; c != nullptr; c = c->siguiente) {
        html << "<h2>Clase: " << c->nombre << "</h2>\n";
        
        for (Seccion* s = c->secciones; s != nullptr; s = s->siguiente) {
            html << "<h3>Sección: " << s->nombre << " (Año: " << s->anho << ")</h3>\n";
            html << "<table>\n";
            html << "<tr><th>Nombre</th><th>Zona</th><th>Examen</th><th>Promedio</th></tr>\n";
            
            for (Estudiante* e = s->estudiantes; e != nullptr; e = e->siguiente) {
                float promedio = (e->zona + e->examen) / 2.0;
                html << "<tr>"
                     << "<td>" << e->nombre << "</td>"
                     << "<td>" << e->zona << "</td>"
                     << "<td>" << e->examen << "</td>"
                     << "<td>" << std::fixed << std::setprecision(2) << promedio << "</td>"
                     << "</tr>\n";
            }
            
            html << "</table>\n";
        }
    }
    
    html << "</body>\n</html>\n";
    html.close();
    
    std::cout << "Reporte HTML generado: reporte.html\n";
}

Estudiante* buscarEstudiante(const std::string& nombre) {
    for (Clase* c = listaClases; c != nullptr; c = c->siguiente) {
        for (Seccion* s = c->secciones; s != nullptr; s = s->siguiente) {
            for (Estudiante* e = s->estudiantes; e != nullptr; e = e->siguiente) {
                if (e->nombre == nombre) {
                    std::cout << "Estudiante encontrado en:\n";
                    std::cout << "Clase: " << c->nombre << "\n";
                    std::cout << "Sección: " << s->nombre << "\n";
                    std::cout << "Nota zona: " << e->zona << "\n";
                    std::cout << "Nota examen: " << e->examen << "\n";
                    return e;
                }
            }
        }
    }
    std::cout << "Estudiante no encontrado.\n";
    return nullptr;
}

// Función para eliminar un estudiante por nombre
bool eliminarEstudiante(const std::string& nombre) {
    for (Clase* c = listaClases; c != nullptr; c = c->siguiente) {
        for (Seccion* s = c->secciones; s != nullptr; s = s->siguiente) {
            Estudiante* actual = s->estudiantes;
            Estudiante* anterior = nullptr;
            
            while (actual != nullptr) {
                if (actual->nombre == nombre) {
                    // Caso 1: Es el primer elemento de la lista
                    if (anterior == nullptr) {
                        s->estudiantes = actual->siguiente;
                    } 
                    // Caso 2: Está en medio o al final
                    else {
                        anterior->siguiente = actual->siguiente;
                    }
                    
                    delete actual;
                    std::cout << "Estudiante " << nombre << " eliminado correctamente.\n";
                    return true;
                }
                
                anterior = actual;
                actual = actual->siguiente;
            }
        }
    }
    
    std::cout << "No se encontró al estudiante " << nombre << " para eliminar.\n";
    return false;
}

void generarDiagrama() {
    std::ofstream dot("diagrama.dot");
    dot << "digraph G {\n";
    dot << "  rankdir=LR;\n";
    dot << "  node [shape=record];\n";
    
    for (Clase* c = listaClases; c; c = c->siguiente) {
        dot << "  \"" << c->nombre << "\" [label=\"Clase: " << c->nombre << "\"];\n";
        
        for (Seccion* s = c->secciones; s; s = s->siguiente) {
            dot << "  \"" << s->nombre << "_" << c->nombre << "\" [label=\"Sección: " 
                << s->nombre << "\\nAño: " << s->anho << "\"];\n";
            dot << "  \"" << c->nombre << "\" -> \"" << s->nombre << "_" << c->nombre << "\";\n";
            
            for (Estudiante* e = s->estudiantes; e; e = e->siguiente) {
                dot << "  \"" << e->nombre << "_" << s->nombre << "\" [label=\"" << e->nombre  << "\\nExamen: " << e->examen 
                    << "\\nZona: " << e->zona << "\"];\n";
                dot << "  \"" << s->nombre << "_" << c->nombre << "\" -> \"" 
                    << e->nombre << "_" << s->nombre << "\";\n";
            }
        }
    }
    
    dot << "}\n";
    dot.close();
    
    system("dot -Tpng diagrama.dot -o diagrama.png");
}

int main() {

  std::string basePath = "C:/Users/anddr/OneDrive/Escritorio/PracticaP1/";

    // Clase 1
    insertarClase("Programación I");
    Clase* prog1 = listaClases;
    insertarSeccion(prog1, "A", 2024);
    cargarArchivos(prog1, prog1->secciones, 
                  basePath + "alumnos.txt",
                  basePath + "notas_zonas.txt",
                  basePath + "examen_final.txt");

    // Clase 2
    insertarClase("Programación II");
    Clase* prog2 = listaClases;
    insertarSeccion(prog2, "B", 2025);
    cargarArchivos(prog2, prog2->secciones,
                  basePath + "alumnosB.txt",
                  basePath + "notas_zonasB.txt",
                  basePath + "examen_finalB.txt");

    // Verificar estructura
    std::cout << "\nEstructura completa:\n";
    for (Clase* c = listaClases; c; c = c->siguiente) {
        std::cout << "Clase: " << c->nombre << "\n";
        for (Seccion* s = c->secciones; s; s = s->siguiente) {
            std::cout << "  Sección " << s->nombre << " (" << s->anho << "):\n";
            for (Estudiante* e = s->estudiantes; e; e = e->siguiente) {
                std::cout << "    - " << e->nombre << " (Zona: " << e->zona 
                          << ", Examen: " << e->examen << ")\n";
            }
        }
    }

    // Generar reporte HTML
    generarReporteHTML();
    generarDiagrama();
    std::cout << "Convirtiendo a imagen...\n";
    system("dot -Tpng diagrama.dot -o diagrama.png");
    
    std::cout << "Imagen generada: diagrama.png\n";
    //Ejemplo de uso de los nuevos métodos
    /* std::cout << "\n=== Prueba de búsqueda ===" << std::endl;
    Estudiante* encontrado = buscarEstudiante("Juan Pérez");
    if (encontrado) {
        std::cout << "Datos completos: " << encontrado->nombre 
         << ", Zona: " << encontrado->zona 
        << ", Examen: " << encontrado->examen << std::endl;
    }
     */
    /* std::cout << "\n=== Prueba de eliminación ===" << std::endl;
    if (eliminarEstudiante("Juan Pérez")) {
        std::cout << "Verificando eliminación..." << std::endl;
        buscarEstudiante("Juan Pérez"); // Debería mostrar que no existe
    } */
    

    // Ingresar un alumno manualmente
    /* if (ingresarAlumnoManual("Programación I", "A", 2024, 
                            "Juan Pérez", 85.5, 90.0)) {
        std::cout << "Alumno ingresado correctamente.\n";
    } else {
        std::cout << "Error al ingresar alumno.\n";
    }
    
    // Verificar que se ingresó
    Estudiante* encontrado = buscarEstudiante("Juan Pérez");
    if (encontrado) {
        std::cout << "Alumno encontrado: " << encontrado->nombre 
                  << " con promedio: " 
                  << (encontrado->zona + encontrado->examen)/2.0 << "\n";
    }
    */
    return 0; 
    
}