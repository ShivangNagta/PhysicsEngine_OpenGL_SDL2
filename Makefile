all:
	g++ -Isrc/include -Isrc/include/imgui -Isrc/include/SDL2 -Isrc/include/glad -Lsrc/lib -o bin/main \
	    main.cpp src/include/glad/glad.c -Isrc/include/glm \
	    -lmingw32 -lSDL2main -lSDL2
