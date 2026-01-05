# Store this directory for relative path resolution when included from other CMakeLists
set(NICE_OLED_COMMON_DIR ${CMAKE_CURRENT_LIST_DIR})

if(CONFIG_ZMK_DISPLAY AND CONFIG_NICE_OLED_WIDGET_STATUS)

    zephyr_library()
    file(GLOB font_sources ${NICE_OLED_COMMON_DIR}/src/fonts/*.c)
    zephyr_library_sources(${ZEPHYR_BASE}/misc/empty_file.c)
    zephyr_library_include_directories(${ZEPHYR_LVGL_MODULE_DIR})
    zephyr_library_include_directories(${ZEPHYR_BASE}/lib/gui/lvgl/)
    zephyr_library_include_directories(${ZEPHYR_BASE}/drivers)
    zephyr_library_include_directories(${CMAKE_SOURCE_DIR}/include)
    zephyr_library_include_directories(${ZEPHYR_CURRENT_MODULE_DIR}/include)
    zephyr_library_include_directories(${ZEPHYR_CURRENT_CMAKE_DIR}/include)
    zephyr_library_include_directories(${NICE_OLED_COMMON_DIR}/include)
    if(font_sources)
      zephyr_library_sources(${font_sources})
    endif()

    if (CONFIG_NICE_OLED_WIDGET_RAW_HID_DRIVER)
      zephyr_library_sources(${NICE_OLED_COMMON_DIR}/src/raw_hid/events.c)
      zephyr_library_sources(${NICE_OLED_COMMON_DIR}/src/raw_hid/usb_hid.c)
      zephyr_library_sources(${NICE_OLED_COMMON_DIR}/src/raw_hid/hid.c)

      if (CONFIG_ZMK_BLE)
        zephyr_library_sources(${NICE_OLED_COMMON_DIR}/src/raw_hid/hog.c)
      endif()

    endif()

    if(CONFIG_NICE_OLED_WIDGET_RAW_HID_DRIVER)
      zephyr_include_directories(${NICE_OLED_COMMON_DIR}/include)
      zephyr_include_directories(${APPLICATION_SOURCE_DIR}/include)
    endif()

    zephyr_library_sources(${NICE_OLED_COMMON_DIR}/custom_status_screen.c)
    zephyr_library_sources(${NICE_OLED_COMMON_DIR}/assets/images.c)

    if(CONFIG_NICE_OLED_WIDGET_CENTRAL_SHOW_BATTERY_PERIPHERAL_ALL OR CONFIG_NICE_OLED_WIDGET_CENTRAL_SHOW_BATTERY_PERIPHERAL_ONLY OR CONFIG_NICE_OLED_WIDGET_CENTRAL_SHOW_BATTERY_PERIPHERAL_AND_CENTRAL)
      # TODO
      # target_sources_ifdef(CONFIG_NICE_OLED_WIDGET_CENTRAL_SHOW_BATTERY_PERIPHERAL_ALL app PRIVATE ${NICE_OLED_COMMON_DIR}/widgets/bolt.c)
      # target_sources_ifdef(CONFIG_NICE_OLED_WIDGET_CENTRAL_SHOW_BATTERY_PERIPHERAL_ALL app PRIVATE ${NICE_OLED_COMMON_DIR}/widgets/gear_icon.c)
      # target_sources_ifdef(CONFIG_NICE_OLED_WIDGET_CENTRAL_SHOW_BATTERY_PERIPHERAL_ALL app PRIVATE ${NICE_OLED_COMMON_DIR}/widgets/disconnected_icon.c)
      # target_sources_ifdef(CONFIG_NICE_OLED_WIDGET_CENTRAL_SHOW_BATTERY_PERIPHERAL_ALL app PRIVATE ${NICE_OLED_COMMON_DIR}/widgets/warning_animation.c)
    else()
      zephyr_library_sources(${NICE_OLED_COMMON_DIR}/widgets/battery.c)
    endif()
    zephyr_library_sources(${NICE_OLED_COMMON_DIR}/widgets/output.c)
    zephyr_library_sources(${NICE_OLED_COMMON_DIR}/widgets/util.c)

    zephyr_library_sources(${NICE_OLED_COMMON_DIR}/assets/vim.c)
    if(CONFIG_NICE_OLED_SHOW_SLEEP_ART_ON_IDLE OR CONFIG_NICE_OLED_SHOW_SLEEP_ART_ON_IDLE)
      zephyr_library_sources(${NICE_OLED_COMMON_DIR}/assets/sleep_status_art.c)
      zephyr_library_sources(${NICE_OLED_COMMON_DIR}/widgets/sleep_status.c)
    endif()

    zephyr_library_sources(${NICE_OLED_COMMON_DIR}/assets/luna_images.c)
    zephyr_library_sources(${NICE_OLED_COMMON_DIR}/assets/bongo_cat_images.c)


    if(NOT CONFIG_ZMK_SPLIT OR CONFIG_ZMK_SPLIT_ROLE_CENTRAL)

      if(CONFIG_NICE_OLED_WIDGET_WPM)
        zephyr_library_sources(${NICE_OLED_COMMON_DIR}/widgets/wpm.c)
        target_sources_ifdef(CONFIG_NICE_OLED_WIDGET_WPM_LUNA app PRIVATE ${NICE_OLED_COMMON_DIR}/widgets/luna.c)
        target_sources_ifdef(CONFIG_NICE_OLED_WIDGET_WPM_BONGO_CAT app PRIVATE ${NICE_OLED_COMMON_DIR}/widgets/bongo_cat.c)
      endif()

      if(CONFIG_NICE_OLED_WIDGET_CONNECTION_STATUS)
        # TODO
      endif()

      target_sources_ifdef(CONFIG_NICE_OLED_WIDGET_HID_INDICATORS app PRIVATE ${NICE_OLED_COMMON_DIR}/widgets/hid_indicators.c)
      target_sources_ifdef(CONFIG_NICE_OLED_WIDGET_MODIFIERS_INDICATORS_LUNA app PRIVATE ${NICE_OLED_COMMON_DIR}/widgets/modifiers.c)
      target_sources_ifdef(CONFIG_NICE_OLED_WIDGET_MODIFIERS_INDICATORS_FIXED_SYMBOL app PRIVATE ${NICE_OLED_COMMON_DIR}/assets/modifiers_sym.c)

      if(CONFIG_NICE_OLED_WIDGET_RESPONSIVE)
        # target_sources_ifdef(CONFIG_NICE_OLED_WIDGET_RESPONSIVE_BONGO_CAT app PRIVATE ${NICE_OLED_COMMON_DIR}/assets/responsive_bongo_cat_images.c)
        target_sources_ifdef(CONFIG_NICE_OLED_WIDGET_RESPONSIVE_BONGO_CAT app PRIVATE ${NICE_OLED_COMMON_DIR}/widgets/responsive_bongo_cat.c)
      endif()

      if(CONFIG_NICE_OLED_WIDGET_LAYER)
        zephyr_library_sources(${NICE_OLED_COMMON_DIR}/widgets/layer.c)
      endif()

      target_sources_ifdef(CONFIG_NICE_OLED_WIDGET_PROFILE_BIG app PRIVATE ${NICE_OLED_COMMON_DIR}/assets/profile_active.c)
      target_sources_ifdef(CONFIG_NICE_OLED_WIDGET_PROFILE_BIG app PRIVATE ${NICE_OLED_COMMON_DIR}/assets/profile.c)
      zephyr_library_sources(${NICE_OLED_COMMON_DIR}/widgets/profile.c)
      zephyr_library_sources(${NICE_OLED_COMMON_DIR}/widgets/screen.c)
    else()

      if(CONFIG_NICE_OLED_WIDGET_ANIMATION_PERIPHERAL_WPM)
        target_sources_ifdef(CONFIG_NICE_OLED_WIDGET_ANIMATION_PERIPHERAL_WPM app PRIVATE ${NICE_OLED_COMMON_DIR}/widgets/luna.c)
      endif()

      zephyr_library_sources(${NICE_OLED_COMMON_DIR}/assets/crystal.c)
      target_sources_ifdef(CONFIG_NICE_OLED_WIDGET_ANIMATION_PERIPHERAL_CAT app PRIVATE ${NICE_OLED_COMMON_DIR}/assets/cat.c)
      target_sources_ifdef(CONFIG_NICE_OLED_WIDGET_ANIMATION_PERIPHERAL_SPACEMAN app PRIVATE ${NICE_OLED_COMMON_DIR}/assets/spaceman.c)
      target_sources_ifdef(CONFIG_NICE_OLED_WIDGET_ANIMATION_PERIPHERAL_HEAD app PRIVATE ${NICE_OLED_COMMON_DIR}/assets/head.c)
      # target_sources_ifdef(CONFIG_NICE_OLED_WIDGET_STATIC_IMAGE_PERIPHERAL_VIM app PRIVATE ${NICE_OLED_COMMON_DIR}/assets/vim.c)
      target_sources_ifdef(CONFIG_NICE_OLED_WIDGET_STATIC_IMAGE_PERIPHERAL_VIP_MARCOS app PRIVATE ${NICE_OLED_COMMON_DIR}/assets/vip_marcos.c)
      target_sources_ifdef(CONFIG_NICE_OLED_WIDGET_ANIMATION_PERIPHERAL_POKEMON app PRIVATE ${NICE_OLED_COMMON_DIR}/assets/pokemon.c)
      zephyr_library_sources(${NICE_OLED_COMMON_DIR}/widgets/animation.c)
      zephyr_library_sources(${NICE_OLED_COMMON_DIR}/widgets/screen_peripheral.c)
    endif()
endif()
