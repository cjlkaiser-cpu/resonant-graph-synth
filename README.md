# Resonant Graph Synth

Sintetizador fisico-armonico basado en una red de resonadores acoplados.

![Status](https://img.shields.io/badge/status-alpha-purple)
![C++17](https://img.shields.io/badge/C%2B%2B-17-blue)
![JUCE](https://img.shields.io/badge/JUCE-7.x-orange)

## Concepto

Cada nota es un **nodo** en un grafo musical. Cuando tocas:
1. Energia se inyecta en el nodo
2. Se propaga por las conexiones del grafo
3. Nodos vecinos resuenan simpaticamente
4. El timbre emerge de la topologia

Como un piano con cuerdas simpaticas, sitar, o campanas - generalizado a estructuras musicales abstractas.

## Demo

```bash
# Clonar
git clone --recursive https://github.com/cjlkaiser-cpu/resonant-graph-synth.git
cd resonant-graph-synth

# Build
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j8

# Ejecutar
open build/ResonantGraphSynth_artefacts/Release/Standalone/Resonant\ Graph\ Synth.app
```

## Caracteristicas

- **12 resonadores** Karplus-Strong en grafo circular
- **4 topologias**: Chromatic, Fifths, Tonnetz, Harmonic
- **Visualizacion** en tiempo real de energia
- **Teclado MIDI** integrado
- **Builds**: Standalone, VST3, AU

## Controles

| Control | Funcion |
|---------|---------|
| Damping | Duracion del sonido |
| Brightness | Brillo del timbre |
| Coupling | Intensidad de resonancia simpatica |
| Topology | Patron de conexiones entre nodos |

## Arquitectura

```
src/
├── core/                 # Motor DSP
│   ├── Resonator.cpp     # Karplus-Strong extendido
│   ├── ResonatorGraph.cpp # Grafo + propagacion
│   └── Exciter.cpp       # Generacion de impulsos
├── gui/
│   └── GraphView.cpp     # Visualizacion del grafo
├── PluginProcessor.cpp   # Audio callback
└── PluginEditor.cpp      # UI JUCE
```

## Stack

- C++17
- JUCE 7.x (submodulo)
- CMake 3.21+

## Origen

Evolucion de [Sympathetic-12](https://github.com/cjlkaiser-cpu/physics-sound-lab) (Rust/WASM) a aplicacion nativa.
Parte del ecosistema [EigenLab](https://github.com/cjlkaiser-cpu/eigenlab).

## Licencia

MIT
