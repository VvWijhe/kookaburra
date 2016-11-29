## Copyright 2011-2016 Hogeschool van Arnhem en Nijmegen
## Opleiding Embedded Systems Engineering
## ir drs E.J Boks
## Basis CMake bestand voor STM32 gerelateerde development.
## $Id: STM32Development.cmake 2277 2016-08-29 12:41:49Z ewout $

if(NOT CMAKESCRIPTDIR)
  message(FATAL_ERROR "Geef met CMAKESCRIPTDIR aan waar de STM32 CMake buildscripts staan.")
else(NOT CMAKESCRIPTDIR)

  set(GEMDIR ${PROJECT_SOURCE_DIR}/../gemeenschappelijk)
  set(BASISDIR ${GEMDIR}/basis)
  
  set(F0DISCOVERYDIR ${GEMDIR}/STM32F0Discovery)
  set(F1DISCOVERYDIR ${GEMDIR}/STM32F1Discovery)
  set(F3DISCOVERYDIR ${GEMDIR}/STM32F3Discovery)
  set(F4DISCOVERYDIR ${GEMDIR}/STM32F4Discovery)
  
  add_definitions(-DUSE_STDPERIPH_DRIVER)

endif(NOT CMAKESCRIPTDIR)
