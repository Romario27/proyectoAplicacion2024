#include <H5Cpp.h>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <typeinfo>
#include <list>
#include <nlohmann/json.hpp>
#include "graph.cpp"
#include <algorithm> 
#include <cctype> 

using json = nlohmann::json;
using namespace H5;


std::vector<std::vector<std::string>> infolist;

// Función recursiva para listar los grupos y datasets
void exploreGroup(const Group& group, const std::string& path = "/") {
    hsize_t num_objs = group.getNumObjs();  // Obtener el número de objetos en el grupo
    
    std::vector<std::string> lista;
    bool save = false;

    // obtiene el nombre de la capa
    size_t first_slash = path.find('/', 0);  // Primer '/'
    size_t second_slash = path.find('/', first_slash + 1);  // Segundo '/'
    size_t third_slash = path.find('/', second_slash + 1);  // Tercer '/'
    std::string layerName = path.substr(second_slash + 1, third_slash - second_slash - 1);
    //layerName = "Name: " + layerName ;
    //lista.push_front(layerName);
    lista.insert(lista.begin(), layerName);

    for (hsize_t i = 0; i < num_objs; ++i) {
        std::string obj_name = group.getObjnameByIdx(i);
        H5G_obj_t obj_type = group.getObjTypeByIdx(i);
        std::string full_path = path + obj_name;

       
        if (obj_type == H5G_GROUP) {
            //std::cout << " -> Group" << std::endl;
            Group subgroup = group.openGroup(obj_name);
            exploreGroup(subgroup, full_path + "/");  // Llamada recursiva para explorar el subgrupo
            
        } else if (obj_type == H5G_DATASET) {
            DataSet dataset = group.openDataSet(obj_name);
            DataSpace dataspace = dataset.getSpace();
            DataType datatype = dataset.getDataType();

            //Obtiene el nombre del parametros
            size_t colon_pos = obj_name.find(':');
            std::string paramName = obj_name.substr(0, colon_pos);

            // calcula las dimenciones del dataset y del kernel
            int num_dimensions = dataspace.getSimpleExtentNdims();
            hsize_t dims_out[num_dimensions];
            int ndims = dataspace.getSimpleExtentDims( dims_out, NULL);
            
            
            if (num_dimensions == 1){
                //std::cout << paramName <<": " << (unsigned long)(dims_out[0]) << std::endl;
                std::string bias = std::to_string((unsigned long)(dims_out[0]));
                bias = paramName + ": " + bias;
                lista.push_back(bias);

            } else {
                if (num_dimensions==2){
                    /*std::cout <<  "Kernel: " <<
                    (unsigned long)(dims_out[0]) << " x " <<
                    (unsigned long)(dims_out[1]) << std::endl;*/

                    // Crear un ostringstream
                    std::ostringstream oss;

                    // Escribir en el ostringstream en lugar de std::cout
                    oss << paramName <<": " << (unsigned long)(dims_out[0]) << "x" <<
                            (unsigned long)(dims_out[1]);

                    // Convertir a std::string
                    std::string Kernel = oss.str();
                    lista.push_back(Kernel);
                    std::string Snum_dimensions = "num_dimensions: " + std::to_string(num_dimensions);
                    lista.push_back(Snum_dimensions);

                } else{ 
                    if (num_dimensions==3){
                        /*std::cout <<  "Kernel: " <<
                        (unsigned long)(dims_out[0]) << " x " <<
                        (unsigned long)(dims_out[1]) << " x " <<
                        (unsigned long)(dims_out[2]) << std::endl;*/

                    // Crear un ostringstream
                    std::ostringstream oss;

                    // Escribir en el ostringstream en lugar de std::cout
                    oss << paramName <<": " <<  (unsigned long)(dims_out[0]) << "x" <<
                            (unsigned long)(dims_out[1]) << "x" <<
                            (unsigned long)(dims_out[2]);

                    // Convertir a std::string
                    std::string Kernel = oss.str();
                    lista.push_back(Kernel);
                    std::string Snum_dimensions = "num_dimensions: " + std::to_string(num_dimensions);
                    lista.push_back(Snum_dimensions);

                    } else{
                        /*std::cout <<  "Kernel: " <<
                        (unsigned long)(dims_out[0]) << " x " <<
                        (unsigned long)(dims_out[1]) << " x " <<
                        (unsigned long)(dims_out[2]) << " x " <<
                        (unsigned long)(dims_out[3]) << std::endl;*/

                        // Crear un ostringstream
                        std::ostringstream oss;

                        // Escribir en el ostringstream en lugar de std::cout
                        oss << paramName <<": " << (unsigned long)(dims_out[0]) << "x" <<
                                (unsigned long)(dims_out[1]) << "x" <<
                                (unsigned long)(dims_out[2])<< "x" <<
                                (unsigned long)(dims_out[3]);

                        // Convertir a std::string
                        std::string Kernel = oss.str();
                        lista.push_back(Kernel);
                        std::string Snum_dimensions = "num_dimensions: " + std::to_string(num_dimensions);
                        lista.push_back(Snum_dimensions);
                    }
                }
            }

            /* // este podria servir para los pesos
            if (datatype.getClass() == H5T_FLOAT){
                hsize_t num_elements = dataspace.getSimpleExtentNpoints();
                std::vector<float> data(num_elements);  // Asumiendo que los datos son de tipo float
                dataset.read(data.data(), datatype, dataspace);

                int contador = 0;

                // Imprimir los datos
                for (const auto& value : data) {
                    contador++;
                    //std::cout << "Valor: " << value << std::endl;
                }
            }*/
        save = true;
        
        } else if (obj_type == H5G_TYPE) {
            std::cout << " -> Named datatype" << std::endl;
        }
    }
    if (save){
        if (lista.size()<6){
            infolist.push_back(lista);}
    }
}

