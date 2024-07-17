all:
# 	g++ -Isrc/Include -Lsrc/lib -o main main.cpp -lmingw32 -lSDL2main -lSDL2

	g++ -Isrc/include -Isrc/include/imgui -Isrc/include/SDL2 -Lsrc/lib -Limage_src/lib -o main \
    main.cpp \
    imgui/imgui.cpp imgui/imgui_draw.cpp \
    imgui/imgui_widgets.cpp imgui/imgui_tables.cpp \
    imgui/imgui_impl_sdl2.cpp imgui/imgui_impl_sdlrenderer2.cpp \
    -lmingw32 -lSDL2main -lSDL2 -lSDL2_image
