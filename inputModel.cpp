#include <array>
#include <memory>
#include <string>
#include <vector>

//#include <executer.hpp>
//#include <layer.hpp>
//#include <runtime.hpp>
#include "extractor.cpp"
#include "graph.cpp"
#include "inputModel.hpp"
#include <unordered_map>

InputModel::InputModel(const std::string& address){

    std::string addresmodel = address;
    Graph grafo = generarGrafo(addresmodel);
    // hacer logica para dejar el grafo con la info requerida por layer
    
    //layers_ = {};
    
    
    for(int i=0; i<grafo.nodesVector.size(); i++){
        
        std::vector<std::string> infoPrevlayer;
        std::vector<std::string> infolayer=grafo.nodesVector[i];
        std::vector<std::string> infoNextlayer;

        if(i>0){
            infoPrevlayer=grafo.nodesVector[i-1];;
        }

        if(i<grafo.nodesVector.size()-1){
            infoNextlayer=grafo.nodesVector[i+1];
        }
        
        
        
        std::string labelLayer= infolayer[0];
        std::cout << "*******************" << std::endl;
        std::cout << labelLayer << std::endl;
        //auto layer;

        /*0 = class_name
          1 = ID
          2 = activation
          3 = kernel
          4 = num_dimensions
          5 = poolings
          6 = pool_size
          7 = strides
          8 = padding
          9 = dilatations
          10 = dilation_rate*/

        
        //-----------calcular num_dimension y size---------
        int num_dimensionsInt=0;
        int dimensions[num_dimensionsInt];
        int size=1;

        if (infolayer[4] != "null"){
            num_dimensionsInt = std::stoi(infolayer[4]);
            std::vector<int> dimensionsVec = createArray(infolayer[3],0);

            for (int j=0; j<num_dimensionsInt; j++){
                dimensions[j]=dimensionsVec[j];
                size *= dimensionsVec[j];
            }
            size *= sizeof(float);
        }else{
            size=0;}

        std::cout << "size: " <<size << std::endl;
        std::cout << "num_dimensions: " << num_dimensionsInt << std::endl;

        int opcion=10;
        std::vector<int> dimensionsPrev;
        std::vector<int> dimensionsNext;
        int input_size=-1;
        int output_size=-1;
        /* opcion=
        0 next 
        1 both
        2 prev*/

        if (i>0){
            if(i<grafo.nodesVector.size()-1){
                if (infoNextlayer[0] == "Dense" and infoPrevlayer[0] == "Dense"){
                    dimensionsPrev = createArray(infoPrevlayer[3],0);
                    dimensionsNext = createArray(infoNextlayer[3],0);
                    opcion=1;
                }else if(infoPrevlayer[0] == "Dense"){
                    dimensionsPrev = createArray(infoPrevlayer[3],0);
                }else if(infoNextlayer[0] == "Dense"){
                    dimensionsNext = createArray(infoNextlayer[3],0);
                    opcion=0;
                }
            }else{
                if (infoPrevlayer[0] == "Dense" ){
                    dimensionsPrev = createArray(infoPrevlayer[3],0);
                    opcion=2;
                }
            }
        }else{
            if(infoNextlayer[0] == "Dense"){
                opcion=0;
            }
        }

        

        switch(opcion) {
            case 0:
                output_size= dimensionsNext[0];
                
                break;
            case 1:
                input_size= dimensionsPrev[1];
                output_size= dimensionsNext[0];
                break;
            case 2:
                input_size= dimensionsPrev[1];
                output_size= dimensionsPrev[1];
                break;
        }

        if (labelLayer == "Conv2D") {

            std::vector<int> stridesVec = createArray(infolayer[7],num_dimensionsInt);
            std::vector<int> paddingVec = createArray(infolayer[8],num_dimensionsInt);
            std::vector<int> dilatationsVec = createArray(infolayer[10],num_dimensionsInt);
           
            int strides[num_dimensionsInt];
            int padding[num_dimensionsInt];
            int dilatations[num_dimensionsInt];
            

            for (int j=0; j<num_dimensionsInt; j++){
                strides[j]=stridesVec[j];
                padding[j]=paddingVec[j];
                dilatations[j]=dilatationsVec[j];
            }
            //layers_.push_back(std::make_shared<Layer<Conv2D>>());
            //layer = std::dynamic_pointer_cast<Layer<Conv2D>>(layers_[i]);
            //layer->type = Layers::CONV2D;
            //layer-> size = size;
            //layer->props = Conv2D{.num_dimensions = num_dimensionsInt,
            //                      .dimensions = dimensions,
            //                      .strides = strides,
            //                      .padding = padding
            //                      .dilatations = dilatations};

        }
        else if (labelLayer == "Add") {

            
            // layers_.push_back(std::make_shared<Layer<Add>>());
            //layer = std::dynamic_pointer_cast<Layer<Add>>(layers_[i]);
            //layer->type = Layers::ADD;
            //layer-> size = size;
            //layer->props = Add{.num_dimensions = num_dimensionsInt,
            //                      .dimensions = dimensions,
            //                      .preceding = layers_[i - 1]};
        }
        else if (labelLayer == "DepthConv2D") {

            std::vector<int> stridesVec = createArray(infolayer[7],num_dimensionsInt);
            std::vector<int> paddingVec = createArray(infolayer[8],num_dimensionsInt);
            std::vector<int> dilatationsVec = createArray(infolayer[10],num_dimensionsInt);
           
            int strides[num_dimensionsInt];
            int padding[num_dimensionsInt];
            int dilatations[num_dimensionsInt];
            

            for (int j=0; j<num_dimensionsInt; j++){
                strides[j]=stridesVec[j];
                padding[j]=paddingVec[j];
                dilatations[j]=dilatationsVec[j];
            }
            // layers_.push_back(std::make_shared<Layer<DepthConv2D>>());
            //layer = std::dynamic_pointer_cast<Layer<DepthConv2D>>(layers_[i]);
            //layer->type = Layers::DEPTHWISE_CONV2D;
            //layer-> size = size;
            //layer->props = Conv2D{.num_dimensions = num_dimensionsInt,
            //                      .dimensions = dimensions,
            //                      .strides = strides,
            //                      .padding = padding
            //                      .dilatations = dilatations};
        }
        else if (labelLayer == "Dense") {

            input_size= dimensions[0];
            output_size= dimensions[1];
            
            // layers_.push_back(std::make_shared<Layer<Dense>>());
            //layer = std::dynamic_pointer_cast<Layer<Dense>>(layers_[i]);
            //layer->type = Layers::DENSE;
            //layer-> size = size;
            //layer->props = Dense{.num_dimensions = num_dimensionsInt,
            //                      .dimensions = dimensions};
        }
        else if (labelLayer == "Multiplier") {
            // layers_.push_back(std::make_shared<Layer<Multiplier>>());
            //layer = std::dynamic_pointer_cast<Layer<Multiplier>>(layers_[i]);
            //layer->type = Layers::MULTIPLIER;
            //layer-> size = size;
            //layer->props = Add{.num_dimensions = num_dimensionsInt,
            //                      .dimensions = dimensions,
            //                      .preceding = layers_[i - 1]};
        }
        else if (labelLayer == "MaxPooling") {

            // pos 5 es pooling o pool_size es 6
            std::vector<int> poolingsVec;
            std::vector<int> stridesVec = createArray(infolayer[7],num_dimensionsInt);

            if (infolayer[5] == "null"){
                poolingsVec = createArray(infolayer[6],num_dimensionsInt);
            }else{
                poolingsVec = createArray(infolayer[5],num_dimensionsInt);
            }

            int strides[num_dimensionsInt];
            int poolings[num_dimensionsInt];
            for (int j=0; j<num_dimensionsInt; j++){
                poolings[j]=poolingsVec[j];
                strides[j]=stridesVec[j];
            }

            // layers_.push_back(std::make_shared<Layer<MaxPooling>>());
            //layer = std::dynamic_pointer_cast<Layer<MaxPooling>>(layers_[i]);
            //layer->type = Layers::MAX_POOL;
            //layer-> size = size;
            //layer->props = Pooling{.num_dimensions = num_dimensionsInt,
            //                      .poolings = poolings,
            //                      .strides = strides};
        }
        else if (labelLayer == "AveragePooling" or labelLayer == "AveragePooling2D")  {

            // pos 5 es pooling o pool_size es 6
            std::vector<int> poolingsVec;
            std::vector<int> stridesVec = createArray(infolayer[7],num_dimensionsInt);

            if (infolayer[5] == "null"){
                poolingsVec = createArray(infolayer[6],num_dimensionsInt);
            }else{
                poolingsVec = createArray(infolayer[5],num_dimensionsInt);
            }

            int strides[num_dimensionsInt];
            int poolings[num_dimensionsInt];
            for (int j=0; j<num_dimensionsInt; j++){
                poolings[j]=poolingsVec[j];
                strides[j]=stridesVec[j];
            }
            

            // layers_.push_back(std::make_shared<Layer<AveragePooling>>());
            //layer = std::dynamic_pointer_cast<Layer<AveragePooling>>(layers_[i]);
            //layer->type = Layers::AVG_POOL;
            //layer-> size = size;
            //layer->props = Pooling{.num_dimensions = num_dimensionsInt,
            //                      .poolings = XXXXXXX,
            //                      .strides = strides};
        }
        else if (labelLayer == "Activation") {
            // layers_.push_back(std::make_shared<Layer<Activation>>());
            //layer = std::dynamic_pointer_cast<Layer<Activation>>(layers_[i]);
            //layer->type = Layers::ACTIVATION;
            //layer-> size = size;
            //layer->props = Mapper{.minvalue = -1e6,
            //                      .maxvalue = 1e6};
        }
        else if (labelLayer == "Padding") {

            std::vector<int> paddingVec = createArray(infolayer[8],num_dimensionsInt);
            int padding[num_dimensionsInt];
            
            for (int j=0; j<num_dimensionsInt; j++){
                padding[j]=paddingVec[j];
            }

            // layers_.push_back(std::make_shared<Layer<Padding>>());
            //layer = std::dynamic_pointer_cast<Layer<Padding>>(layers_[i]);
            //layer->type = Layers::PADDING;
            //layer-> size = size;
            //layer->props = Padding{.num_dimensions = num_dimensionsInt,
            //                      .padding = padding,
            //                      .preceding = layers_[i - 1]};
        }
        else if (labelLayer == "Layerwise") {
            // layers_.push_back(std::make_shared<Layer<Layerwise>>());
            //layer = std::dynamic_pointer_cast<Layer<Layerwise>>(layers_[i]);
            //layer->type = Layers::LAYERWISE;
            //layer-> size = size;
            //layer->props = Layerwise{.initial_value = 0,
            //                      .operation = XXXXXXXXXX};
        }
        else if (labelLayer == "ReductionActivation") {
            // layers_.push_back(std::make_shared<Layer<ReductionActivation>>());
            //layer = std::dynamic_pointer_cast<Layer<ReductionActivation>>(layers_[i]);
            //layer->type = Layers::REDUCT_ACTIVATION;
            //layer-> size = size;
        }
        else {
            // layers_.push_back(std::make_shared<Layer<Invalid>>());
            std::cout << "Capa no reconocida o no esta dentro de las integradas" << std::endl;
        }

        //std::cout << "debug2" << std::endl;
        //-----------determina activacion---------
        if (infolayer[2]=="\"tanh\""){
            std::cout << "tanh" << std::endl;
            //layer->activation = Activations::TANH;
        }
        else if(infolayer[2]=="\"relu\""){
            std::cout << "relu" << std::endl;
            //layer->activation = Activations::RELU;
        }
        else if(infolayer[2]=="\"softmax\""){
            std::cout << "softmax" << std::endl;
            //layer->activation = Activations::SOFTMAX;
        }
        else{
            std::cout << "No requiere activacion o la activacion no esta registrada" << std::endl;
            //layer->activation = Activations::NONE;
        }

        //std::cout << "debug3" << std::endl;
        //layer->num_inputs = 1;
        //layer->num_outputs = 1;


        //layer->input_size = input_size;
        //layer->output_size = output_size;
        std::cout << "input_size = " << input_size << std::endl;
        std::cout << "output_size = " << output_size << std::endl;
        

    }
    /*for(int i=0; i<grafo.nodesVector.size(); i++){
        std::cout << grafo.nodesVector[i][0] << std::endl;
        
    }

    for(int i=0; i<grafo.adjVector.size(); i++){
        std::cout << grafo.adjVector[i][0] << " ------> " << grafo.adjVector[i][1] << std::endl;
    }*/
    
}