// Función para procesar cada capa y sus configuraciones
std::vector<std::vector<std::string>> parseLayers(const json& config) {
    std::vector<std::vector<std::string>> layers_info;
    int id=0;
    
    for (const auto& layer : config) {
        std::vector<std::string> layer_params;
        // Agregar la clase de la capa
        layer_params.push_back("class_name: " + layer["class_name"].get<std::string>());
        
        // Obtener la configuración de la capa
        const auto& config = layer["config"];
        for (json::const_iterator it = config.begin(); it != config.end(); ++it) {
            // Convertir cada parámetro en string
            std::string key = it.key();
            std::string value = it->dump();  // Convertimos el valor a string usando dump()
            layer_params.push_back(key + ": " + value);
        }

        // Añadir los parámetros de esta capa a la lista
        std::string idString= "ID: " + std::to_string(id);
        layer_params.insert(layer_params.begin(), idString); // añade al inicio de layer_param el valor idString
        layers_info.push_back(layer_params);
        id++;
    }
    
    return layers_info;
}


std::vector<std::vector<std::string>> sincronizeVector(const std::vector<std::vector<std::string>>& info){
    std::vector<std::vector<std::string>> layers_info = info;
    //sincroniza los 2 resultados
    bool encontrado;
    // i lleva el indice del infolist
    for (int i=0; i<infolist.size();i++){
        // j lleva la posicion de la lista de listas 
        encontrado=false;
        for (int j=0; j<layers_info.size();j++){
            // k lleva cada parametro de la lista
            for (int k=0 ; k<layers_info[j].size();k++){
                bool result = layers_info[j][k].find(infolist[i][0]) != std::string::npos;
                if (result){
                    //infolist[i].erase(infolist[i].begin());
                    layers_info[j].insert(layers_info[j].begin()+1, infolist[i].begin(), infolist[i].end());
                    k=layers_info[j].size();
                    encontrado=true;
                }
            }
            if (encontrado){
                j=layers_info.size();
            }
        }
    }
    return layers_info;
}


