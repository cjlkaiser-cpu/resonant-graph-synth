# Resonant Graph Synth

Un sintetizador fisico-armonico basado en una red de resonadores acoplados.

## Concepto

Cada nota es un **nodo** en un grafo musical. Cuando tocas una nota:
1. Se inyecta energia en ese nodo
2. La energia se propaga por las conexiones del grafo
3. Los nodos vecinos resuenan simpaticamente
4. El timbre emerge de la topologia

Piensa en un piano con cuerdas simpaticas, un sitar, o campanas - pero generalizado a estructuras musicales abstractas.

## Caracteristicas

- **Physical Synthesis**: No es FM/subtractive/wavetable - es sintesis estructural
- **Grafos musicales**: Cromatico, quintas, Tonnetz, o custom
- **Resonadores Karplus-Strong** extendidos con inharmonicidad
- **Visualizacion** de energia propagandose por el grafo
- **Presets instrumentales**: Piano resonante, Sitar, Campanas, Pads

## Build

Requiere:
- CMake 3.21+
- JUCE 7.x
- Compilador C++17 (clang, gcc, MSVC)

```bash
cmake -B build
cmake --build build
```

## Uso

```bash
# Standalone
./build/ResonantGraphSynth_artefacts/Standalone/ResonantGraphSynth

# O instalar plugins
cp -r build/ResonantGraphSynth_artefacts/VST3/* ~/Library/Audio/Plug-Ins/VST3/
```

## Arquitectura

```
src/
├── core/           # Motor DSP (C++ puro)
│   ├── Resonator   # Nodo individual
│   ├── ResonatorGraph  # Grafo completo
│   └── VoiceManager    # Polifonia
├── gui/            # Interfaz JUCE
└── presets/        # Sistema de presets
```

## Inspiracion

Desarrollado como parte del ecosistema [EigenLab](https://github.com/cjlkaiser/eigenlab), explorando la fisica del sonido y la sintesis estructural.

## Licencia

MIT
