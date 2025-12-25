# Resonant Graph Synth - Roadmap

## Vision

Un sintetizador fisico-armonico basado en una red de resonadores acoplados.
Cada nota es un nodo; el sonido emerge de como la energia se propaga por un grafo musical.

**Inspiracion:** Piano con cuerdas simpaticas, sitar, campanas - generalizado a estructuras musicales abstractas.

**Diferenciador:** No es subtractive/FM/wavetable - es *physical/structural synthesis*.

---

## Arquitectura

```
MIDI / GUI
     |
Voice Manager (polyphony)
     |
Excitation (pluck / strike / bow)
     |
+----------------------------+
|  Resonator Graph Engine    |
|                            |
|  Node = physical resonator |
|  Edge = energy coupling    |
+----------------------------+
     |
Global Space / Reverb
     |
   Output
```

---

## Milestones

### M0: Scaffolding (1-2 dias)
- [ ] Proyecto JUCE configurado (Projucer o CMake)
- [ ] Build standalone funcionando
- [ ] Estructura de directorios
- [ ] Audio callback basico (sine test)

**Criterio de exito:** App abre, genera tono de prueba.

---

### M1: Single Resonator (3-5 dias)
- [ ] Clase `Resonator` con Karplus-Strong extendido
- [ ] Parametros: frequency, damping, brightness, inharmonicity
- [ ] Excitacion por MIDI note-on
- [ ] GUI minima: 1 slider de damping

**Criterio de exito:** Tocar notas MIDI, escuchar cuerdas plucked.

**Archivos:**
- `src/core/Resonator.h/.cpp`
- `src/core/Exciter.h/.cpp`

---

### M2: Graph Engine (5-7 dias)
- [ ] Clase `ResonatorGraph` con N nodos
- [ ] Matriz de acoplamiento sparse
- [ ] Propagacion de energia por bloque
- [ ] Estabilidad garantizada (limitadores)
- [ ] Grafos predefinidos: cromatico, quintas, Tonnetz

**Criterio de exito:** Tocar C4, escuchar resonancia en G4 y E4.

**Archivos:**
- `src/core/ResonatorGraph.h/.cpp`
- `src/core/GraphTopology.h/.cpp`

---

### M3: Voice Manager (3-4 dias)
- [ ] Polifonia (8-16 voces)
- [ ] Voice stealing inteligente
- [ ] Energia compartida entre voces
- [ ] Note-off con decay natural

**Criterio de exito:** Acordes suenan bien, sin glitches.

**Archivos:**
- `src/core/VoiceManager.h/.cpp`
- `src/core/Voice.h/.cpp`

---

### M4: GUI Basica (4-5 dias)
- [ ] Visualizacion del grafo (nodos + conexiones)
- [ ] Energia por nodo (brillo/tamano)
- [ ] Controles globales: damping, coupling, topology
- [ ] Teclado virtual o piano roll simple

**Criterio de exito:** Ver energia propagarse visualmente.

**Archivos:**
- `src/gui/GraphView.h/.cpp`
- `src/gui/MainComponent.h/.cpp`

---

### M5: Presets Instrumentales (3-4 dias)
- [ ] Sistema de presets (JSON o ValueTree)
- [ ] Preset: Piano Resonante
- [ ] Preset: Sitar Extendido
- [ ] Preset: Campana Coral
- [ ] Preset: Pad Estructural

**Criterio de exito:** 4 presets que suenan distintos y musicales.

**Archivos:**
- `src/presets/PresetManager.h/.cpp`
- `resources/presets/*.json`

---

### M6: Refinamiento DSP (4-5 dias)
- [ ] Excitacion avanzada: bow, strike, pluck con parametros
- [ ] Damping dependiente de frecuencia
- [ ] No-linealidad suave (warmth)
- [ ] Reverb/space global

**Criterio de exito:** Sonido profesional, no "demo".

---

### M7: Plugin Build (2-3 dias)
- [ ] Target VST3
- [ ] Target AU (macOS)
- [ ] Parametros automatizables
- [ ] State save/load

