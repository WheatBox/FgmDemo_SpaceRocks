# 使用下列指令进行编译 | Use these commands to compile

# cmake -S . -B build
# cd build
# make

# 如果你安装了 VS 但依然想用 MinGW 的话 | If you installed VS but want to use MinGW

# cmake -S . -B build -G "MinGW Makefiles"
# cd build
# make

cmake_minimum_required (VERSION 3.15)

project (MyGame)

include_directories (FxxkGML)

add_compile_options(-Wall -Wno-unknown-pragmas)

macro(build _name _outpath)
message(${_outpath})
	add_library (${_name} SHARED
		FxxkGML/FxxkGML/vars.cpp
		FxxkGML/FxxkGML/math.cpp
		FxxkGML/FxxkGML/basic.cpp
		FxxkGML/FxxkGML/miscellaneous.cpp
		FxxkGML/FxxkGML/draw.cpp
		FxxkGML/FxxkGML/instance.cpp
		FxxkGML/FxxkGML/layer.cpp
		FxxkGML/FxxkGML/game_input.cpp
		FxxkGML/FxxkGML/debug.cpp
		FxxkGML/FxxkGML/audio.cpp
		FxxkGML/FxxkGML/sprite.cpp
		FxxkGML/FxxkGML/font.cpp
		FxxkGML/FxxkGML.cpp
		${ARGN}
	)
	set_target_properties(${_name} PROPERTIES
		PREFIX ""
		SUFFIX ""
		RUNTIME_OUTPUT_DIRECTORY ${_outpath}
	)
endmacro(build)

# ----------------------------------------------
# 在此处追加你要编译的 dll
# Add the dll you want to compile here
# 
# 格式：
# build (目标文件名 目标输出路径
#     *.cpp文件们
# )
# 
# Format:
# build (dest_filename dest_output_path
#     *.cpp_files
# )

set (OutDir ../Out/Space)

build (Game.dll ${OutDir}
	Code/Game.cpp

	FxxkGML/wini/wini.cpp
)

build (Player.dll ${OutDir}/Player
	Code/Player/Player.cpp
)

build (Bullet.dll ${OutDir}/Player
	Code/Player/Bullet.cpp
)

build (Rock.dll ${OutDir}/Enemies
	Code/Enemies/Rock.cpp
)

# ----------------------------------------------