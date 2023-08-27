cmake_minimum_required(VERSION 3.0)
#Данный блок отвечает за установку переменных, а также создание
#непосредственно самого проекта на основе переменных
#удобнее всего в одном месте задать название проекта, а дальше
#ссылаться на него, чтобы не допустить ошибку или же для простого
#рефакторинга
set(PROJECT_NAME Snake_minigame)
set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})

project(${PROJECT_NAME} VERSION ${PROJECT_VESRION})
#-------

#тут мы делаем переменную Sources, чтобы потом один раз ее 
#использовать, в данном случае у нас всего лишь один файл,
#но на больших преоктах, где много файлов, удобно ее применять,
#чтобы при создании программы и/или библиотеки можно было указать всего лишь
#одну переменную вместо списка всех файлов 
set(Sources src/main.cpp)

add_executable(${PROJECT_NAME} ${Sources})

#тут мы устанавливаем стандарт c++, что он необходим, а также
#что необходимы его расширения
set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS ON
)

#магическая функция cmake, которая позволяет в автоматизировать процесс
#подключения библиотек и различных пакетов, она вместо нас создает и задает
#все необходимые переменные, для облегченного подключения библиотек и пакетов к нашему
#проекту, но эта функция может найти не все библиотеку так, что иногда придется создавать
#спец файлы для поисках этих библиотек самостоятельно
find_package(SFML 2.5 COMPONENTS graphics audio system window QUIET)

#дальше идет блок, который в зависимости от выполнения предыдущей 
#команды cmake find_package() будет определять и сообщать юзеру,
#о том будет ли использоваться библиотека, которая нашлась (если) на компьютере юзера,
#или она будет скачена и собрана вместе с проектом
if(SFML_FOUND)
    #вывод сообщения об успешном нахождение библиотеки на компе
    message("SFML was successfully found")
else()
    #вывод сообщения, что библиотеку не удалось найти
    message("CMAKE couldn't find sfml")

    #подключения модуля CMake для скачивания
    include(FetchContent)
    set(SFML_VERSION "2.6.0")
    
    #скачивание библиотки
    FetchContent_Declare(
        sfml
        GIT_REPOSITORY "https://github.com/SFML/SFML.git"
        GIT_TAG        "${SFML_VERSION}"
    )

    #в данном блоке поисходит получение необходимых свойств для сборки библиотеки 
    #а также создание условий, чтобы библиотека стала доступной для твоего проекта
    #(уход на низкий уровень CMake, чаще всего ты будешь использовать 
    #FetchContent_MakeAvailable())
    FetchContent_GetProperties(sfml)
    if(NOT sfml_POPULATED)
        FetchContent_Populate(sfml)
        add_subdirectory(${sfml_SOURCE_DIR} ${sfml_BINARY_DIR})
    endif()
    #-----
endif()
#-----

#подключение библиотеки (не знаю какие части тебе библиотеки тебе тут нужны,
#поэтому подключил все)
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        sfml-audio
        sfml-graphics
        sfml-system
        sfml-window
)

#этот блок отвечает за опции (флаги компиляции), которые необходимы.
#В данному случае они устанавливаются в зависимости от компилятора
if (MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        /W4
    )
else ()
    target_compile_options(${PROJECT_NAME} PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
endif()



