#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <limits>
#include <sstream>

using namespace std;

// Estructura para representar un alumno
struct Alumno {
    string nombre;
    
    Alumno(string n) : nombre(n) {}
};

// Estructura para representar las notas de zona
struct Zona {
    double nota;
    
    Zona(double n) : nota(n) {}
};

// Estructura para representar las notas de examen final
struct ExamenFinal {
    double nota;
    
    ExamenFinal(double n) : nota(n) {}
};

// Estructura combinada para el reporte
struct RegistroCompleto {
    string nombre;
    double nota_zona;
    double nota_examen;
    double promedio;
    
    RegistroCompleto(string n, double z, double e) : 
        nombre(n), nota_zona(z), nota_examen(e) {
        promedio = (nota_zona + nota_examen) / 2.0;
    }
};

void generarReporteHTML(const vector<RegistroCompleto>& registros, 
                       const RegistroCompleto& mejor_tarea, 
                       const RegistroCompleto& mejor_examen, 
                       const RegistroCompleto& peor_alumno) {
    ofstream html("reporte_notas.html");
    
    html << "<!DOCTYPE html>\n";
    html << "<html>\n";
    html << "<head>\n";
    html << "    <title>Reporte de Notas de Alumnos</title>\n";
    html << "    <style>\n";
    html << "        body { font-family: Arial, sans-serif; margin: 20px; }\n";
    html << "        h1 { color: #2c3e50; }\n";
    html << "        h2 { color: #3498db; }\n";
    html << "        table { border-collapse: collapse; width: 100%; margin-bottom: 20px; }\n";
    html << "        th, td { border: 1px solid #ddd; padding: 8px; text-align: left; }\n";
    html << "        th { background-color: #f2f2f2; }\n";
    html << "        tr:nth-child(even) { background-color: #f9f9f9; }\n";
    html << "        .mejor { background-color: #d4edda; }\n";
    html << "        .peor { background-color: #f8d7da; }\n";
    html << "        .resumen { background-color: #e2e3e5; padding: 15px; border-radius: 5px; margin-bottom: 20px; }\n";
    html << "    </style>\n";
    html << "</head>\n";
    html << "<body>\n";
    html << "    <h1>Reporte de Notas de Alumnos</h1>\n";
    html << "    \n";
    html << "    <div class=\"resumen\">\n";
    html << "        <h2>Resumen</h2>\n";
    html << "        <p><strong>Mejor nota de tarea:</strong> " << mejor_tarea.nombre << " (Nota Zona: " << mejor_tarea.nota_zona << ")</p>\n";
    html << "        <p><strong>Mejor nota de examen:</strong> " << mejor_examen.nombre << " (Nota Examen: " << mejor_examen.nota_examen << ")</p>\n";
    html << "        <p><strong>Peor promedio:</strong> " << peor_alumno.nombre << " (Promedio: " << fixed << setprecision(2) << peor_alumno.promedio << ")</p>\n";
    html << "    </div>\n";
    html << "    \n";
    html << "    <h2>Detalle de Notas</h2>\n";
    html << "    <table>\n";
    html << "        <tr>\n";
    html << "            <th>Nombre</th>\n";
    html << "            <th>Nota Zona</th>\n";
    html << "            <th>Nota Examen</th>\n";
    html << "            <th>Promedio</th>\n";
    html << "        </tr>\n";

    for (const auto& reg : registros) {
        string clase = "";
        if (reg.nombre == mejor_tarea.nombre && reg.nota_zona == mejor_tarea.nota_zona) {
            clase = "mejor";
        } else if (reg.nombre == peor_alumno.nombre && reg.promedio == peor_alumno.promedio) {
            clase = "peor";
        }
        
        html << "        <tr class=\"" << clase << "\">\n";
        html << "            <td>" << reg.nombre << "</td>\n";
        html << "            <td>" << reg.nota_zona << "</td>\n";
        html << "            <td>" << reg.nota_examen << "</td>\n";
        html << "            <td>" << fixed << setprecision(2) << reg.promedio << "</td>\n";
        html << "        </tr>\n";
    }

    html << "    </table>\n";
    html << "</body>\n";
    html << "</html>\n";

    cout << "Reporte HTML generado como 'reporte_notas.html'\n";
}

