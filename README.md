# LawnFlowDyn
A spatially-explicit individual-based model simulating how lawn care practices (mowing frequency, wild patches) affect flowering plant diversity and pollinator potential in urban residential areas.

> **Author**: Alyssa Weber (MSc Forest and Ecosystem Sciences, Göttingen)  

---

## What It Does

Simulates growth, mowing, and reproduction of 4 common lawn plants:
- White Clover (*Trifolium repens*)
- Daisies (*Bellis perennis*)
- Dandelions (*Taraxacum officinale*)
- Red Fescue (*Festuca rubra*)

This IBM simulates how **reduced mowing** and **wild patches** increase plant diversity and support pollinators in urban lawns. The model runs for 26 weeks (1 week = 1 time step) across a 165m × 165m residential area (825×825 pixels, 20 cm resolution).

---

## Why It Matters

Lawns are the largest green space in cities. This model shows how small changes in management—like mowing less or letting corners grow—can boost biodiversity and support pollinators. It’s a foundation for science-based urban greening.

---

## Key UI Parameters

| Parameter | Default | Description |
|--------|--------|-------------|
| Mowing Frequency | 2 weeks | How often lawns are mowed (meadow patches mowed only at weeks 13 & 26) |
| General Mortality Chance | 0.1 | Chance of plant death during mowing |
| Spawn Chances | 0.25 each | Initial/reproduction probability per species |
| Scenario | 1 | Choose from 3 configurations: no wild patches, flower patches, or flower + meadow patches |

---

## Outputs

- Time-series of flower abundance per species
- Minimum, maximum, and averages of the weekly Effective Species Number (EFN) – a diversity metric
- Spatial visualization of plant distribution

---

## Future Directions

1. **Add solitary bee agents** to study pollinator foraging and emergent pathways.
2. Introduce **sunlight/shading** and **rainfall** dynamics
3. Model **flowering periods**  for plants and **nesting site dependency** for solitary bee agents.
4. Add individual home-owner decision-making for lawn configurations and care regimes

---

## ⚠️ Important Note: This Model Is Not Yet Ready for Practical Use

This model is **not a finished or validated tool** for real-world decision-making. It serves as a **foundational starting point** for future research and development.

---

## Try it out Yourself

Clone this repository into your preferred file location, open this project in QtCreator, and run it to experience it for yourself!
