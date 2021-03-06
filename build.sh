


# echo "-- FREETYPE IS REQUIRED! -- "

BASE_DIR=$(pwd)

SRC_DIR=$BASE_DIR"/src"
BUILD_DIR=$BASE_DIR"/build"
THIRDPARTY_DIR=$BASE_DIR"/thirdparty"

# -F framework include dir
# -I include dir


cp -r thirdparty/frameworks/* $BUILD_DIR/

mkdir -p $BUILD_DIR/shaders 
cp -r $SRC_DIR/shaders/* $BUILD_DIR/shaders/

mkdir -p $BUILD_DIR/images
cp -r $SRC_DIR/images/* $BUILD_DIR/images/

mkdir -p $BUILD_DIR/fonts
cp -r $SRC_DIR/fonts/* $BUILD_DIR/fonts/


pushd $SRC_DIR
    clang++ main.cpp graphics.cpp controller_manager.cpp level.cpp util.cpp core.cpp game.cpp font.cpp \
    -F$THIRDPARTY_DIR"/frameworks" -I$THIRDPARTY_DIR"/include" \
    -framework SDL2 -framework SDL2_Image -framework opengl \
    -o $BUILD_DIR"/ludum_dare_38" -std=c++11 \
    -L$BASE_DIR"/thirdparty/lib/macosx/" \
    -lfreetype \
    -rpath @executable_path/ \
    -D SLOW
popd