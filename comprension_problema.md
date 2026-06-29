# Comprensión del Problema — Métricas de Centralidad en Redes

## ¿Qué es una red?

Una **red** (o grafo) es un conjunto de **vértices** (nodos) conectados por **aristas** (relaciones). Dependiendo del contexto, los vértices representan entidades distintas:

| Red | Vértice | Arista |
|---|---|---|
| Proteínas de levadura | Proteína | Interacción física |
| Comercio internacional | País | Flujo comercial |
| Actores de IMDb | Actor | Aparición en la misma película |
| IoT | Dirección IP | Conexión de red |

La pregunta central del proyecto es: **¿cuál elemento de la red es el más "importante"?** Las métricas de centralidad responden eso desde distintos ángulos.

---

## Las 7 métricas

### 1. Degree Centrality

**Idea:** un vértice es importante si tiene muchas conexiones directas.

**Fórmula (no dirigido, normalizada):**

$$C_D(v) = \frac{deg(v)}{n - 1}$$

donde $deg(v)$ es el número de vecinos de $v$ y $n$ el total de vértices.

**En grafos dirigidos** se distingue:
- *In-degree*: aristas que llegan a $v$ (popularidad)
- *Out-degree*: aristas que salen de $v$ (actividad)

**Complejidad:** O(V + E)

**Interpretación real:** en la red de proteínas, una proteína con Degree alto interactúa con muchas otras — es un "hub" biológico y suele ser esencial para la célula.

---

### 2. Betweenness Centrality

**Idea:** un vértice es importante si aparece en los caminos más cortos entre otros vértices — actúa como "puente" o intermediario.

**Fórmula:**

$$C_B(v) = \sum_{s \neq v \neq t} \frac{\sigma_{st}(v)}{\sigma_{st}}$$

donde:
- $\sigma_{st}$ = número de caminos más cortos entre $s$ y $t$
- $\sigma_{st}(v)$ = cuántos de esos caminos pasan por $v$

**Normalización:** se divide por $(n-1)(n-2)$ para dirigido, o $(n-1)(n-2)/2$ para no dirigido.

**Algoritmo usado:** Brandes — O(V · E)

**Interpretación real:** en redes de comercio, un país con alta Betweenness es un intermediario crítico del comercio global. Si se elimina, muchos flujos se interrumpen.

---

### 3. Closeness Centrality

**Idea:** un vértice es importante si está cerca (en promedio) de todos los demás.

**Fórmula (Wasserman & Faust, maneja grafos desconectados):**

$$C_C(v) = \frac{n-1}{N-1} \cdot \frac{n-1}{\sum_{u \neq v} d(v, u)}$$

donde $n$ es el número de vértices alcanzables desde $v$, $N$ el total de vértices, y $d(v,u)$ la distancia más corta.

**Algoritmo:** BFS desde cada vértice — O(V · (V + E))

**Interpretación real:** en una red de carreteras, un nodo con alta Closeness es el lugar ideal para un cuartel de bomberos porque llega rápido a cualquier punto.

---

### 4. PageRank

**Idea:** un vértice es importante si está conectado a otros vértices importantes. Un voto de un vértice importante vale más que un voto de uno irrelevante.

**Fórmula iterativa:**

$$PR(v) = \frac{1-d}{n} + d \sum_{u \in N^{-}(v)} \frac{PR(u)}{|N^{+}(u)|}$$

donde:
- $d = 0.85$ es el factor de amortiguación (damping factor)
- $N^{-}(v)$ = vecinos que apuntan hacia $v$
- $N^{+}(u)$ = vecinos a los que apunta $u$

**Complejidad:** O(k · (V + E)) donde $k$ es el número de iteraciones hasta convergencia.

**Interpretación real:** es el algoritmo original de Google. En la red de comercio, un país con alto PageRank importa/exporta con otros países que a su vez tienen mucho comercio.

---

### 5. Average Shortest Path

**Idea:** mide qué tan lejos está un vértice en promedio del resto. A diferencia de Closeness, no es una métrica de "centralidad" sino de "aislamiento".

**Fórmula por vértice:**

$$ASP(v) = \frac{1}{|R(v)|} \sum_{u \in R(v)} d(v, u)$$

donde $R(v)$ son los vértices alcanzables desde $v$.

**Métrica global del grafo:** promedio de todos los $ASP(v)$ — indica cuán "compacta" es la red entera.

**Algoritmo:** BFS desde cada vértice — O(V · (V + E))

**Interpretación real:** en redes sociales, el famoso "seis grados de separación" es justamente el Average Shortest Path global de la red.

---

### 6. Eigenvector Centrality *(adicional)*

**Idea:** extiende Degree Centrality ponderando cada vecino por su propia importancia. Es la base matemática de PageRank.

**Fórmula:**

$$C_E(v) = \frac{1}{\lambda} \sum_{u \in N(v)} C_E(u)$$

