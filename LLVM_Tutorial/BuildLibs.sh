#! /bin/bash

curr_path="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
cd $curr_path

cd Tests


for folder in `ls`
do
    if [ -d ./$folder ]
    then
        cd $folder
        if [ ! -d ./build ]
        then
            mkdir build
        fi
        cd ..
    fi
done

cd LLVM_Learner_Libs/build
cmake ../
make -j4
#cp `building/libHI_Test.a` ../
#cp `find -name *.a` ../