std::vector<std::vector<std::string>> cleanVector(const std::vector<std::vector<std::string>>& info){

    std::vector<std::vector<std::string>> infoSend;
    std::vector<std::string> labels;

    labels.push_back("class_name"); 
    labels.push_back("ID");
    labels.push_back("activation");
    labels.push_back("kernel");
    labels.push_back("num_dimensions");
    //labels.push_back("bias");
    labels.push_back("poolings");
    labels.push_back("pool_size");
    labels.push_back("strides");
    labels.push_back("padding");
    labels.push_back("dilatations");
    labels.push_back("dilation_rate");
    int count=0;
    bool añadido=false;
    
    for (int i=0; i<info.size();i++){
        std::vector<std::string> aux;
        // j lleva la posicion de la lista de labels 
        for (int j=0; j<labels.size();j++){
            // k lleva cada parametro de la lista
            for (int k=0; k<info[i].size(); k++){
                //tranforma todo en minuscula
                std::string str = info[i][k];
                std::string toFind = labels[j];
                std::transform(str.begin(), str.end(), str.begin(), ::tolower);
                std::transform(toFind.begin(), toFind.end(), toFind.begin(), ::tolower);
    
                bool result = str.find(toFind) != std::string::npos;
                if (result){
                    std::string value= info[i][k];
                    size_t pos = value.find(':');
                    if (pos != std::string::npos) {
                        // Obtener el valor (parte después del ":")
                        value = value.substr(pos + 1);
                        // Quitar posibles espacios en blanco
                        value.erase(0, value.find_first_not_of(" \n\r\t"));
        }
                    if(count=!j){
                        int range= j-count;
                        for(int t=0;t<range;t++){
                            aux.insert(aux.begin()+count,"null");
                            count++;
                        }
                        aux.push_back(value);
                    }else{
                        aux.push_back(value);
                        count++;
                    }
                    k=info[i].size();
                    añadido=true;
                }
            }
            if (!añadido){
                aux.push_back("null");
            }
            añadido=false;
        }
        std::cout<< aux.size()<< std::endl;
        infoSend.push_back(aux);
    }
    return infoSend;
}




Graph generarGrafo(const std::string& address) {

    Attribute modelConfig;
    try {
        // Abre el archivo .h5
        H5File file(address, H5F_ACC_RDONLY);

        
        // Abre el grupo raíz
        Group root_group = file.openGroup("/");

        // abre y parsea el model_config
        modelConfig = file.openAttribute("model_config"); 

        /*
        // abre y parsea el training_config
        //Attribute trainingConfig = file.openAttribute("training_config");
        
        //std::string layerName = jsonObject["config"]["layers"][0]["name"];
        //std::string inbound_nodes = jsonObject["config"]["layers"][1]["inbound_nodes"][0]["args"][0]["config"]["keras_history"][0];
        */

        // Explora el grupo raíz
        exploreGroup(root_group);
        //exploreModelConfig(modelConfig);
        

    } catch (FileIException error) {
        error.printErrorStack();
    } catch (GroupIException error) {
        error.printErrorStack();
    } catch (DataSetIException error) {
        error.printErrorStack();
    }


    DataType dataType = modelConfig.getDataType();
    std::string modelConfigValue;
    modelConfig.read(dataType, modelConfigValue);
    json modelConfigJson= json::parse(modelConfigValue);

    std::vector<std::vector<std::string>> layers_info;
    


    if (modelConfigJson["class_name"]== "Functional"){
        layers_info = parseLayers(modelConfigJson["config"]["layers"]);
    }else{
        layers_info = parseLayers(modelConfigJson["config"]);
    }

    // sincronza las 2 listas
    layers_info = sincronizeVector(layers_info);

    // limpiar vector
    layers_info= cleanVector(layers_info);
    
    // se crea el grafo
    Graph grafo;


    
    // Agregar nodos con su información
    for (int i=0; i<layers_info.size(); i++){
        std::string id = layers_info[i][1];
        std::vector<std::string> vec= layers_info[i];
        grafo.addNode(id, vec);
        if (i!=0){
            grafo.addEdgeVector({layers_info[i-1][1],layers_info[i][1]});
            grafo.addEdge(layers_info[i-1][1],layers_info[i]);
        }
        
    }
    //grafo.printGraph();

    //Imprimir los resultados   
    std::cout<< "-------- Informacion de cada capa---------"<<std::endl; 
    for (const auto& layer : layers_info) {
        for (const auto& param : layer) {
            std::cout << param << std::endl;
        }
        std::cout << "-------------------------" << std::endl;
    }

    

/*    for (size_t i = 0; i < infolist.size(); ++i) {
        for (std::string val : infolist[i]) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }*/

    return grafo;
}
