#pragma once
#include "IMetric.h"

// Calcula la Centralidad de Grado (Degree Centrality). 
// Básicamente, responde a la pregunta clave: "¿Cuántas conexiones directas tiene este nodo?"
class DegreeCentrality : public IMetric {
public:
    // Constructor con dos opciones muy importantes para los experimentos:
    // - normalized: Si es 'true', divide el conteo total por (N-1). Esto escala los puntajes 
    //               para que queden entre 0.0 y 1.0, permitiendo comparar redes de distinto tamaño.
    // - in_degree:  Si la red es dirigida (como Twitter, donde te siguen pero tú no los sigues),
    //               poner esto en 'true' cuenta solo las flechas que ENTRAN al nodo.
    explicit DegreeCentrality(bool normalized = true, bool in_degree = false);

    // Función obligatoria exigida por IMetric. Aquí es donde ocurre la magia.
    // Toma el grafo, cuenta los vecinos y devuelve el diccionario [ID_NODO -> PUNTAJE].
    std::unordered_map<int, double> compute(
        const Graph& g, ProgressFn progress = nullptr) const override;
        
    // Etiqueta de texto para que el CSV y los prints en consola salgan con el nombre correcto.
    std::string name() const override { return "Degree Centrality"; }

private:
    // Variables internas para recordar qué configuración le pedimos al crear el objeto.
    bool normalized_;
    bool in_degree_;
};