# Funcionalidad general 27032024
# Define las reglas para compilar el proyecto, incluyendo cómo compilar 
# los archivos fuente en objetos y luego enlazarlos en el ejecutable final. 
# También incluye una regla para limpiar los artefactos de compilación.

# Configuración del compilador: puede cambiar a clang++ si lo desea
CXX=g++
CXXFLAGS=-Iinclude -std=c++11 -Wall

# Definir las fuentes y el destino.
SOURCES=$(wildcard src/*.cpp)
OBJECTS=$(SOURCES:src/%.cpp=build/%.o)
TARGET=build/app

# Objetivo predeterminado
all: $(TARGET)

# Regla para vincular la aplicación
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Regla para compilar los archivos fuente en archivos objeto
build/%.o: src/%.cpp | build
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Crear el directorio de compilación
build:
	mkdir -p build

# Limpiar artefactos de construcción
clean:
	rm -rf build

.PHONY: all clean
