

#ifndef FPGA_CONFIG_H
#define FPGA_CONFIG_H

#include "hal/gpio_types.h"

#include "esp_err.h"

#include <vector>

/// Bit-bangs a set of GPIO pins to configure a Spartan-7 FPGA via slave serial
class FpgaConfig final
{
public:

    /// GPIO pad assignments to be used for FPGA slave serial configuration
    struct GpioAssignments
    {
        /// Configuration clock (output, connected to FPGA's CCLK)
        gpio_num_t configClk = GPIO_NUM_17;
        
        /// Serial out (output, connected to FPGA's DIN)
        gpio_num_t serialOut = GPIO_NUM_27;
        
        /// Program (output, active low, connected to FPGA's PROGRAM_B)
        gpio_num_t program   = GPIO_NUM_25;
        
        /// Done (input, connected to FPGA's open-drain DONE)
        gpio_num_t done      = GPIO_NUM_34;
        
        /// Initialize (bidir, open-drain, connected to FPGA's INIT_B)
        gpio_num_t init      = GPIO_NUM_26;
    };
    
    enum class Verbosity : bool
    {
        Quiet = false,
        Verbose = true
    };

    FpgaConfig(GpioAssignments pins, size_t configBufferSize = 256U);
    
    ~FpgaConfig();

    esp_err_t configure(const char* filename, Verbosity verbose = Verbosity::Quiet);

private:

    const GpioAssignments m_pins;
    std::vector<uint8_t>  m_configBuffer;
    
    void initGpio();
    void deinitGpio();
    
    // Delete copy and move constructors
    FpgaConfig(const FpgaConfig&) = delete;
    FpgaConfig(FpgaConfig&&) = delete;
    
    // Delete copy and move assignment operators
    FpgaConfig& operator=(const FpgaConfig&) = delete;
    FpgaConfig& operator=(FpgaConfig&&) = delete;
};

#endif  // FPGA_CONFIG_H
