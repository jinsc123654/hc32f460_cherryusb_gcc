cmake_minimum_required(VERSION 3.22)

# Enable CMake support for ASM and C languages
enable_language(C ASM)

# ========================
# HC32F460 Generated Symbols
# ========================
set(HC_Defines_Syms
        USE_DDL_DRIVER
        HC32F460
        HC32F460xE
        $<$<CONFIG:Debug>:DEBUG>
)

# ========================
# HC32F460 Include Paths
# ========================
set(HC_Include_Dirs
        ${CMAKE_SOURCE_DIR}/Core/applications
        ${CMAKE_SOURCE_DIR}/Drivers/cmsis/Device/HDSC/hc32f4xx/Include
        ${CMAKE_SOURCE_DIR}/Drivers/cmsis/Include
        ${CMAKE_SOURCE_DIR}/Drivers/hc32_ll_driver/inc
        ${CMAKE_SOURCE_DIR}/Core/board
        # 组件依赖
        ${CMAKE_SOURCE_DIR}/Core/components/segger_rtt/SEGGER_RTT_V768c/RTT
        ${CMAKE_SOURCE_DIR}/Core/components/usb_app
)

# ========================
# 自动查找所有源文件
# ========================
file(GLOB HC32_Drivers_Src
        ${CMAKE_SOURCE_DIR}/Core/board/*.c
        ${CMAKE_SOURCE_DIR}/Drivers/cmsis/Device/HDSC/hc32f4xx/Source/*.c
        ${CMAKE_SOURCE_DIR}/Drivers/hc32_ll_driver/src/*.c
        ${CMAKE_SOURCE_DIR}/Core/applications/*.c
        # 组件依赖
        ${CMAKE_SOURCE_DIR}/Core/components/segger_rtt/SEGGER_RTT_V768c/RTT/*.c

)

# 排除特定文件
list(REMOVE_ITEM HC32_Drivers_Src
        ${CMAKE_SOURCE_DIR}/Drivers/hc32_ll_driver/src/hc32_ll_usb.c
)

message(STATUS "Found STM32 driver source files: ${HC32_Drivers_Src}")

# ========================
# Application Sources
# ========================
set(HC_Application_Src
        ${CMAKE_SOURCE_DIR}/Core/board/startup_hc32f460.s
)

# ========================
# Link Directories & Libraries
# ========================
set(HC_LINK_DIRS "")
set(HC_LINK_LIBS
        HC32_Drivers
)

# ========================
# Interface library for includes and symbols
# ========================
add_library(hc32f460 INTERFACE)
target_include_directories(hc32f460 INTERFACE ${HC_Include_Dirs})
target_compile_definitions(hc32f460 INTERFACE ${HC_Defines_Syms})

# ========================
# Create HC32_Drivers static library
# ========================
add_library(HC32_Drivers OBJECT)
target_sources(HC32_Drivers PRIVATE ${HC32_Drivers_Src})
target_link_libraries(HC32_Drivers PUBLIC hc32f460)

# ========================
# Add hc32f460 application sources
# ========================
target_sources(${CMAKE_PROJECT_NAME} PRIVATE ${HC_Application_Src})

# ========================
# Link directories & libraries
# ========================
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE ${HC_LINK_DIRS})
target_link_libraries(${CMAKE_PROJECT_NAME} ${HC_LINK_LIBS})

# ========================
# Add map file to clean target
# ========================
set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES ADDITIONAL_CLEAN_FILES ${CMAKE_PROJECT_NAME}.map)

# ========================
# Validate C standard
# ========================
if((CMAKE_C_STANDARD EQUAL 90) OR (CMAKE_C_STANDARD EQUAL 99))
    message(ERROR "Generated code requires C11 or higher")
endif()

# ========================
# ========================
# CherryUSB Integration
# ========================
# ========================
set(USE_CHERRY_USB_1.3.0 OFF)
if(USE_CHERRY_USB_1.3.0)
    # 初始化 CherryUSB 列表
    set(cherryusb_srcs "")
    set(cherryusb_incs "")

    # CherryUSB 配置
    set(CONFIG_CHERRYUSB_DEVICE 1)
    set(CONFIG_CHERRYUSB_DEVICE_CDC 1)
    #set(CONFIG_CHERRYUSB_DEVICE_HID 1)
    #set(CONFIG_CHERRYUSB_DEVICE_MSC 1)
    set(CONFIG_CHERRYUSB_DEVICE_DCD "dwc2_hc")
    #set(CONFIG_CHERRYUSB_HOST 0)
    #set(CONFIG_CHERRYUSB_OSAL "freertos")

    # 导入 cherryusb.cmake
    include(${CMAKE_SOURCE_DIR}/Core/components/usb_app/CherryUSB-1.3.0/cherryusb.cmake)

    # 添加 include 路径
    target_include_directories(hc32f460 INTERFACE ${cherryusb_incs})

    # 创建对象库并加入工程
    add_library(CherryUSB OBJECT)
    target_sources(CherryUSB PRIVATE ${cherryusb_srcs})
    target_link_libraries(CherryUSB PUBLIC hc32f460)
    target_sources(${CMAKE_PROJECT_NAME} PRIVATE $<TARGET_OBJECTS:CherryUSB>)
endif()

# 是否启用 CherryUSB 1.3.0
set(USE_CHERRY_USB_1.3.0 OFF)
if(USE_CHERRY_USB_1.3.0)

    target_compile_definitions(${CMAKE_PROJECT_NAME} PUBLIC USE_CHERRY_USB_1_3_0=1)
    # ------------------ 初始化 CherryUSB 源码列表 ------------------
    set(cherryusb_srcs "")
    set(cherryusb_incs "")

    # ------------------ CherryUSB 配置 ------------------
    set(CONFIG_CHERRYUSB_DEVICE 1)
    set(CONFIG_CHERRYUSB_DEVICE_CDC 1)
    #set(CONFIG_CHERRYUSB_DEVICE_HID 1)
    #set(CONFIG_CHERRYUSB_DEVICE_MSC 1)
    set(CONFIG_CHERRYUSB_DEVICE_DCD "dwc2_hc")
    #set(CONFIG_CHERRYUSB_HOST 0)
    #set(CONFIG_CHERRYUSB_OSAL "freertos")
    # ------------------ 导入 CherryUSB.cmake ------------------
    include(${CMAKE_SOURCE_DIR}/Core/components/usb_app/CherryUSB-1.3.0/cherryusb.cmake)

    # ------------------ 添加 include 路径到 hc32f460 target ------------------
    target_include_directories(hc32f460 INTERFACE ${cherryusb_incs})

    # ------------------ 创建 CherryUSB 对象库 ------------------
    add_library(CherryUSB OBJECT)
    target_sources(CherryUSB PRIVATE ${cherryusb_srcs})
    target_include_directories(CherryUSB PUBLIC ${cherryusb_incs})
    target_link_libraries(CherryUSB PUBLIC hc32f460)

    # ------------------ 将 CherryUSB 对象库加入主项目 ------------------
    target_sources(${CMAKE_PROJECT_NAME} PRIVATE $<TARGET_OBJECTS:CherryUSB>)

    # ------------------ 单独添加自定义 C 文件 ------------------
    target_sources(${CMAKE_PROJECT_NAME} PRIVATE
            ${CMAKE_SOURCE_DIR}/Core/components/usb_app/src/bsp_usb_init.c
            ${CMAKE_SOURCE_DIR}/Core/components/usb_app/src/cdc_acm_template_1.3.0.c
    )
endif()



# 是否启用 CherryUSB 1.5.2
set(USE_CHERRY_USB_1.5.2 ON)

# 是否启用 CherryUSB 1.5.2
if(USE_CHERRY_USB_1.5.2)
    # ------------------ 对主 target 添加宏 ------------------
    target_compile_definitions(${CMAKE_PROJECT_NAME} PUBLIC USE_CHERRY_USB_1_5_2=1)

    # ------------------ 初始化 CherryUSB 源码列表 ------------------
    set(cherryusb_srcs "")
    set(cherryusb_incs "")

    # ------------------ CherryUSB 配置 ------------------
    set(CONFIG_CHERRYUSB_DEVICE 1)
    set(CONFIG_CHERRYUSB_DEVICE_CDC_ACM 1)
    set(CONFIG_CHERRYUSB_DEVICE_DWC2_HC 1)

    # ------------------ 导入 CherryUSB.cmake ------------------
    include(${CMAKE_SOURCE_DIR}/Core/components/usb_app/CherryUSB-1.5.2/cherryusb.cmake)

    # ------------------ 添加 include 路径 ------------------
    target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ${cherryusb_incs})
    target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/Core/components/usb_app)

    # ------------------ 添加 CherryUSB 源文件 ------------------
    target_sources(${CMAKE_PROJECT_NAME} PRIVATE
            ${cherryusb_srcs}
            ${CMAKE_SOURCE_DIR}/Core/components/usb_app/src/bsp_usb_init.c
            ${CMAKE_SOURCE_DIR}/Core/components/usb_app/src/cdc_acm_template_1.5.2.c
    )

    # ------------------ 屏蔽重复宏定义 warning ------------------
    target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE -Wno-redefined-macro)
endif()
