Preparar el entorno virtual(recomendacion usar python3.9)
	1- sudo apt install python3.9-venv -----> instala el entorno
	2- python3.9 -m venv venv_3.9 -------> crea el entorno
	3- source venv_3.9/bin/activate ------> activa el entorno
	4- python3.9 -m pip install tensorflow==2.9.1
		        	    protobuf==3.19.6
		        	    onnx --no-deps
		        	    onnx-tf --no-deps
		        	    PyYAML
		        	    tensorflow-addons==0.19.0
		        	    tensorflow_probability[tf]
		        	    pip install protobuf==3.19.6 --upgrade


Si cierra el teminal, vuelve a abrir el entorno ya creado y da problemas, lo mejor es volver a realizar los pasos apartir del paso 2.



-----------------------------------------------------------
Para ejecutar el extractor.cpp realizar:

En la carpeta "proyecto" o donde este el .cpp ejecutar: 

    cmake -B build -DCMAKE_PREFIX_PATH=~/vcpkg/installed/x64-linux 
esto es para direccionar donde esta la libreria "nlohmann"

Luego ejecutar:

    cd build
    make


en el cmakelist creo que no es necesario pero lo dejo por aqui
"set(CMAKE_TOOLCHAIN_FILE "/home/romario/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")"















