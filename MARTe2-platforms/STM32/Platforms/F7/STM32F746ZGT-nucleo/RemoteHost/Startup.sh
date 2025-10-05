#!/bin/sh
MARTe2_DIR=/home/giuseppe/FullProject/MARTe2
MARTe2_LIBS=$(find ${MARTe2_DIR}/Build/x86-linux/Core/ -name "*.so")
for lib in ${MARTe2_LIBS}; do
    export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:$(dirname ${lib})
done
MARTe2_components_DIR=/home/giuseppe/FullProject/MARTe2-components
MARTe2_components_LIBS=$(find ${MARTe2_components_DIR} -name "*.so")
for lib in ${MARTe2_components_LIBS}; do
    export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:$(dirname ${lib})
done
MARTe2_extensions_DIR=/home/giuseppe/FullProject/marte-extensions/
MARTe2_extensions_LIBS=$(find ${MARTe2_extensions_DIR}/target/lib/Build/x86-linux/Components/ -name "*.so")
for lib in ${MARTe2_extensions_LIBS}; do
    echo $lib
    export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:$(dirname ${lib})
done
echo $LD_LIBRARY_PATH
${MARTe2_DIR}/Build/x86-linux/App/MARTeApp.ex -l RealTimeLoader -f $1 -m StateMachine:START 
