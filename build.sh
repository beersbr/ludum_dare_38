

BASE_DIR=$(pwd)

SRC_DIR=$BASE_DIR"/src"
BUILD_DIR=$BASE_DIR"/build"
THIRDPARTY_DIR=$BASE_DIR"/thirdparty"

# -F framework include dir
# -I include dir

cp -r thirdparty/frameworks/* $BUILD_DIR/

pushd $SRC_DIR
	clang++ main.cpp -I$THIRDPARTY_DIR"/include" -F$THIRDPARTY_DIR"/frameworks" -framework SDL2 -framework opengl -o $BUILD_DIR"/ludum_dare_38" \
	-rpath @executable_path/ \
	-D SLOW
popd