**Criterio de exito:** Funciona en Logic/Ableton/Reaper.

---

### M8: Polish (3-4 dias)
- [ ] GUI final con look profesional
- [ ] Manual de usuario
- [ ] Video demo
- [ ] Release build optimizado

**Criterio de exito:** Listo para portfolio/GitHub release.

---

## Componentes DSP (Detalle)

### Resonator (por nodo)
```cpp
class Resonator {
    float frequency;      // Hz
    float damping;        // 0.99-0.9999
    float brightness;     // filtro LP
    float inharmonicity;  // detuning parciales

    DelayLine delayLine;
    LowpassFilter lpf;
    AllpassFilter apf;    // para inharmonicidad

    float process(float excitation);
    float getEnergy();
};
```

### Excitacion
```cpp
class Exciter {
    enum Type { Pluck, Strike, Bow };

    float position;   // 0-1 donde golpear
    float hardness;   // suave-duro
    float velocity;   // intensidad

    void trigger(Resonator& target);
};
```

### Grafo
```cpp
class ResonatorGraph {
    std::vector<Resonator> nodes;
    SparseMatrix<float> coupling;  // NxN

    void injectEnergy(int nodeIdx, float amount);
    void processBlock(float* output, int numSamples);

    void setTopology(Topology type);  // Chromatic, Fifths, Tonnetz
};
```

---

## Topologias de Grafo

| Nombre | Descripcion | Caracter |
|--------|-------------|----------|
| Chromatic | Cada nota conecta con +-1 semitono | Denso, cluster |
| Fifths | Circulo de quintas | Armonico, abierto |
| Tonnetz | Triangulos M3/m3/P5 | Neo-Riemanniano |
| Harmonic | Serie armonica desde fundamental | Brillante |
| Random | Conexiones aleatorias | Experimental |
| Custom | Editable por usuario | Libre |

---

## Stack Tecnico

- **C++17** (o 20 si JUCE lo soporta bien)
- **JUCE 7.x** - Framework audio/GUI
- **CMake** - Build system (preferido sobre Projucer)
- **Catch2** - Tests unitarios (opcional)

---

## Estructura de Archivos

```
resonant-graph-synth/
├── CMakeLists.txt
├── ROADMAP.md
├── README.md
├── CLAUDE.md
├── src/
│   ├── core/
│   │   ├── Resonator.h/.cpp
│   │   ├── Exciter.h/.cpp
│   │   ├── ResonatorGraph.h/.cpp
│   │   ├── GraphTopology.h/.cpp
│   │   ├── Voice.h/.cpp
│   │   └── VoiceManager.h/.cpp
│   ├── gui/
│   │   ├── MainComponent.h/.cpp
│   │   ├── GraphView.h/.cpp
│   │   └── ControlPanel.h/.cpp
│   └── presets/
│       └── PresetManager.h/.cpp
├── resources/
│   └── presets/
│       ├── piano-resonant.json
│       ├── sitar-extended.json
│       └── ...
└── docs/
    └── architecture.md
```

---

## Estimacion Total

| Milestone | Dias |
|-----------|------|
| M0 Scaffolding | 1-2 |
| M1 Single Resonator | 3-5 |
| M2 Graph Engine | 5-7 |
| M3 Voice Manager | 3-4 |
| M4 GUI Basica | 4-5 |
| M5 Presets | 3-4 |
| M6 DSP Refinement | 4-5 |
| M7 Plugin Build | 2-3 |
| M8 Polish | 3-4 |
| **Total** | **28-39 dias** |

Con sesiones de 2-3 horas/dia = 6-8 semanas realistas.

---

## MVP Minimo (M0-M2)

Si quieres algo funcionando rapido:
- 12 resonadores (1 octava)
- Grafo fijo de quintas
- Sin GUI elaborada
- Solo standalone

**Tiempo:** ~10 dias

---

## Siguiente Paso

Ejecutar M0: setup del proyecto JUCE con CMake.
