

BASE_DIR=$(pwd)

SRC_DIR=$BASE_DIR"/src"
BUILD_DIR=$BASE_DIR"/build"
THIRDPARTY_DIR=$BASE_DIR"/thirdparty"

# -F framework include dir
# -I include dir

pushd $SRC_DIR
	clang++ main.cpp -I$THIRDPARTY_DIR"/include" -F$THIRDPARTY_DIR"/frameworks" -framework SDL2 -o $BUILD_DIR"/ludum_dare_38" 
popd