En forma matricial: $\mathbf{A} \mathbf{x} = \lambda \mathbf{x}$, donde $\mathbf{x}$ es el eigenvector principal de la matriz de adyacencia $\mathbf{A}$.

**Algoritmo:** Power Iteration — O(k · (V + E))

**Interpretación real:** una proteína con alta Eigenvector centralidad no solo tiene muchas interacciones, sino que interactúa con otras proteínas que también son muy conectadas. Tiende a identificar el núcleo funcional de la célula.

---

### 7. Clustering Coefficient *(adicional)*

**Idea:** mide qué fracción de los vecinos de un vértice están conectados entre sí — qué tan "agrupado" está el entorno local.

**Fórmula local:**

$$C_{Cl}(v) = \frac{\text{triángulos que pasan por } v}{\binom{deg(v)}{2}} = \frac{t_v}{deg(v)(deg(v)-1)}$$

donde $t_v$ es el número de pares de vecinos de $v$ que están conectados entre sí.

**Coeficiente global:** promedio de todos los coeficientes locales.

**Complejidad:** O(V · deg² ) en el peor caso

**Interpretación real:** en la red de actores de IMDb, un actor con alto CC actúa siempre con el mismo grupo de actores (que también actúan entre sí). Un CC bajo indica que actúa con actores de distintos "círculos" sin que estos se conozcan.

---

## Representación del grafo: Lista de Adyacencia

Se eligió lista de adyacencia porque todos los datasets son **redes dispersas** (pocas aristas relativas al máximo posible):

| Representación | Memoria | Iterar vecinos | ¿Apta para redes reales? |
|---|---|---|---|
| Matriz de adyacencia | O(V²) | O(V) | No — Yeast: 6526² = 42M entradas |
| **Lista de adyacencia** | **O(V + E)** | **O(deg)** | **Sí** |

Con Yeast: V=6526, E=532.180 → lista usa ~24 MB vs ~340 MB de una matriz.

---

## Datasets utilizados

### Dataset 1 — Yeast PPI (Proteínas de Levadura)
- **Tipo:** No dirigido, no ponderado
- **Vértices:** 6.526 proteínas
- **Aristas:** 532.180 interacciones
- **Formato:** `proteina_A \t proteina_B` (una arista por línea)
- **Parser:** `EdgeListParser` con mapeo automático string→int

### Dataset 2 — Trade Network (Comercio Internacional)
- **Tipo:** Dirigido, ponderado
- **Vértices:** ~161 países
- **Aristas:** miles de relaciones comerciales
- **Formato:** Pajek `.net` con secciones `*Vertices` y `*Arcs`
- **Años disponibles:** 2000, 2005, 2010, 2015, 2018 (combinados en un solo grafo)
- **Parser:** `PajekParser`

> **Nota sobre los pesos:** el ADT Grafo almacena el peso de las aristas, pero las
> métricas operan sobre la *topología* no ponderada (caminos por número de saltos,
> reparto uniforme en PageRank). La dirección de las aristas sí se respeta en Trade.
> La versión ponderada (Dijkstra, PageRank ponderado) queda como trabajo futuro.

---

## Experimentos requeridos

### 1. Construcción del grafo
- Tiempo de construcción para cada dataset
- Memoria utilizada (en bytes/KB)

### 2. Tiempo de cómputo de cada métrica
- Cada métrica se ejecuta **10 veces**
- Se reporta **media** y **varianza** del tiempo en milisegundos

### 3. Impacto de modificar aristas
Se prueba en **tres zonas distintas** de la red según el grado de los nodos fuente:
*hub* (10% superior), *periférico* (10% inferior) y *aleatorio*.
- **Agregar** 5 aristas por zona → medir cambio porcentual en cada métrica
- **Quitar** 5 aristas por zona → medir cambio porcentual en cada métrica

---

## Análisis de peor caso

| Métrica | Complejidad |
|---|---|
| Degree Centrality | O(V + E) |
| Betweenness Centrality | O(V · E) |
| Closeness Centrality | O(V · (V + E)) |
| PageRank | O(k · (V + E)) |
| Average Shortest Path | O(V · (V + E)) |
| Eigenvector Centrality | O(k · (V + E)) |
| Clustering Coefficient | O(V · deg²) |

donde $k$ es el número de iteraciones hasta convergencia (en la práctica, < 100).

**Peor caso de construcción del grafo** (lista de adyacencia):
- `addVertex`: O(1) esperado (inserción en `unordered_map`; O(V) en el peor caso con colisiones)
- `addEdge`: O(1) esperado (búsqueda en hash + `push_back`)
- `removeEdge`: O(deg)
- `hasEdge`: O(deg)
- Total construcción: O(V + E)

**Complejidad de espacio:** todas las métricas guardan un resultado O(V) (un valor por vértice).
La mayoría no usa memoria auxiliar adicional más allá del BFS (O(V)); la excepción es
Betweenness (Brandes), que mantiene listas de predecesores y pila: O(V + E).
