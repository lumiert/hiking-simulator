# Sistema de Gerenciamento de Objetos 3D

## Overview

O `SceneManager` fornece um sistema simples e intuitivo para colocar e gerenciar objetos 3D em cena. Todos os modelos são carregados em cache e renderizados com shaders.

## API Principal

### Função: `placeObject()`

```cpp
int placeObject(const std::string& modelPath, float x, float y, float z, int collisionType);
```

**Parâmetros:**
- `modelPath` (string): Caminho para o arquivo GLTF (ex: `"game/assets/shared/models/shopping_cart/scene.gltf"`)
- `x, y, z` (float): Posição inicial do objeto no mundo
- `collisionType` (int): Tipo de colisão:
  - `0` = Sem colisão
  - `1` = Colisão estática (fixa)
  - `3` = Colisão dinâmica (afetada por física)

**Retorna:** ID do objeto (int) para manipulação posterior

## Exemplos de Uso

### Exemplo 1: Colocar um carrinho de compras
```cpp
// No código do shaders.exe ou do jogo
int cartId = scene.placeObject("game/assets/shared/models/shopping_cart/scene.gltf", 0.0f, 0.0f, 0.0f, 1);
```

### Exemplo 2: Múltiplos objetos
```cpp
// Carrinho no centro
int cart = scene.placeObject("game/assets/shared/models/shopping_cart/scene.gltf", 0.0f, 0.0f, 0.0f, 1);

// Carrinho sem colisão
int cart2 = scene.placeObject("game/assets/shared/models/shopping_cart/scene.gltf", 5.0f, 0.0f, 5.0f, 0);

// Objeto dinâmico
int dynamicCart = scene.placeObject("game/assets/shared/models/shopping_cart/scene.gltf", -5.0f, 0.0f, -5.0f, 3);
```

### Exemplo 3: Diferentes modelos
```cpp
// Assumindo que você tem múltiplos modelos em assets
int obj1 = scene.placeObject("game/assets/shared/models/model_a/scene.gltf", 0.0f, 0.0f, 0.0f, 1);
int obj2 = scene.placeObject("game/assets/shared/models/model_b/scene.gltf", 10.0f, 0.0f, 0.0f, 1);
int obj3 = scene.placeObject("game/assets/shared/models/model_c/scene.gltf", -10.0f, 0.0f, 0.0f, 0);
```

## Funções de Suporte

### Obter referência a um objeto
```cpp
SceneObject* obj = scene.getObject(cartId);
if (obj) {
    obj->position = glm::vec3(5.0f, 0.0f, 5.0f); // Mover objeto
    obj->rotation = glm::vec3(0.0f, 1.57f, 0.0f); // Rotacionar (radianos)
    obj->scale = glm::vec3(2.0f, 2.0f, 2.0f);    // Escalar
}
```

### Remover um objeto
```cpp
scene.removeObject(cartId);
```

### Renderizar todos os objetos
```cpp
// Automaticamente chamado em scene.renderAll()
scene.renderAll(shaderProgram, view, projection, time);
```

### Limpar a cena
```cpp
// Remover todos os objetos
scene.cleanup();
```

## Estrutura Interna

### CollisionType (Enum)
```cpp
enum class CollisionType {
    NONE = 0,      // Sem colisão, apenas visual
    STATIC = 1,    // Colisão estática (não se move)
    DYNAMIC = 3    // Colisão dinâmica (afetada por física)
};
```

### SceneObject
```cpp
struct SceneObject {
    int id;                           // ID único
    std::string modelPath;            // Caminho do arquivo
    glm::vec3 position;               // Posição (x, y, z)
    glm::vec3 rotation;               // Rotação em radianos (Euler)
    glm::vec3 scale;                  // Escala (1.0 = tamanho original)
    CollisionType collisionType;      // Tipo de colisão
    GLTFMeshData mesh;                // Dados da malha
    
    glm::mat4 getModelMatrix() const; // Obtém matriz de transformação
};
```

## Performance

- **Cache de Modelos**: Os modelos GLTF são carregados uma vez e reutilizados (cache automático)
- **Renderização em Lote**: Todos os objetos são renderizados em um único render call
- **Eficiência de Memória**: Múltiplas instâncias do mesmo modelo compartilham dados de mesh

## Limitações Atuais

1. **Parser GLTF Simplificado**: Atualmente carrega apenas estrutura básica. Para modelos complexos, usa fallback de cubo
2. **Sem Física Integrada**: Os tipos de colisão (1, 3) são apenas flags para uso futuro
3. **Sem Animações**: Modelos animados não têm suporte atualmente
4. **Sem Texturas**: Renderização apenas com cor baseada em normas

## Próximos Passos

Para melhorar o sistema:
1. Implementar parser GLTF completo com biblioteca tiny_gltf
2. Integrar motor de física (Bullet ou similar)
3. Adicionar suporte a texturas e materiais
4. Implementar LOD (Level of Detail) para otimização
5. Adicionar suporte a skeletal animations
