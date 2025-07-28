cmake_minimum_required(VERSION 3.10)
project(OtelYonetimSistemi)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -g")

include_directories(${CMAKE_SOURCE_DIR})

set(DOMAIN_SOURCES
    src/domain/entities/Musteri.cpp
    src/domain/entities/Oda.cpp
    src/domain/services/OtelDomainService.cpp
)

set(APPLICATION_SOURCES
    src/application/use-cases/OdaEkleUseCase.cpp
    src/application/use-cases/CheckInUseCase.cpp
    src/application/use-cases/CheckOutUseCase.cpp
)

set(INFRASTRUCTURE_SOURCES
    src/infrastructure/repositories/InMemoryOdaRepository.cpp
)

set(PRESENTATION_SOURCES
    src/presentation/OtelConsoleUI.cpp
)

add_executable(otel_yonetim 
    main.cpp
    ${DOMAIN_SOURCES}
    ${APPLICATION_SOURCES}
    ${INFRASTRUCTURE_SOURCES}
    ${PRESENTATION_SOURCES}
)

set_target_properties(otel_yonetim PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