InputModel::~InputModel(){};


std::vector<int> InputModel::createArray(const std::string& data, int size){
    std::vector<int> final;
    int value;

    
    if (data == "\"valid\""){
        value=1;
        for (int i=0 ; i<size ; i++){
            final.push_back(value);
        }
    }
    else if(size == 0){
        std::stringstream dataSs(data);
        std::string token;
        // Dividir la cadena usando 'x' como delimitador
        while (std::getline(dataSs, token, 'x')) {
            final.push_back(std::stoi(token)); // Convertir cada token a entero
        }
    }
    else{
        value= data[1] - '0';
        for (int i=0 ; i<size ; i++){
            final.push_back(value);
        }
    }
    return final;
} 



/*Runtime InputModel::Load(const std::string &path,
                     const std::vector<std::string> &paths) {
  std::stringstream msg_ss;

  if (paths.size() != layers_.size()) {
    msg_ss << "Sizes between the paths and layers do not match"
           << " Paths: " << paths.size() << " Layers: " << layers_.size();
    return Runtime{Runtime::INCOMPATIBLE_PARAMETER, msg_ss.str()};
  }

  for (unsigned int i = 0; i < layers_.size(); ++i) {
    if (layers_[i]->size == 0) {
      continue;
    }
    Runtime res = ReadWeights(path + "/" + paths[i], layers_[i].get());
    if (res.code) {
      return res;
    }
  }

  msg_ss << num_layers << " layers read";
  return Runtime{0, msg_ss.str()};
}

Runtime InputModel::Execute(std::shared_ptr<Executer> executer,
                        std::shared_ptr<DataContainer> input,
                        std::shared_ptr<DataContainer> output) {
  if (!executer) {
    return Runtime{Runtime::INVALID_PARAMETER, "Executer is null"};
  }

  return executer->Run(layers_, input, output);
}*/
