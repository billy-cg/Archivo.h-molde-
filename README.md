Archivo genérico en C++ (header-only)

Este proyecto surge a partir de un trabajo práctico de la facultad, donde se necesitaba una forma simple y reutilizable de manejar archivos binarios en C++ sin duplicar código para cada entidad.
A partir de esa necesidad se desarrolló una clase template genérica para guardar, leer, modificar y buscar registros en archivos binarios.

Archivo<T> es una clase template header-only que permite manejar archivos binarios de cualquier tipo T, siempre que cumpla ciertos requisitos.
Está pensada para proyectos académicos o aplicaciones simples de persistencia de datos.

El tipo T debe cumplir:

Constructor por defecto T()

Para búsquedas por ID: int getID() const

Opcional: método mostrar() const

Ser trivialmente copiable para usar fwrite / fread

Recomendado:

Usar tipos simples (int, float, char nombre[40])

Evitar std::string, punteros, contenedores STL y métodos virtuales


USO BASICO



#include "Archivo.h"
Archivo<MiClase> arch("datos.dat");

arch.guardar(obj);
arch.contar();
arch.leer(pos, obj);
arch.modificar(pos, obj);
arch.buscarPorID(id);
arch.recorrer([](const MiClase& r){ r.mostrar(); });


EJEMPLO DE TIPO COMPATIBLE

struct Cliente {
    int id;
    char nombre[40];
    Cliente() : id(0) {}
    int getID() const { return id; }
    void mostrar() const {
        cout << id << " - " << nombre << endl;
    }
};



