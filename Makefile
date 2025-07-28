CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g -I.

SRC_DIR = src
BUILD_DIR = build
BIN_DIR = bin

DOMAIN_SOURCES = $(SRC_DIR)/domain/entities/Musteri.cpp \
                 $(SRC_DIR)/domain/entities/Oda.cpp \
                 $(SRC_DIR)/domain/services/OtelDomainService.cpp

APPLICATION_SOURCES = $(SRC_DIR)/application/use-cases/OdaEkleUseCase.cpp \
                      $(SRC_DIR)/application/use-cases/CheckInUseCase.cpp \
                      $(SRC_DIR)/application/use-cases/CheckOutUseCase.cpp

INFRASTRUCTURE_SOURCES = $(SRC_DIR)/infrastructure/repositories/InMemoryOdaRepository.cpp

PRESENTATION_SOURCES = $(SRC_DIR)/presentation/OtelConsoleUI.cpp

WEB_SOURCES = $(SRC_DIR)/presentation/web/WebServer.cpp

MAIN_SOURCE = main.cpp
MAIN_WEB_SOURCE = main_web.cpp

CONSOLE_SOURCES = $(MAIN_SOURCE) $(DOMAIN_SOURCES) $(APPLICATION_SOURCES) $(INFRASTRUCTURE_SOURCES) $(PRESENTATION_SOURCES)
CONSOLE_OBJECTS = $(CONSOLE_SOURCES:%.cpp=$(BUILD_DIR)/%.o)

WEB_SOURCES_ALL = $(MAIN_WEB_SOURCE) $(DOMAIN_SOURCES) $(APPLICATION_SOURCES) $(INFRASTRUCTURE_SOURCES) $(WEB_SOURCES)
WEB_OBJECTS = $(WEB_SOURCES_ALL:%.cpp=$(BUILD_DIR)/%.o)

TARGET_CONSOLE = $(BIN_DIR)/otel_yonetim
TARGET_WEB = $(BIN_DIR)/otel_yonetim_web

all: console web

console: $(TARGET_CONSOLE)

web: $(TARGET_WEB)

$(TARGET_CONSOLE): $(CONSOLE_OBJECTS) | $(BIN_DIR)
	$(CXX) $(CONSOLE_OBJECTS) -o $@

$(TARGET_WEB): $(WEB_OBJECTS) | $(BIN_DIR)
	$(CXX) $(WEB_OBJECTS) -o $@ -lpthread

$(BUILD_DIR)/%.o: %.cpp | $(BUILD_DIR)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

run-console: $(TARGET_CONSOLE)
	./$(TARGET_CONSOLE)

run-web: $(TARGET_WEB)
	./$(TARGET_WEB)

run: run-web

debug: CXXFLAGS += -DDEBUG -O0
debug: all

release: CXXFLAGS += -DNDEBUG -O2
release: clean all

.PHONY: all console web clean run run-console run-web debug release
