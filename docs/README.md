# Documentación del Proyecto - Modelos Probabilistas en C

Esta carpeta contiene la documentación completa del proyecto en formato Quarto Markdown.

## Archivos de Documentación

- **`proyecto-modelos-probabilistas.qmd`** - Documentación principal completa
- **`index.qmd`** - Página de inicio para el sitio web
- **`_quarto.yml`** - Configuración del proyecto Quarto
- **`references.bib`** - Bibliografía en formato BibTeX
- **`styles.css`** - Estilos personalizados para la documentación

## Cómo Generar la Documentación

### Prerrequisitos

1. **Instalar Quarto**: 
   ```bash
   # En Ubuntu/Debian
   wget https://github.com/quarto-dev/quarto-cli/releases/download/v1.4.550/quarto-1.4.550-linux-amd64.deb
   sudo dpkg -i quarto-1.4.550-linux-amd64.deb
   
   # En otras distribuciones, consultar: https://quarto.org/docs/get-started/
   ```

2. **Verificar instalación**:
   ```bash
   quarto --version
   ```

### Generar Documentación

#### Renderizar como HTML

```bash
cd docs/
quarto render proyecto-modelos-probabilistas.qmd --to html
```

Esto generará:
- `proyecto-modelos-probabilistas.html` - Documento HTML independiente

#### Renderizar como PDF

```bash
cd docs/
quarto render proyecto-modelos-probabilistas.qmd --to pdf
```

Esto generará:
- `proyecto-modelos-probabilistas.pdf` - Documento PDF

**Nota**: Para PDF requiere LaTeX instalado:
```bash
sudo apt-get install texlive-full  # Ubuntu/Debian
```

#### Crear Sitio Web Completo

```bash
cd docs/
quarto render
```

Esto genera un sitio web completo en la carpeta `_site/` con:
- Página de inicio
- Documentación completa
- Navegación automática
- Estilos personalizados

#### Modo Preview (Desarrollo)

```bash
cd docs/
quarto preview
```

Esto inicia un servidor local con recarga automática para desarrollo.

## Estructura de la Documentación

La documentación incluye:

### 1. Resumen Ejecutivo
- Características principales
- Introducción a los modelos implementados

### 2. Introducción Teórica
- Redes Bayesianas
- Modelos Ocultos de Markov
- Algoritmo de Viterbi

### 3. Arquitectura del Sistema
- Estructura del proyecto
- Componentes principales
- Flujo de datos

### 4. Implementación Detallada
- Estructuras de datos
- Algoritmo de Viterbi paso a paso
- Parámetros del modelo
- Ejemplos de ejecución

### 5. Sistema de Compilación
- Makefile y targets
- Flags de compilación
- Procedimientos de ejecución

### 6. Características Técnicas
- Gestión de memoria
- Validación de datos
- Precisión matemática
- Manejo de errores

### 7. Testing y Validación
- Estrategia de testing
- Casos de prueba
- Verificación de resultados

### 8. Extensiones Futuras
- Mejoras algorítmicas
- Optimizaciones
- Nuevas funcionalidades

### 9. Conclusiones
- Logros del proyecto
- Valor educativo
- Calidad del software

## Características de la Documentación

- ✅ **Formato profesional** con Quarto Markdown
- ✅ **Fórmulas matemáticas** en LaTeX
- ✅ **Código destacado** con syntax highlighting
- ✅ **Navegación automática** con tabla de contenidos
- ✅ **Bibliografía académica** con referencias
- ✅ **Estilos personalizados** para mejor legibilidad
- ✅ **Multiplataforma** (HTML, PDF, sitio web)
- ✅ **Responsive design** para diferentes dispositivos

## Personalización

### Modificar Estilos

Editar `styles.css` para cambiar:
- Colores y tipografía
- Espaciado y layout
- Estilos de código y fórmulas

### Agregar Contenido

1. Editar `proyecto-modelos-probabilistas.qmd`
2. Agregar nuevas secciones con headers `#`, `##`, `###`
3. Incluir código con bloques ```c
4. Agregar fórmulas con `$$` o `$`

### Bibliografía

Agregar nuevas referencias en `references.bib` usando formato BibTeX.

## Comandos Útiles

```bash
# Renderizar solo la documentación principal
quarto render proyecto-modelos-probabilistas.qmd

# Renderizar todo el sitio
quarto render

# Preview con recarga automática
quarto preview

# Limpiar archivos generados
quarto clean

# Verificar formato
quarto check

# Publicar en GitHub Pages (opcional)
quarto publish gh-pages
```

## Salida Generada

Después de renderizar, obtendrás:

### HTML
- Documento independiente con todos los recursos embebidos
- Navegación interactiva
- Código con syntax highlighting
- Fórmulas matemáticas renderizadas

### PDF
- Documento profesional listo para imprimir
- Tabla de contenidos automática
- Bibliografía formateada
- Calidad de publicación académica

### Sitio Web
- Múltiples páginas con navegación
- Diseño responsive
- Optimizado para motores de búsqueda
- Listo para deployment

## Troubleshooting

### Error de LaTeX (para PDF)
```bash
# Instalar dependencias completas
sudo apt-get install texlive-full pandoc
```

### Error de permisos
```bash
# Asegurar permisos de escritura
chmod -R 755 docs/
```

### Error de Quarto no encontrado
```bash
# Verificar PATH
echo $PATH
# Reinstalar Quarto si es necesario
```

Esta documentación proporciona una base sólida y profesional para el proyecto de modelos probabilistas, siguiendo las mejores prácticas académicas y de desarrollo de software.
