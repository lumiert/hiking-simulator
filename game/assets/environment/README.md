# Environment Assets

Estrutura para organizar todos os assets de ambiente do cenário.

## Estrutura de Pastas

```
environment/
├── sky/              # Céu, sol, lua, stars
│   ├── sun.png       # Textura do sol (amarelo/laranja)
│   ├── moon.png      # Textura da lua (branco/azulado)
│   ├── stars.png     # Textura de stars (campo estelar)
│   └── skybox/       # Skybox cúbico (se necessário)
│
├── particles/        # Partículas e efeitos
│   ├── water_drop.png     # Gota de água
│   ├── dust.png           # Poeira
│   ├── sunray.png         # Raio de sol
│   ├── mist.png           # Névoa
│   └── rain.glb           # Malha de chuva
│
├── vegetation/       # Plantas e árvores
│   ├── leaves.png         # Textura de folhas
│   ├── leaf.glb           # Modelo de folha
│   ├── grass.png          # Textura de grama
│   └── flowers/
│
└── weather/          # Efeitos climáticos
    ├── rain.png           # Textura de chuva
    ├── snow.png           # Textura de neve
    ├── clouds.png         # Textura de nuvens
    └── lightning.png      # Relâmpago

```

## Formatos Suportados

- **Imagens**: PNG (com canal alfa)
- **Modelos**: GLB (formato binário)

## Como Usar

### Carregamento de Texturas Sky
```cpp
// sun.png carregado automaticamente em game/assets/environment/sky/
// moon.png carregado automaticamente em game/assets/environment/sky/
```

### Adicionando Novos Elementos

1. **Estrelas**: Coloque `stars.png` em `sky/` e adicione ao código
2. **Partículas**: Coloque texturas em `particles/` e crie Sistema de Partículas
3. **Folhas**: Coloque `leaves.png` e `leaf.glb` em `vegetation/`
4. **Chuva/Neve**: Coloque em `weather/` e implemente Sistema de Clima

## Convenções

- Use nomes em **lowercase com underscores**
- Texturas: máximo 2048x2048
- Modelos: optimize com poly count <10k
- Inclua versão PNG com canal alfa para transparência
