#pragma once

#include <array>
#include <memory>
#include <string>
#include <vector>

//#include <executer.hpp>
//#include <layer.hpp>
#include "model.hpp"
//#include <runtime.hpp>

class InputModel : public Model {
 public:
  /**
   * @brief Construct a new InputModel object
   *
   *@param address cnn address
   */
  InputModel(const std::string& address);

  /**
   * @brief Destroy the InputModel object
   */
  ~InputModel();
  
  /**
   * @brief Number of layers of the LeNet5
   *
   */
  //static constexpr unsigned int num_layers = 13;

  /**
   * @brief Loads the model and its parameter from the binary files
   *
   * @param path folder to the model weights
   * @param paths files with the weights for each layer
   * @return Runtime
   */
  /*Runtime Load(const std::string &path,
               const std::vector<std::string> &paths) override;*/

  /**
   * @brief Execute the model provided an executor.
   *
   * It performs the inference on an input buffer and provides an output buffer
   *
   * @param executer executor engine
   * @param input input buffer
   * @param output output buffer
   * @return Runtime
   */
  /*Runtime Execute(std::shared_ptr<Executer> executer,
                  std::shared_ptr<DataContainer> input,
                  std::shared_ptr<DataContainer> output) override;*/
private:

   /**
   * @brief Construct a new InputModel object
   *
   *@param data data input
   *@param size dimensions number
   */
   std::vector<int> createArray(const std::string& data, int size);
};
