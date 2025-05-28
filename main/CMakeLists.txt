# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.1)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.1.1)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico CACHE STRING "Board type")

add_executable(pico_emb 
    tft_lcd_ili9341/ili9341/ili9341.c
    tft_lcd_ili9341/gfx/gfx.c
    main.c
)

target_include_directories(pico_emb PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}                                   # <raiz>/main
    ${CMAKE_CURRENT_LIST_DIR}/tft_lcd_ili9341/ili9341           # headers ili9341
    ${CMAKE_CURRENT_LIST_DIR}/tft_lcd_ili9341/gfx               # headers gfx
)

target_link_libraries(pico_emb
    pico_stdlib
    hardware_spi
    hardware_adc
    hardware_dma
    hardware_pwm
    freertos
)
set_target_properties(pico_emb PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(pico_emb)
