Archivo genérico en C++ (header-only)

Archivos:
- Archivo.h : clase template Archivo<T>

Requisitos para T:
- Constructor por defecto (T()).
- Método int getID() const;  // requerido solo si usás buscarPorID() u obtenerUltimoID()
- Opcional: método mostrar() const para imprimirlo (útil en recorrer()).
- Debe ser "serializable" por memcpy: la estrategia usa fwrite/fread de sizeof(T).
  => Preferible que T use tipos simples y arreglos char fijos para strings.

Uso:
- #include "Archivo.h"
- Archivo<MiClase> arch("miarchivo.dat");
- arch.guardar(obj);
- arch.contar();
- arch.leer(pos, obj);
- arch.modificar(pos, obj);
- arch.buscarPorID(id);
- arch.recorrer([](const MiClase& r){ r.mostrar(); });

Notas:
- Template: la implementación está en el header (necesario para templates).
- Si tu clase contiene punteros, std::string u objetos no triviales, NO uses fwrite/fread directo;
  en ese caso implementá serialización/deserialización manual o usa archivos texto/JSON.


¿Por qué template?
template <typename T> permite escribir una sola clase (Archivo) que funcione con cualquier tipo T (materiales, Trabajo, Cliente...). El compilador genera el código específico para cada T que uses. Evitás duplicar funciones por cada tipo.

¿Por qué la implementación va en el header?
Porque los templates se instancian en tiempo de compilación. Si pones la implementación en un .cpp separado el compilador no la verá cuando haga Archivo<Cliente> en otro archivo. Por eso los templates suelen ser header-only.

¿Por qué const char* ruta; y no std::string?

const char* ruta es simple y directo: guarda la ruta que pasás al constructor (ej: "cliente.dat").

Podés cambiarlo a std::string ruta; si preferís — funciona igual. Usé const char* por simplicidad y compatibilidad con fopen.
Ejemplo con std::string: Archivo(const std::string& r): ruta(r.c_str()) { ... } — pero cuidado con la vida útil del c_str(). Mejor usar std::string ruta; como campo si preferís.

Requisitos del tipo T (importante):

Debe ser trivialmente copiables para que fwrite(&obj, sizeof(T), 1, p) sea seguro. Eso significa:

Evitar miembros std::string, punteros a memoria dinámica, contenedores STL, vtables (clases con métodos virtuales).

Preferir int, float, arrays char nombre[40], etc.

Debe tener T() por defecto (constructor).

Si querés buscar por ID, debe tener int getID() const;.

Si tu clase no es POD (por ejemplo tiene std::string), no uses este método binario; en su lugar:

Serializá manualmente (escribir campos en orden con fwrite de cada campo o escribir en texto/JSON).

O convertir tu clase a una estructura "serializable" antes de escribir.

Buscar por ID y eficiencia
buscarPorID recorre el archivo linealmente (O(n)). Si tu aplicación crece y necesitás búsquedas rápidas, pensá en:

Índice en memoria (mapa id->posición) que cargás al inicio.

Un archivo de índice separado.

¿Qué pasa si cambias la definición de T (añadís un campo)?
Los archivos binarios previos quedan incompatibles. Si necesitás versionado, hacé:

Archivo nuevo (ej: cliente_v2.dat) o

Añadí cabecera con versión y serialización controlada.

Consejos/prácticas recomendadas rápidas

Para nombres/strings usa char nombre[40] si vas a fwrite/fread directo.

Evitá std::string dentro de T si usarás fread/fwrite directo.

Manejá errores: las funciones que devuelven bool te dicen si la operación tuvo éxito. Podés mejorar retornando códigos de error.

Si querés buscar por otro campo (ej idCliente) create una función genérica que reciba un predicado:


template<typename Pred>
int buscarPor(const Pred& p) {
    FILE* f = fopen(ruta,"rb");
    if (!f) return -1;
    T aux; int pos=0;
    while (fread(&aux,sizeof(T),1,f)==1){
        if (p(aux)) { fclose(f); return pos; }
        pos++;
    }
    fclose(f);
    return -1;
}