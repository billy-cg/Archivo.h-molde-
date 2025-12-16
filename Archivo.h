// Archivo.h
#ifndef ARCHIVO_H
#define ARCHIVO_H

#include <cstdio>   // fopen, fread, fwrite, fclose, fseek, ftell
#include <string>

template <typename T>
class Archivo {
private:
    const char* ruta;   // ruta del archivo binario (ej: "Archivo.dat")

public:
    // Constructor: guardamos la ruta del archivo
    Archivo(const char* rutaArchivo) : ruta(rutaArchivo) {}

    // Guarda (append) un registro al final del archivo
    bool guardar(const T& reg) {
        FILE* p = fopen(ruta, "ab");
        if (!p) return false;
        bool ok = fwrite(&reg, sizeof(T), 1, p) == 1;
        fclose(p);
        return ok;
    }

    // Cuenta registros (tamaño archivo / sizeof(T))
    int contar() {
        FILE* p = fopen(ruta, "rb");
        if (!p) return 0;
        fseek(p, 0, SEEK_END);
        long bytes = ftell(p);
        fclose(p);
        return (bytes <= 0) ? 0 : static_cast<int>(bytes / sizeof(T));
    }

    // Lee el registro en la posicion 'pos' (0-index)
    // Retorna true si leyo correctamente y deja el resultado en 'reg'
    bool leer(int pos, T& reg) {
        FILE* p = fopen(ruta, "rb");
        if (!p) return false;
        if (fseek(p, static_cast<long>(pos) * sizeof(T), SEEK_SET) != 0) {
            fclose(p);
            return false;
        }
        bool ok = fread(&reg, sizeof(T), 1, p) == 1;
        fclose(p);
        return ok;
    }

    // Sobrescribe el registro en la posicion 'pos'
    bool modificar(int pos, const T& reg) {
        FILE* p = fopen(ruta, "rb+");
        if (!p) return false;
        if (fseek(p, static_cast<long>(pos) * sizeof(T), SEEK_SET) != 0) {
            fclose(p);
            return false;
        }
        bool ok = fwrite(&reg, sizeof(T), 1, p) == 1;
        fclose(p);
        return ok;
    }

    // Buscar por ID: requiere que T tenga metodo int getID() const;
    // Devuelve la posicion (>=0) o -1 si no encuentra / archivo inaccesible.
    int buscarPorID(int idBuscado) {
        FILE* p = fopen(ruta, "rb");
        if (!p) return -1;
        T aux;
        int pos = 0;
        while (fread(&aux, sizeof(T), 1, p) == 1) {
            if (aux.getID() == idBuscado) {
                fclose(p);
                return pos;
            }
            pos++;
        }
        fclose(p);
        return -1;
    }

    // Recorrer todos y aplicar una función que reciba (const T&)
    // Ej: arch.recorrer([](const Caballo& c){ c.mostrar(); });
    template <typename Func>
    void recorrer(Func f) {
        FILE* p = fopen(ruta, "rb");
        if (!p) return;
        T aux;
        while (fread(&aux, sizeof(T), 1, p) == 1) {
            f(aux);
        }
        fclose(p);
    }

    // Obtener ultimo ID. Si no hay registros devuelve 0.
    int obtenerUltimoID() {
        int cant = contar();
        if (cant == 0) return 0;
        T aux;
        if (!leer(cant - 1, aux)) return 0;
        return aux.getID();
    }
};

#endif // ARCHIVO_H
