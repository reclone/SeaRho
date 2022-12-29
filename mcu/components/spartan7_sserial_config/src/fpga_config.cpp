
#include "fpga_config.h"

#include "driver/gpio.h"

#include <bit>
#include <climits>
#include <cstdio>
#include <iostream>
#include <span>


FpgaConfig::FpgaConfig(FpgaConfig::GpioAssignments pins, size_t configBufferSize) :
    m_pins{pins},
    m_configBuffer(configBufferSize)
{
    initGpio();
}

FpgaConfig::~FpgaConfig()
{
    deinitGpio();
}

void FpgaConfig::initGpio()
{
    // Program - output, initialized high
    gpio_set_level(m_pins.program, 1U);
    gpio_set_direction(m_pins.program, GPIO_MODE_OUTPUT);
    
    // Init - open-drain input/output, initialized high
    gpio_set_level(m_pins.init, 1U);
    gpio_set_direction(m_pins.init, GPIO_MODE_INPUT_OUTPUT_OD);
    
    // Configuration clock - output, initialized low
    gpio_set_level(m_pins.configClk, 0U);
    gpio_set_direction(m_pins.configClk, GPIO_MODE_OUTPUT);
    
    // Serial output (FPGA DIN) - output, initialized low
    gpio_set_level(m_pins.serialOut, 0U);
    gpio_set_direction(m_pins.serialOut, GPIO_MODE_OUTPUT);
    
    // Done - input
    gpio_set_direction(m_pins.done, GPIO_MODE_INPUT);
}

void FpgaConfig::deinitGpio()
{
    gpio_reset_pin(m_pins.configClk);
    gpio_reset_pin(m_pins.serialOut);
    gpio_reset_pin(m_pins.program);
    gpio_reset_pin(m_pins.done);
    gpio_reset_pin(m_pins.init);
}

esp_err_t FpgaConfig::configure(const char* filename, FpgaConfig::Verbosity verbose)
{
    if (verbose == Verbosity::Verbose)
    {
        std::cout << "Configuring FPGA with " << filename << " ...\n";
    }
    
    std::FILE* fp = std::fopen(filename, "rb");
    if (!fp)
    {
        if (verbose == Verbosity::Verbose)
        {
            std::cerr << "Failed to open " << filename << "\n";
        }
        return ESP_ERR_NOT_FOUND;
    }
    
    gpio_set_level(m_pins.program, 0U);
    while (gpio_get_level(m_pins.init))  //FIXME implement proper timeout
    {
        // Busy loop
    }
    
    gpio_set_level(m_pins.program, 1U);
    while (!gpio_get_level(m_pins.init))  //FIXME implement proper timeout
    {
        // Busy loop
    }
    
    auto readCount = fread(m_configBuffer.data(), 1U, m_configBuffer.size(), fp);
    while (readCount > 0U)
    {
        std::span<uint8_t> readSpan{m_configBuffer.data(), readCount};
        for (auto byteVal : readSpan)
        {
            for (unsigned int ii = 0U; ii < CHAR_BIT; ++ii)
            {
                gpio_set_level(m_pins.configClk, 0U);
                auto lvl = (byteVal & 0x80) ? 1U : 0U;
                gpio_set_level(m_pins.serialOut, lvl);
                
                byteVal = byteVal << 1U;
                gpio_set_level(m_pins.configClk, 1U);
            }
        }
        
        readCount = fread(m_configBuffer.data(), 1U, m_configBuffer.size(), fp);
    }
    
    std::fclose(fp);
    
    gpio_set_level(m_pins.configClk, 0U);
    gpio_set_level(m_pins.serialOut, 0U);
    
    if (!gpio_get_level(m_pins.done))
    {
        if (verbose == Verbosity::Verbose)
        {
            std::cerr << "Failed to configure FPGA\n";
        }
        return ESP_ERR_INVALID_STATE;
    }
    
    if (verbose == Verbosity::Verbose)
    {
        std::cout << "FPGA is successfully configured.\n";
    }
    
    return ESP_OK;
}