template<typename T, typename F>
vector<T> leerArchivo(const string& nombre_archivo, F conversor) {
    vector<T> datos;
    ifstream archivo(nombre_archivo);
    
    if (!archivo.is_open()) {
        cerr << "Error al abrir el archivo: " << nombre_archivo << endl;
        exit(1);
    }

    string linea;
    while (getline(archivo, linea)) {
        // Eliminar espacios en blanco
        linea.erase(0, linea.find_first_not_of(" \t\n\r\f\v"));
        linea.erase(linea.find_last_not_of(" \t\n\r\f\v") + 1);
        
        if (!linea.empty()) {
            try {
                datos.push_back(conversor(linea));
            } catch (const exception& e) {
                cerr << "Error en archivo " << nombre_archivo << ": " << e.what() << endl;
                exit(1);
            }
        }
    }

    archivo.close();
    return datos;
}

int main() {
    cout << "Procesando archivos de notas..." << endl;
    
    // Leer los archivos usando estructuras específicas
    vector<Alumno> alumnos = leerArchivo<Alumno>("C:/Users/anddr/OneDrive/Escritorio/Libros C++/alumnos.txt", 
        [](string linea) { return Alumno(linea); });
    
    vector<Zona> zonas = leerArchivo<Zona>("C:/Users/anddr/OneDrive/Escritorio/Libros C++/notas_zonas.txt", 
        [](string linea) { 
            double nota = stod(linea);
            if (nota < 0 || nota > 100) throw out_of_range("Nota fuera de rango 0-100");
            return Zona(nota); 
        });
    
    vector<ExamenFinal> examenes = leerArchivo<ExamenFinal>("C:/Users/anddr/OneDrive/Escritorio/Libros C++/examen_final.txt", 
        [](string linea) { 
            double nota = stod(linea);
            if (nota < 0 || nota > 100) throw out_of_range("Nota fuera de rango 0-100");
            return ExamenFinal(nota); 
        });

    // Verificar consistencia en la cantidad de registros
    if (alumnos.size() != zonas.size() || alumnos.size() != examenes.size()) {
        cerr << "Error: Los archivos no tienen la misma cantidad de registros" << endl;
        cerr << "Alumnos: " << alumnos.size() << ", Zonas: " << zonas.size() 
             << ", Examenes: " << examenes.size() << endl;
        return 1;
    }

    // Combinar los datos en registros completos
    vector<RegistroCompleto> registros;
    for (size_t i = 0; i < alumnos.size(); ++i) {
        registros.emplace_back(
            alumnos[i].nombre,
            zonas[i].nota,
            examenes[i].nota
        );
    }

    // Encontrar el mejor en tareas (nota_zona más alta)
    auto mejor_tarea = *max_element(registros.begin(), registros.end(),
        [](const RegistroCompleto& a, const RegistroCompleto& b) {
            return a.nota_zona < b.nota_zona;
        });

    // Encontrar el mejor en examen (nota_examen más alta)
    auto mejor_examen = *max_element(registros.begin(), registros.end(),
        [](const RegistroCompleto& a, const RegistroCompleto& b) {
            return a.nota_examen < b.nota_examen;
        });

    // Encontrar el peor alumno (promedio más bajo)
    auto peor_alumno = *min_element(registros.begin(), registros.end(),
        [](const RegistroCompleto& a, const RegistroCompleto& b) {
            return a.promedio < b.promedio;
        });

    // Mostrar resultados en consola
    cout << "\n=== RESULTADOS ===" << endl;
    cout << "1. Mejor nota de tarea: " << mejor_tarea.nombre 
         << " (Zona: " << mejor_tarea.nota_zona 
         << ", Examen: " << mejor_tarea.nota_examen << ")" << endl;
    cout << "2. Mejor nota de examen: " << mejor_examen.nombre 
         << " (Zona: " << mejor_examen.nota_zona 
         << ", Examen: " << mejor_examen.nota_examen << ")" << endl;
    cout << "3. Peor alumno: " << peor_alumno.nombre 
         << " (Promedio: " << fixed << setprecision(2) << peor_alumno.promedio << ")" << endl;

    // Generar reporte HTML
    generarReporteHTML(registros, mejor_tarea, mejor_examen, peor_alumno);

    return 0;
}